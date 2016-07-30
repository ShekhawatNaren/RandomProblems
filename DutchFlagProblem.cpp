#include "stdafx.h"
#include <vector>
#include <algorithm>
#include <iostream>
/*
Its a 3-way partitioning algorithm which maintains the invariant
v[0...low-1]<v[low...mid-1]<v[mid..high-1]=unkown(yet to be scanned & qualified)<v[high...N-1]
Running time O(n), sorts the data in single pass
*/
void DutchFlagSorting(std::vector<char> &v) {
	size_t low = 0;
	size_t mid = 0;
	size_t high = v.size()-1;
	while (mid < high) {
		switch (v[mid]) {
		case 'R':
			std::swap(v[mid], v[low]);
			mid++;
			low++;
			break;
		case 'W':
			mid++;
			break;
		case 'B':
			std::swap(v[mid], v[high]);
			high--;
			break;
		}
	}
}
int main()
{
	std::vector<char> v{ 'W','B','B','R','W','R','B','W','R','B' };
	std::for_each(v.begin(), v.end(), [](char& c) {std::cout << c << ","; });
	std::cout << std::endl;
	DutchFlagSorting(v);
	std::for_each(v.begin(), v.end(), [](char& c) {std::cout << c << ","; });
	std::cout << std::endl;
	return 0;
}