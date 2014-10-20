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
        int numKmers = 1 << 2*k;
        vector<int> frequencies (numKmers);
        FillFrequencies(seq->seq.s, k, frequencies);
        vector<int>::iterator it;
        for( it = frequencies.begin(); it != frequencies.end(); ++it )
        {
            cout << *it << " ";
        }
        
    }
    //set<string>::iterator it;
    //for( it=globalClumps.begin(); it != globalClumps.end(); ++it )
    //{
    //cout << *it << " ";
    //}
    cout << endl << endl;
    //
    //kseq_destroy(seq);
    //gzclose(fp); 
    //}
}
