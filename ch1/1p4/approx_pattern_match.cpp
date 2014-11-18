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
    const vector<int> matches = ApproxMatches("CGTGACAGTGTATGGGCATCTTT","TGT",1);
    cout << matches.size() << endl;



    //parts 2 and 3 
    //string pattern = argv[2];
    //int patternLength = pattern.length();
    //int mismatches = atoi(argv[3]);

    //gzFile fp = gzopen(argv[1],"r");
    //kseq_t* seq = kseq_init(fp);

    //int l;
    //if((l = kseq_read(seq)) >= 0 )
    //{
        //string genome = seq->seq.s;
        //int length = seq->seq.l;

        //const vector<int> matches = ApproxMatches(genome, pattern, mismatches);
        //vector<int>::const_iterator it;
        //for( it = matches.begin(); it != matches.end(); ++it )
        //{
            //cout << *it << " ";
        //}
        //cout << endl;
        //cout << matches.size() << endl;
    //}
    
    int kmerLength = atoi(argv[2]);
    int mismatches = atoi(argv[3]);

    gzFile fp = gzopen(argv[1],"r");
    kseq_t* seq = kseq_init(fp);

    int l;
    if((l = kseq_read(seq)) >= 0 )
    {
        string genome = seq->seq.s;
        int length = seq->seq.l;

        vector<int> counts;
        int maxCount = 0;
        int numPatterns = 1 << 2*kmerLength;
        for( int patternIx=0; patternIx < numPatterns; ++patternIx )
        {
            const vector<int> matches = ApproxMatches(genome, NumberToPattern(patternIx, kmerLength), mismatches);
            int count = matches.size();
            counts.push_back(count);
            //if(  count > maxCount )
            //{
            //maxCount = count;
            //}
        }

        vector<int> sumCounts;
        sumCounts.resize(numPatterns);
        int maxSumCount = 0;
        string revCompStr;
        revCompStr.resize(kmerLength);
        for( int patternIx=0; patternIx < numPatterns; ++patternIx )
        {
            ReverseComplement(NumberToPattern(patternIx, kmerLength),revCompStr);
            int revComp = PatternToNumber(revCompStr);
            int count = counts[patternIx] + counts[revComp];
            sumCounts[patternIx] = count;
            sumCounts[revComp] = count;
            if( count > maxSumCount )
            {
                maxSumCount = count;
            }
        }

        vector<int>::iterator it;
        int ix = 0;
        for( it = sumCounts.begin(); it != sumCounts.end(); ++it )
        {
            if( *it == maxSumCount )
            {
                cout << NumberToPattern(ix,kmerLength) << " ";
            }
            ++ix;
        }
    }
    return 0;
}
