#include "translation.h"
#include <sstream>
#include <stdlib.h>

const map<string,char> AminoAcids::codon2acid = AminoAcids::create_map();

const map<char,int> AminoAcids::acid_weights = AminoAcids::create_acid_weight_map();

const char AminoAcids::acids[] = {'G','A','S','P','V','T','C','I','L','N','D','K','Q','E','M','H','F','R','Y','W'};

const int AminoAcids::weights[] = {57,71,87,97,99,101,103,113,114,115,128,129,131,137,147,156,163,186};

int AminoAcids::Mass(const string peptide)
{
    int mass = 0;
    for( int i=0; i<peptide.length(); i++ )
    {
        mass += AminoAcids::acid_weights.at(peptide[i]);
    }
    return mass;
}

int AminoAcids::Mass2(vector<int> peptide)
{
    int mass = 0;
    vector<int>::iterator it;
    for( it = peptide.begin(); it!= peptide.end(); ++it )
    {
        mass += *it;
    }
    return mass;
}
