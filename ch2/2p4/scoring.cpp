#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <zlib.h>
#include "../../helpers/kseq.h"
#include "../../helpers/generics.h"
#include "../../helpers/translation.h"
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <string>
#include <numeric>

using namespace std;


int main(int argc, char* argv[])
{
    string peptide = "PEPE";
    string spectrum = "0 97 97 129 194 196 226 226 244 258 323 323 452";
    
    istringstream iss(spectrum);
    string token;
    map<int,int> weight_counts;
    while( getline(iss,token,' ') )
    {
        int weight = atoi(token.c_str());
        if( weight_counts.find(weight) != weight_counts.end() )
        {
            weight_counts[weight]++;
        }
        else
        {
            weight_counts.insert( pair<int,int>(weight,1) );
        }
    }

    cout << Score(peptide, weight_counts, false) << endl;
    return 0;
}
