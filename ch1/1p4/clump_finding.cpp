#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <zlib.h>
#include "../../helpers/kseq.h"
#include "../../helpers/generics.h"
#include <vector>
#include <set>

using namespace std;

KSEQ_INIT(gzFile, gzread)

int main(int argc, char* argv[])
{
    gzFile fp = gzopen(argv[1],"r");
    kseq_t* seq = kseq_init(fp);

    int k = atoi(argv[2]);
    int L = atoi(argv[3]);
    int t = atoi(argv[4]);

    set<string> globalClumps;

    int l;
    if((l = kseq_read(seq)) >= 0 )
    {
        string genome = seq->seq.s;
        int length = seq->seq.l;

        string window = genome.substr(0,L);
        int numKmers = 1 << 2*k;
        vector<int> frequencies (numKmers);
        int maxCount = FillFrequencies(window, k, frequencies);
        
        for( int i=0; i < numKmers; ++i )
        {
            if( frequencies[i] >= t )
            {
                string kmer = NumberToPattern(i,k);
                //cout << "adding: " << kmer << endl;
                globalClumps.insert(kmer);
            }
        }

        for( int i=1; i<length-L+1; ++i )
        {
            string leavingSubstr = genome.substr(i-1,k);
            //cout << "leaving: " << leavingSubstr << endl;
            int leavingIx = PatternToNumber(leavingSubstr);
            if( frequencies[leavingIx] > 0 )
                --frequencies[leavingIx];
            
            string enteringSubstr = genome.substr(i+L-k,k);
            //cout << "entering: " << enteringSubstr << endl;
            int enteringID = PatternToNumber(enteringSubstr);
            ++frequencies[enteringID];
            if( frequencies[enteringID] >= t )
            {
                //cout << "adding: " << enteringSubstr << endl;
                globalClumps.insert(enteringSubstr);
            }

        }

        set<string>::iterator it;
        for( it=globalClumps.begin(); it != globalClumps.end(); ++it )
        {
            cout << *it << " ";
        }
        cout << endl;
        cout << globalClumps.size() << endl;
        cout << endl << endl;

        kseq_destroy(seq);
        gzclose(fp); 
    }
}
