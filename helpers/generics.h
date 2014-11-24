#ifndef GENERICS_INCLUDED
#define GENERICS_INCLUDED

#include <string>
#include <vector>
#include <set>
#include <map>

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

int PopulateSkew(string sequence, int length, vector<int>& skews, bool min);

int HammingDistance(string a, string b);

const vector<int> ApproxMatches(string genome, string pattern, int mismatches);

const vector<int> GetSpectrum(string peptide, bool cyclic);
const vector<int> GetSpectrum2(const vector<int>& peptide, bool cyclic);

int Score(string peptide, map<int,int> weight_counts, bool cyclic );
int Score2(const vector<int>& peptide, map<int,int> weight_counts, bool cyclic );


const map<int,int> SpectralConvolution(const vector<int>& peptide);

const set<string> Neighbors( string pattern, int d );

float Entropy(const vector< vector<float> >& profile);

float ProfileProbability(string pattern, const vector< vector<float> >& profile);

int MotifScore( const vector<string>& motif );

string ProfileMostProbable( const vector< vector<float> >& profile, string pattern, int k);

const vector< vector<float> > BuildProfile( vector<string> motif );

#endif
