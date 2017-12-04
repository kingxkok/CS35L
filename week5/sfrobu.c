#include "limits.h"
#include "stdlib.h" //qsort
#include "string.h"
#include "stdio.h" //fprintf for errors
#include "unistd.h"
#include <sys/types.h>
#include <sys/stat.h>
#include "ctype.h"



const int ESTIMATED_AVG_WORD_SIZE = 10;

int 
frobcmp(const void *in1,  const void *in2)
{

  const char* a = *(const char**)in1;
  const char* b = *(const char**)in2;
  for(;; a++, b++) //Loop through word char by char
  {
  	if(*a == ' ' && *b == ' ') { return 0; } //end of both words, and were same 
  	//frob does XOR 42 to a byte, so reverse with ^42
	else if (*a == ' ' ) { return -1; } //a is shorter 
	else if (*b == ' ' ) { return 1; } //b is shorter 
	else if ((*a^42) < (*b^42)) return -1;
	else if ((*a^42) > (*b^42)) return 1;
  }
}

int 
toupper_frobcmp(const void *in1,  const void *in2)
{

  const char* a = *(const char**)in1;
  const char* b = *(const char**)in2;
  for(;; a++, b++) //Loop through word char by char
  {
  	if(*a == ' ' && *b == ' ') { return 0; } //end of both words, and were same 
  	else if (*a == ' ' ) { return -1; } //a is shorter 
	else if (*b == ' ' ) { return 1; } //b is shorter 
  	//frob does XOR 42 to a byte, so reverse with ^42
  	else if ((*a^42)>=0 && (*b^42)>=0 ){ //can use toupper
		if (toupper(*a^42) < toupper(*b^42)) return -1;
		else if (toupper(*a^42) > toupper(*b^42)) return 1;
	}
	
	else if ((*a^42) < (*b^42)) return -1;
	else if ((*a^42) > (*b^42)) return 1;

  }
}

size_t 
my_write(char* buf, char** total_buffer){
	size_t len = 0;
	while(*buf != ' ') {
		**total_buffer = *buf;
		buf++;
		(*total_buffer)++;
		len++;
	}
	**total_buffer = ' '; //add space at the end
	(*total_buffer)++;
	len++;

	return len;
}


int
main (int argc, char** argv){

	int IGNORE_CASE = 0;
	if(argc > 1){
		if(strcmp("-f", argv[1]) == 0)
			IGNORE_CASE=1;
	}

	char* buf;
	struct stat file_info;
	ssize_t num_read;
	struct stat* fip = &file_info;
	ssize_t total_read;
	ssize_t buffer_size;

	int status = fstat(0, fip);
	off_t   file_size = file_info.st_size;
//	fprintf(stdout, "%lld", file_info.st_size);

	// Initial Read
	buf = malloc(file_size*sizeof(char));
	num_read = read(0, buf, file_size); 
	buffer_size = file_size+1024;
	total_read = num_read;
	// Keep reading as file grows
	do{
		if(total_read +1024 >= buffer_size){
			buffer_size = (buffer_size*2)+1024;
			
			//fprintf(stdout, "%lld ", buffer_size);
			char* resized_buf = realloc(buf, buffer_size*sizeof(char));
			if(resized_buf != NULL){
				buf = resized_buf;
			}
			else {
				free(buf);
				fprintf(stderr, "Error while Allocating Memory!");
				exit(1);
			}
		}
		num_read = read(0, buf+total_read, 2);
		if(num_read < 0){
			fprintf(stderr, "Error while reading!");
			fprintf(stderr, "Total read: %lld; Buffer Size: %lld", total_read, buffer_size);
			exit(1);
		}
		total_read += num_read;
	//	if(buffer_size > 200000) break;
	}while(num_read != 0); 
	//finish reading
	*(buf+total_read) = ' '; //make sure there's a space at the end
	*(buf+total_read+1) = EOF; //to mark end of buf 

	//mark words
	size_t est_num_words = total_read/ESTIMATED_AVG_WORD_SIZE;
	size_t wordListSize = est_num_words; //size as in mem alloced
	char** wordList = malloc(sizeof(char*)*(wordListSize));
	char* word = buf;
	char* charIter = buf; //goes thru each char in the buf
	size_t wordIndex = 0;

	//ignore leading spaces
	while(*charIter == ' ') charIter++; //ignore extra spaces
	word = charIter; //set word to point to next word

	while(*charIter != EOF){ 
		if(*charIter == ' '){ //end of word
			 if(wordIndex + 2 > wordListSize){ //make sure there's enough memory
				 wordListSize = (wordIndex*2 + 4);
				char** newList = realloc(wordList, sizeof(char*)*wordListSize);
				if(newList != NULL) wordList = newList;
				else { //check if realloc worked
					fprintf(stderr, "Error while Allocating Memory!\n" );
					free(buf);
					free(wordList);
					exit(1);
				}
			} 
			//there's now enough memory for wordList
			wordList[wordIndex] = word; //add word to list
			wordIndex++;
			while(*charIter == ' ') charIter++; //ignore extra spaces
			word = charIter; //set word to point to next word
			continue; //since charIter and word have already been incremented
		} 
		// else *charIter is not a space so keep moving
		charIter++;
	} 

	if(IGNORE_CASE)
		qsort(wordList, wordIndex, sizeof(char*), toupper_frobcmp);
	else 
		qsort(wordList, wordIndex, sizeof(char*), frobcmp);


	char* buf_for_print = malloc(sizeof(char)*buffer_size);
	char* buf_iter = buf_for_print;
	size_t total_len = 0;
	for(int i = 0; i < wordIndex; i++)
		total_len += my_write(wordList[i], &buf_iter);
	
	write(1, buf_for_print, total_len);

	free(buf);
	free(buf_for_print);
	free(wordList);
	exit(0);
}