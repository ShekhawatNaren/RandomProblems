#include "stdafx.h"
#include <string>

int RollingHash(int prevHash,char prevFirstChar, const std::string& str, int patternLength) {
	int prime = 101;
	return ((prevHash - int(prevFirstChar)) / prime) + int(str[str.length() - 1]) * std::pow(prime, patternLength - 1);
}

int CalculateHash(const std::string& str) {
	int prime = 101;
	int strHash = 0;
	for (size_t i = 0; i < str.length(); i++) {
		strHash = strHash + int(str[i])* std::pow(prime, i);
	}
	return strHash;
}
/*
Returns the index positing in the text string where the pattern match start, -1 if no match
*/
int RabinKarpStringMatch(std::string text, std::string pattern) {
	int patternHash = CalculateHash(pattern);
	int textHash = CalculateHash(text.substr(0, pattern.length()));

	for (size_t i = 0; i < (text.length() - pattern.length() + 1); i++) {
		if (patternHash == textHash && (0 == pattern.compare(text.substr(i, pattern.length())))) {
			return i;
		}
		textHash = RollingHash(textHash, text[i], text.substr(i + 1, pattern.length()), pattern.length());
	}

	return -1;
}

int main()
{
	int pos = RabinKarpStringMatch("acdbbabc", "abc");
	int pos1 = RabinKarpStringMatch("acdbbac", "abc");
	return 0;
}
