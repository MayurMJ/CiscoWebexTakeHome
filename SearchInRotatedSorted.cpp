#include <iostream>
#include <vector>
#include <assert.h>


/*
 * Searches for a target element in a rotated sorted array,
 * If the element is found returns the index of the element 
 * in the array, or else returns -1
*/

int SearchRotatedSortedArray(std::vector<int> arr, int target) {
    if(arr.size() == 0) return -1;
    int low = 0, high = arr.size() - 1;
    while(low < high) {
        int mid = low + (high - low) / 2;
        if(arr[mid] == target) return mid;

        // First half of array is sorted, if the target lies
        // between first and mid search in first half else
        // search in second.

        if(arr[low] <= arr[mid]) {
            if(target >= arr[low] && target < arr[mid]) {
                high = mid - 1;
            } else{
                low = mid + 1;
            }
        }
        
        // Second half of array is sorted, if the target lies
        // between mid and Last, search in second half else
        // search in first.
        else {
            if(target > arr[mid] && target <= arr[high]) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
    }
    return arr[low] == target ? low : -1;
}


int main()
{
    assert(SearchRotatedSortedArray({4,5,6,7,0,1,2}, 0) == 4); // Input case
    assert(SearchRotatedSortedArray({4,5,6,7,0,1,2}, 3) == -1); // Input case
    assert(SearchRotatedSortedArray({1}, 0) == -1); // Input case
    assert(SearchRotatedSortedArray({1,2,3,4,5,6}, 3) == 2); // sorted array;
    assert(SearchRotatedSortedArray({4,5,6,7,0,1,2}, 4) == 0); // First element ;
    assert(SearchRotatedSortedArray({4,5,6,7,0,1,2}, 2) == 6); // Last element
    assert(SearchRotatedSortedArray({3,1}, 1) == 1); // reverse sorted
    assert(SearchRotatedSortedArray({}, 0) == -1); // empty array
    return 0;
}
