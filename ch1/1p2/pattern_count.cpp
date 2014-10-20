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

    bool doPatternSearch = false;

    int l;
    if((l = kseq_read(seq)) >= 0 )
    {
        if( doPatternSearch )
        {
            printf("name: %s\n", seq->name.s);
            printf("seq: %s\n", seq->seq.s);
            int count = PatternSearch(seq->seq.s, seq->seq.l, argv[2], atoi(argv[3]));
            printf("Count: %d\n", count);
        }
        else
        {
           set<string> kmers = FrequentWordsFast(seq->seq.s, seq->seq.l, atoi(argv[2]), -1); 
           set<string>::iterator it;
           for( it=kmers.begin(); it != kmers.end(); ++it)
           {
               cout << *it << endl;
           }
        }
    }


    kseq_destroy(seq);
    gzclose(fp);
    return 0;
}
