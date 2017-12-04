#include "limits.h"
#include "stdlib.h" //qsort
#include "string.h"
#include "stdio.h" //fprintf for errors
#include "unistd.h"

enum { N_CHARS = UCHAR_MAX + 1};


int cmpfunc( const void *a, const void *b) {
  return *(char*)a - *(char*)b;
}

/* Array of character values defining the translation (if any) that
   tr is to perform.  */
static char xlate[N_CHARS];

int
main (int argc, char** argv){
	int len = 0;
	if(argc < 3) {
		fprintf(stderr, "Too few arguments!\nTwo arguments: [from] [to]!\n");
		exit(1);
	}
	if(argc > 3) {
		fprintf(stderr, "Too many arguments!\nTwo arguments: [from] [to]!\n");
		exit(1);
	}

	while(argv[1][len] != '\0'){
		++len;
	}
	int len_to = 0;
	while(argv[2][len_to] != '\0'){
		++len_to;
	}
	if(len != len_to) {
		fprintf(stderr, "From and To strings are not the same length!\n");
		exit(1);
	}
	char* from = (char*)malloc((len+1)*sizeof(char));
	strcpy(from, argv[1]); //null-terminated
	char* to = argv[2];

	
	char* copy = (char*)malloc((len+1)*sizeof(char));
	copy = strcpy(copy,from);

	//sort to see if duplicates
	qsort(copy, len, sizeof(char), cmpfunc);
	//see duplicate
	for(int i = 0; i < len-1; i++){
		if( copy[i] == copy[i+1] ) {
			fprintf(stderr, "Duplicate in from string!\n");
			exit(1);
		}
	}
	
	//set up mapping function
	for (int i = 0; i < N_CHARS; i++)
	    xlate[i] = i; //default map to itself
	for (int i = 0; i < len; i++){
		xlate[from[i]] = to[i];
	}

	int read_return = 0;
	char curr;
	char* curr_p = &curr;

	while( (read_return = read(0, curr_p, 1)) > 0) {
	 *curr_p = xlate[*curr_p];
	 write(1, curr_p, 1);
	} 

	if(read_return < 0) {
		fprintf(stderr, "Error while reading!");
		exit(1);
	}

}