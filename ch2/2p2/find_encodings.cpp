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
    gzFile fp = gzopen(argv[1],"r");
    kseq_t* seq = kseq_init(fp);

    string peptide = argv[2];
    int peptideLength = peptide.length();
    int peptideCodonLength = peptideLength * 3;

    string candidate, reverse;
    candidate.resize(peptideCodonLength);
    reverse.resize(peptideCodonLength);

    int l;
    if((l = kseq_read(seq)) >= 0 )
    {
        string genome = seq->seq.s;
        for(int i=0; i<seq->seq.l-peptideCodonLength+1; ++i)
        {
            bool match = true;
            candidate = genome.substr(i,peptideCodonLength);
            ReverseComplement(candidate, reverse);
            cout << "candidate: " << candidate << endl;
            //cout << "reverse  : " << reverse << endl;
            //if( candidate == "ATAATAATGCGGCTCGAATACTTCCTT" )
            //cout << "hello" << endl;

            for( int j=0; j < peptideLength-1; ++j )
            {
                string codon = candidate.substr(3*j,3);
                if( AminoAcids::codon2acid.at(codon) != peptide[j] )
                {
                    match = false;
                    break;
                }
            }
            if( match )
            {
                cout << candidate << endl;
            }

            match = true;
            for( int j=0; j < peptideLength-1; ++j )
            {
                string codon = reverse.substr(3*j,3);
                if( AminoAcids::codon2acid.at(codon) != peptide[j] )
                {
                    //if( candidate == "ATAATAATGCGGCTCGAATACTTCCTT" )
                    //cout << "match fail at: " << codon << " encoding " << AminoAcids::codon2acid.at(codon) << endl;
                    match = false;
                    break;
                }
            }
            if( match )
            {
                cout << candidate << endl;
            }
        }
        cout << endl;
    }


    kseq_destroy(seq);
    gzclose(fp);
    return 0;
}
