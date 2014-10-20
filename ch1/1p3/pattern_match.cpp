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

    int l;
    if((l = kseq_read(seq)) >= 0 )
    {
        string genome = seq->seq.s;
        string kmer = argv[2];
        vector<int> occurrences = PatternMatch(genome, kmer);
        vector<int>::iterator it;
        for( it = occurrences.begin(); it != occurrences.end(); ++it )
        {
            cout << *it << " ";
        }
        cout << endl << endl;
        kseq_destroy(seq);
        gzclose(fp); 
    }

}
