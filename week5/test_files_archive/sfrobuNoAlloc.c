/*
 * Name: Ng, Xian Kai
 * UID: 204785637
 * ngxiankai286@gmail.com
 * Professor: Eggert
 * File - sfrob.c - Program accepts frobnicated words and sort 
 * in unfrobnicated order without storing the unfrobnicated values
 * in memory.
 */

 #include "stdio.h"
 #include "stdlib.h"
 #include "unistd.h"
 #include <string.h>
 #include <errno.h>

//char-by-char unfrobnication and then compare
//pointers to pointers to chars are what our words array is
//(pointers to words)
//qsort requires void* args
int frob_cmp(const void *in1,  const void *in2)
{

  const char* a = *(const char**)in1;
  const char* b = *(const char**)in2;
  for(;; a++, b++) //Loop through word char by char
  {
  	if(*a == ' ' && *b == ' ') { return 0; } //end of both words, and were same 
  	//frob does XOR 42 to a byte, so reverse with ^42
	else if (*a == ' ' || ((*a^42) < (*b^42))) { return -1; } //a is shorter or less
	else if (*b == ' ' || ((*a^42) > (*b^42))) { return 1; } //b is shorter or less
  }
}

//must do this error check each time we get error from read()
void read_with_error_check(char* buf)
{
	int read_return = read(0, buf, 1);
	if(read_return < 0){
	  fprintf(stderr, "Error while reading file: %s", strerror(errno));
	  exit(1);
	}
	if(read_return == 0)
		*buf = EOF;
}



int main(void){
	char** wordList; //array of pointers to words
	wordList = (char**)malloc(sizeof(char*));
	char* word; //particular word of interest in one moment
	word = (char*)malloc(sizeof(char));

	//current and next char to check for EOF and add space
	char curr; 
	char* curr_p = &curr;
	read_with_error_check(curr_p);

	char next;
	char* next_p = &next;
	read_with_error_check(next_p);

	int charIndex = 0;
	int wordIndex = 0;
	int wordSize = 1;
	int wordListSize = 1;

	 while(curr != EOF){
	 	word[charIndex] = curr; //Add letters to the word

	 	//word grows, double size to amortize realloc costs
		if(charIndex + 2 > wordSize) {
			wordSize = ((charIndex<<1) + 2);
  			char* resized_word = realloc(word, (wordSize)*sizeof(char));
	  		if(resized_word != NULL)
			{
			  //Make word point to  reallocated space
			  word = resized_word;
			}
			else //Allocation error, print error and exit
			{
			  for(size_t i = 0; i < wordIndex; i++)
			  {
			 	free(wordList[i]);
			  } 
		  	  free(wordList);
			  free(word);
			  fprintf(stderr, "Error while Allocating Memory!");
			  exit(1);
			}
		}	

		if(curr == ' '){ //end of word!
			wordList[wordIndex] = word;
			wordIndex++;
			//Set word back to empty by pointing it to other space
			//GOTTA DO THIS EVERY TIME A NEW WORD IS HIT.. NOT ONLY WHEN I REALLOC
			//LOL.
			word = NULL;
			word = (char*)malloc(sizeof(char));
			charIndex = -1; //-1 to bring back to 0 since ++ later
			wordSize = 1;
			//resize word list if needed
			if(wordIndex+2 > wordListSize) {
				wordListSize = ((wordIndex<<1) + 2); //doubling size errored at first :c
				char** more_words = realloc(wordList, (wordListSize)*sizeof(char*));
				if(more_words != NULL)
				{
				  	//Point wordList to newly allocated space
					wordList = more_words;
				}
				else //Allocation error, free everything, print error and exit
				{
					for(size_t i = 0; i < wordIndex; i++)
					{
					  free(wordList[i]);
					} 
				  	free(wordList);
				  	free(word);
				  	fprintf(stderr, "Error Allocation Memory!");
				  	exit(1);
				}
			} else {

			}
		}

		if(next == EOF && curr == ' ') 
			break;
		else if (curr == ' ' && next == ' '){
			//skip extra spaces
			while(curr == ' '){
				read_with_error_check(curr_p);
			}
			read_with_error_check(next_p);
			charIndex++;
			continue;
		}
		else if (next == EOF){
			//end but no space, so add
			curr = ' ';
			charIndex++;
			continue;
		}
		//increment character index and get next char
		curr = next;
		read_with_error_check(next_p);
		charIndex++;
	 } //end main while loop


	 //Sort frobbed words
	 qsort(wordList, wordIndex, sizeof(char*), frob_cmp);


	 //Output the words to STDOUT using putchar
	 for(size_t i = 0; i < wordIndex; i++){
	  	for(size_t j = 0;; j++){
			//EOF error checking
			if(putchar(wordList[i][j]) == EOF){
			  fprintf(stderr, "Error while writing character!");
			  for(size_t k = 0; k < wordIndex; k++){
			  	free(wordList[i]);
			  }
			  free(wordList);
			  free(word);
			  exit(1);
			}
			if(wordList[i][j] == ' '){
			  break;
			}
		}
	  }
	 //De-allocate all the space taken up for the words
	 for(size_t i = 0; i < wordIndex; i++){
	  	free(wordList[i]);
	 }
	 free(wordList);
	 free(word);

	 exit(0);
}