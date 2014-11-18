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

using namespace std;

KSEQ_INIT(gzFile, gzread)

int main(int argc, char* argv[])
{
    cout << "hello!" << endl;
    gzFile fp = gzopen(argv[1],"r");
    kseq_t* seq = kseq_init(fp);

    int l;
    while((l = kseq_read(seq)) >= 0 )
    {
        string genome = seq->seq.s;
        for(int i=0; i<seq->seq.l-3+1; i += 3)
        {
            string codon = genome.substr(i,3);
            cout << AminoAcids::codon2acid.at(codon);
        }
        cout << endl;
    }


    kseq_destroy(seq);
    gzclose(fp);
    return 0;
}
