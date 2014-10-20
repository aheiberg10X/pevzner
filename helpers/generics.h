#ifndef GENERICS_INCLUDED
#define GENERICS_INCLUDED

#include <string>
#include <vector>
#include <set>

using namespace std;

void ReverseComplement(string input, string& output);

int PatternSearch(string text, int length, string pattern, int pattern_len);

vector<int> PatternMatch(string text, string substring);

set<string> FrequentWords( string text, int length, int k, int thresh );
set<string> FrequentWordsFast( string text, int length, int k, int thresh );

int PatternToNumber(string pattern);

string NumberToPattern(int number, int length);

int BaseToNumber(char b);
char NumberToBase(int t);

int FillFrequencies(string text, int k, vector<int>& frequencies);

#endif
