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

    int l;
    while((l = kseq_read(seq)) >= 0 )
    {
        string peptide = seq->seq.s;
        const vector<int> spectrum = GetSpectrum(peptide, true);

        vector<int>::const_iterator it;
        for(it = spectrum.begin(); it != spectrum.end(); ++it)
        {
            cout << *it << " ";
        }
        cout << endl;
    }


    kseq_destroy(seq);
    gzclose(fp);
    return 0;
}
