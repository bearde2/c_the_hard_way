#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>


void die(const char *message)
{
    if(errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }

    exit(1);
}

int money_mach(int coin, int zrs){
	
	int i = 0;
		
	for(i = 2; i < 5; i++){
		if(coin/i == 0){
			zrs++;
		} else{
			zrs = money_mach(coin/i, zrs);
		}
	}
		
	return zrs;
}

int main( int argc, char *argv[]){
	if(argc < 2) die("need more arguments");
	if(argc > 2) die("too many arguments");
	
	int coin = atoi(argv[1]);
	int zrs = 0;
	
	int results = money_mach(coin, zrs);
	
	printf("%d\n",results);
	
	return 0;
}
