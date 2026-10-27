/**
 * This file implements parallel mergesort.
 */

#include <stdio.h>
#include <string.h> /* for memcpy */
#include <stdlib.h> /* for malloc */
#include "mergesort.h"

/* this function will be called by mergesort() and also by parallel_mergesort(). */
void merge(int leftstart, int leftend, int rightstart, int rightend){
	// Merge will use a temp array B. Create temp iterators for B, and left/right subarrays
	int index = 0;
	int left = leftstart;
	int right = rightstart;
	// We iterate through both arrays until all elements have been copied into B
	while ((left != leftend) && (right != rightend)) {
		// Compare & insert smaller element, iterating respective pointer
		if (A[left] <= A[right]) {
			B[index] = A[left];
			left++;
		} else {
			B[index] = A[right];
			right++;
		}
		index++;
	}
	// Insert sorted array back into A
	memcpy(A[leftstart], B[leftstart], (rightend - leftstart + 1));
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
		return NULL;
}

/* we build the argument for the parallel_mergesort function. */
struct argument * buildArgs(int left, int right, int level){
		struct argument arg = {
			.left = left,
			.right = right,
			.level = level,
		};
		return &arg;
}

