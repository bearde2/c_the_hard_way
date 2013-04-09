#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>

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

// a typedef creates a fake type, in this
// case for a function pointer
typedef int (*compare_cb)(int a, int b);
typedef int (*sort_alg)(int *a, int b, compare_cb c);

/**
 * A classic bubble sort function that uses the 
 * compare_cb to do the sorting. 
 */
sort_alg *bubble_sort(int *numbers, int count, compare_cb cmp)
{
    int temp = 0;
    int i = 0;
    int j = 0;
    int *target = malloc(count * sizeof(int));

    if(!target) die("Memory error.");

    memcpy(target, numbers, count * sizeof(int));

    for(i = 0; i < count; i++) {
        for(j = 0; j < count - 1; j++) {
            if(cmp(target[j], target[j+1]) > 0) {
                temp = target[j+1];
                target[j+1] = target[j];
                target[j] = temp;
            }
        }
    }

    return target;
}

/**
 * Second sorting algorithm, quicksort. !!Also implement heapsort!!
 */
sort_alg *quick_sort(int *numbers, int count, compare_cb cmp){
	 while(count > 2){	
 		int i = 0;
 		int j = 0;
 		int k = 0;
 		int *target = malloc((count-1) * sizeof(int));
 		int *target2 = malloc(count * sizeof(int));
 		
 		//int target[count-1];
 		//int target2[count];
 		
 		if(!target) die("Memory error");
 		if(!target2) die("Memory error");
 		
 		memcpy(target2, numbers, count*sizeof(int));
 		//target2 = numbers;
 		//printf("%d, %d\n", sizeof(target)/sizeof(int), sizeof(target2)/sizeof(int));
 		
 		int pivot = count/2;
 		int *top_array = (int *)malloc(sizeof(int));
 		int *bottom_array = (int *)malloc(sizeof(int));
 		top_array[0] = 0;
 		bottom_array[0] = 0;
 		
 		int pivot_value = target[pivot-1];
 		//printf("%d\n", pivot);
 		/**the following is meant to remove the pivot value from target2 
 		  *and copy the shortened list to target, but nalthe count is off
 		  */
 		for(i = 0; i < count; i++){
 			//printf("i = %d: j = %d\n", &target2[i], &target[j]);
 			if(i != pivot){
 				memcpy((target+j), (target2+i), sizeof(int));
 				//printf("%d:%d\n", target[i], target2[i]);
 				j++;
 			}
 			printf("i = %d: j = %d\n", target2[i], target[j]);
 		}
 		j = 0;
 		
 		//test target and target2:
 		/*for(i = 0; i < count; i++){
 			printf("%d:%d\n", target[i], target2[i]);
 		}*/
 		
 		
 		for(i = 0; i < count; i++){
 			//printf("%d\n", target[i]);
 			if(cmp(target[i], pivot_value)>0){
				//printf("%s: %d\n", "top_array[j]", target[i]);
 				memcpy(&top_array[j], &target[i], sizeof(int));
 				printf("%s: %d\n", "top_array[j]", top_array[j]);
 				j++;
 				top_array = realloc(top_array, (j+1)*sizeof(int));
 			} else if(cmp(target[i], pivot_value)<= 0){
 				//printf("%s: %d\n", "bottom_array[k]", target[i]);
 				memcpy(&bottom_array[k], &target[i], sizeof(int));
 				printf("%s: %d\n", "bottom_array[k]", bottom_array[k]);
 				k++;
 				bottom_array = realloc(bottom_array, (k+1)*sizeof(int));
 			} 
 			//printf("%d\n", pivot_value);
 		}
	
	int top_size = sizeof(top_array)/sizeof(int);
	int bottom_size = sizeof(bottom_array)/sizeof(int);
	
 	quick_sort(top_array, top_size, cmp);
 	quick_sort(bottom_array, bottom_size, cmp);
 	
 	memcpy(target, top_array, sizeof(top_array));
 	memcpy((target+top_size-1), pivot_value, sizeof(int));
 	memcpy((target+top_size), bottom_array, sizeof(bottom_array));
 	
 	free(top_array);
 	free(bottom_array);
 	free(target);
 	free(target2);
 
 	return target;
 	}	
 }

int sorted_order(int a, int b)
{
    return a - b;
}

int reverse_order(int a, int b)
{
    return b - a;
}

int strange_order(int a, int b)
{
    if(a == 0 || b == 0) {
        return 0;
    } else {
        return a % b;
    }
}

/** 
 * Used to test that we are sorting things correctly
 * by doing the sort and printing it out.
 */
void test_sorting(int *numbers, int count, compare_cb cmp, sort_alg sort)
{
    int i = 0;
    int *sorted = sort(numbers, count, cmp);

    if(!sorted) die("Failed to sort as requested.");

    for(i = 0; i < count; i++) {
        printf("%d ", sorted[i]);
    }
    printf("\n");

    free(sorted);
    
    /*unsigned char *data = (unsigned char *)cmp;

	for(i = 0; i < 25; i++) {
	    printf("%02x:", data[i]);
	}

	printf("\n");*/
}


int main(int argc, char *argv[])
{
    if(argc < 3) die("USAGE: ex18 b 4 3 1 5 6");

    int count = argc - 2;
    int i = 0;
    char **inputs = argv + 2;
    char sort = argv[1][0];
    
    int *alg = malloc(sizeof(sort_alg));

    int *numbers = malloc(count * sizeof(int));
    if(!numbers) die("Memory error.");
	
	for(i = 0; i < count; i++){
		numbers[i] = 0;
	}
	
    for(i = 0; i < count; i++) {
        numbers[i] = atoi(inputs[i]);
    }
    
    switch(sort){
    	case 'b':
    		alg = bubble_sort;
    		break;
    	
    	case 'q':
    		alg = quick_sort;
    		break;
    	
    	default:
    		die("Choose an algorithm: 'b' for bubblesort, 'q' for quicksort");
    }

    test_sorting(numbers, count, sorted_order,alg);
    test_sorting(numbers, count, reverse_order, alg);
    test_sorting(numbers, count, strange_order, alg);

    free(numbers);

    return 0;
}
