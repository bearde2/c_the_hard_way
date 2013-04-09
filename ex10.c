#include <stdio.h>

int main(int argc, char *argv[])
{
	int i=0;
	//int j=0;
	printf("argc: %d\n", argc);
	//go through each sting in argv
	//why am I skipping argv[0]? argv[0] is the function call "./ex10"
	for(i = 1/*,j=0*/; i < argc/*,j<argc*/; i++/*,j++*/){
		printf("arg %d: %s\n", i, argv[i]);
		//printf("arg %d: %s\n", j, argv[j]);
	}
	
	//let's make our own array of strings
	char *states[] = {
		"California", "Oregon", "Washington", "Texas"
	};
	int num_states = 4;
	
	for(i=0; i<num_states; i++){
		printf("state %d: %s\n", i, states[i]);
	}
	
	return 0;
}
