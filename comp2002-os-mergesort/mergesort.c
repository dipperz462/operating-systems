/**
 * This file implements parallel mergesort.
 */

#include <stdio.h>
#include <string.h> /* for memcpy */
#include <stdlib.h> /* for malloc */
#include "mergesort.h"

/* this function will be called by mergesort() and also by parallel_mergesort(). */
void merge(int leftstart, int leftend, int rightstart, int rightend){
	// Merge will use temp array B. Create temp iterators for B, and left/right subarrays
	int index = leftstart;
	int left = leftstart;
	int right = rightstart;
	// We iterate through both arrays, copying into B until one subarray is empty.
	while ((left <= leftend) && (right <= rightend)) {
		// Compare & insert smaller element, iterating respective pointer
		if ((A[left] <= A[right])) {
			B[index++] = A[left++];
		} else {
			B[index++] = A[right++];
		}
	}

	// Copy remaining elements from the left half
    while (left <= leftend) {
        B[index++] = A[left++];
    }

    // Copy remaining elements from the right half
    while (right <= rightend) {
        B[index++] = A[right++];
    }

	// Insert sorted array back into A
	memcpy(&A[leftstart], &B[leftstart], ((rightend - leftstart + 1)*sizeof(int)));
}

/* this function will be called by parallel_mergesort() as its base case. */
void my_mergesort(int left, int right){
	// base case
	if (left >= right) {return;}
	// get midpoint
	int mid = left + (right - left) / 2;
	// sort subarrays
	my_mergesort(left, mid);
	my_mergesort(mid + 1, right);
	// merge subarrays
	merge(left, mid, mid + 1, right);
}

/* this function will be called by the testing program. */
void * parallel_mergesort(void *arg){
		//cast void pointer to argument struct pointer
		struct argument *args = (struct argument*) (arg);
		//base case
		if(args->left>=args->right) {
			return NULL;}
		//cutoff case (move from parallel to serial)
		else if(args->level >= cutoff){
			my_mergesort(args->left, args->right);
			return NULL;
		};
		//parallel case (level < cutoff)
		// get midpoint
		int mid = args->left + (args->right - args->left) / 2;
		// create arguments for left thread
		struct argument * leftArgs = buildArgs(args->left, mid, args->level + 1);

		// create arguments for right thread
		struct argument * rightArgs = buildArgs(mid + 1, args->right, args->level + 1);

		//declares pthread variables
		pthread_t leftThread;
		pthread_t rightThread;

		//creates both threads with correct arguments
		pthread_create(&leftThread,NULL,parallel_mergesort,leftArgs);
		pthread_create(&rightThread,NULL,parallel_mergesort,rightArgs);

		//waits for both threads to finish
		pthread_join(leftThread,NULL);
		pthread_join(rightThread,NULL);

		//merges sub arrays arranged with left and right threads
		merge(args->left,mid,mid+1,args->right);
		free(leftArgs);
		free(rightArgs);
		return NULL;
}

/* we build the argument for the parallel_mergesort function. */
struct argument * buildArgs(int left, int right, int level){
		struct argument * arg = malloc(sizeof *arg);
		arg->left = left;
		arg->right = right;
		arg->level = level;
		return arg;
}