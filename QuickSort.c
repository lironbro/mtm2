/*
 * QuickSort.c
 *
 *  Created on: 5 במאי 2016
 *      Author: Liron
 */



typedef void* Element;

/*
 * the comparing function of two given arrays
 *
 * @return
 * positive value if the first element is "larger" than the second one
 * 0 if the elements are of equal "size"
 * negative value if the second element is "larger" than the first one
 */

typedef int(*CompareElements)(Element, Element);

/*
 * swaps the values of two elements
 *
 * @param a The first element
 * @param a The second element
 */

static void swap(Element* a, Element* b)
{
	Element temp = *a;
	*a = *b;
	*b = temp;
}

/*
 * sorts a given array using quicksort
 *
 *
 * @param arr The array which needs sorting
 * @param n The length of arr
 * @param compare The comparing function for the given array
 *
 *
 */
void quickSort(Element* arr, int n, CompareElements compare)
{
	Element pivot;
	int start=1, end=n-1;
	if(n<2)
		return;
	swap(&arr[0], &arr[n/2]);
	pivot = arr[0];
	while(start<=end){
		while(end >= start && compare(arr[end], pivot) > 0){
			end--;
		}
		while(start <= end && compare(arr[start], pivot) < 0){
			start++;
		}
		if(start < end){
			swap(arr[start++], arr[end--]);
		}
	}
	swap(&arr[0], &arr[end]);
	quickSort(arr, end, compare);
	quickSort(arr+end+1, n-end-1, compare);
}



