// RandomProblems.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<iostream>
#include<unordered_map>
#include<vector>
#include<algorithm>
#include<string>
using namespace std;
/*
Input: Vector of words(string)
Output: groups the anagrams and prints them to console
Approach: Builds a dictonary(hash table) where key is the sorted words hash value 
   and value is the list of anagram words
*/
void groupAnagrams(std::vector<string> words) {
	unordered_map < string, vector<string>> myAnagramDict;
	for (size_t index = 0; index < words.size();index++) {
		string sortedWord = words[index];
		std::sort(sortedWord.begin(), sortedWord.end());
		auto iter = myAnagramDict.find(sortedWord);
		if (iter == myAnagramDict.end()) {
			vector<string> anagrams{ words[index] };
			myAnagramDict.emplace(sortedWord, anagrams);
		}
		else {
			iter->second.push_back(words[index]);
		}
	}
	for (auto anagramIter = myAnagramDict.begin(); anagramIter != myAnagramDict.end(); anagramIter++) {
		for_each(anagramIter->second.cbegin(), anagramIter->second.cend(), [](const string &singleWord) { cout << singleWord <<","; });
	}
}
bool mySortFunc(string str1, string str2) {
	std::sort(str1.begin(), str1.end());
	std::sort(str2.begin(), str2.end());
	return str1<str2;
}
int main()
{
	cout << "First Approach..\n";
	vector<string> words{ "god","how","least","slate","dog","stale","who","steal" };
	groupAnagrams(words);

	cout << "\nSecond Approach..\n";
	sort(words.begin(), words.end(), mySortFunc);
	for_each(words.cbegin(), words.cend(), [](const string &singleWord) { cout << singleWord << ","; });

    return 0;
}

