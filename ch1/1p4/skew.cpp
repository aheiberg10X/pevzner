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

    string a = "TGACCCGTTATGCTCGAGTTCGGTCAGAGCGTCATTGCGAGTAGTCGTTTGCTTTCTCAAACTCC";
    string b = "GAGCGATTAAGCGTGACAGCCCCAGGGAACCCACAAAACGTGATCGCAGTCCATCCGATCATACA";
    cout << HammingDistance(a,b) << endl;
    
    vector<int> skews;
    PopulateSkew("CATTCCAGTACTTCATGATGGCGTGAAGA",29,skews,false);

    int l;
    if((l = kseq_read(seq)) >= 0 )
    {
        string genome = seq->seq.s;
        int length = seq->seq.l;
        vector<int> skews;
        int min = PopulateSkew(genome, length, skews, true);
        cout << "min: " << min << endl;
        for( int i=0; i<length; ++i)
        {
            if(skews[i] == min)
            {
                cout << i << " ";
            }
        }
        cout << endl;
    }
    return 0;
}
