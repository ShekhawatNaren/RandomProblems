#include "stdafx.h"
#include "iostream"
#include <vector>

/*
Input: Takes an STL vector of ints
Output: Returns index of the minimum value in the array
Assumption: Following assumptions are made
		1. The vector contains distinct values
		2. Vector is sorted (increasing order) and can be rotated (ex 3,4,5,6,1,2)
*/
int findMin(vector<int> v) {
	int lowerIndex = 0;
	int higherIndex = v.size()-1;
	if (higherIndex < lowerIndex)
		return -1;
	else {
		int midIndex = (higherIndex - lowerIndex) / 2;
		while (midIndex!=0) {
			if (v[midIndex] > v[higherIndex])
				lowerIndex = midIndex + lowerIndex;
			else
				higherIndex = higherIndex - midIndex;
			midIndex = (higherIndex - lowerIndex) / 2;
		}
		return v[lowerIndex] < v[higherIndex] ? lowerIndex : higherIndex;
	}
}

int main()
{
  //A sorted array has been rotated so that the elements might appear in the order 3,4,5,6,1,2. How would you find the minimum element
	vector<int> vP1{ 3,4,5,6,7,1,2 };
	int m1 = findMin(vP1);

	vector<int> vP2{ 6,7,1,2,3,4,5 };
	int m2 = findMin(vP2);
	
	return 0;
}
