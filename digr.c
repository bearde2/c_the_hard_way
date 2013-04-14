#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

/** Our old friend die from ex17. */
void die(const char *message)
{
    if(errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }

    exit(1);
}

int dr(int number){

	uint64_t i = 10;
	int j = 1,k = 0;
	int sum = 0;
	
	// figure out how many digits (j) and set (i) to 10^j
	while(number - number%i != 0){
		i = i*10;
		j += 1;
	}
	i /= 10;
	
	int digits[j];
	
	// set each digit in the input into an array element
	for(k = 0; k < j; k++){
		if(i > 0){
			digits[k] = number/i;
		}
		number = number - digits[k]*i;
		i /= 10;
	}
	
	// sum over the array
	for(k = 0; k < j; k++){
		sum += digits[k];
	}
	
	// if the sum is 2-digit, recursively call the function
	if(sum > 9){
		sum = dr(sum);
	}
	
	return sum;
}


int main(int argc, char *argv[])
{
	if(argc < 2) die("need more arguments");
	if(argc > 2) die("too many arguments");
	
	int i = 0;
	float number = atoi(argv[i+1]);
	//double prev = 0;
	
	printf("%d\n", dr(number));
	
	return 0;
}
