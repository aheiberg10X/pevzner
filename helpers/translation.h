#ifndef TRANSLATION
#define TRANSLATION

#include <map>
#include <string>
#include <vector>

using namespace std;

struct AminoAcids
{ 
    static const char acids[];
    static const int weights[];

    static int Mass(string peptide);
    static int Mass2(vector<int> peptide);

    static const map<char,int> acid_weights;
    static map<char,int> create_acid_weight_map()
    {
        map<char,int> m;
        m['G'] = 57;
        m['A'] = 71;
        m['S'] = 87;
        m['P'] = 97;
        m['V'] = 99;
        m['T'] = 101;
        m['C'] = 103;
        m['I'] = 113;
        m['L'] = 113;
        m['N'] = 114;
        m['D'] = 115;
        m['K'] = 128;
        m['Q'] = 128;
        m['E'] = 129;
        m['M'] = 131;
        m['H'] = 137;
        m['F'] = 147;
        m['R'] = 156;
        m['Y'] = 163;
        m['W'] = 186;
        return m;
    }

    static const map<string,char> codon2acid;
    static map<string,char> create_map()
    {
        map<string,char> m;
        m["AAA"] = 'K';
        m["AAC"] = 'N';
        m["AAA"] = 'K';
        m["AAC"] = 'N';
        m["AAG"] = 'K';
        m["AAU"] = 'N';
        m["ACA"] = 'T';
        m["ACC"] = 'T';
        m["ACG"] = 'T';
        m["ACU"] = 'T';
        m["AGA"] = 'R';
        m["AGC"] = 'S';
        m["AGG"] = 'R';
        m["AGU"] = 'S';
        m["AUA"] = 'I';
        m["AUC"] = 'I';
        m["AUG"] = 'M';
        m["AUU"] = 'I';
        m["CAA"] = 'Q';
        m["CAC"] = 'H';
        m["CAG"] = 'Q';
        m["CAU"] = 'H';
        m["CCA"] = 'P';
        m["CCC"] = 'P';
        m["CCG"] = 'P';
        m["CCU"] = 'P';
        m["CGA"] = 'R';
        m["CGC"] = 'R';
        m["CGG"] = 'R';
        m["CGU"] = 'R';
        m["CUA"] = 'L';
        m["CUC"] = 'L';
        m["CUG"] = 'L';
        m["CUU"] = 'L';
        m["GAA"] = 'E';
        m["GAC"] = 'D';
        m["GAG"] = 'E';
        m["GAU"] = 'D';
        m["GCA"] = 'A';
        m["GCC"] = 'A';
        m["GCG"] = 'A';
        m["GCU"] = 'A';
        m["GGA"] = 'G';
        m["GGC"] = 'G';
        m["GGG"] = 'G';
        m["GGU"] = 'G';
        m["GUA"] = 'V';
        m["GUC"] = 'V';
        m["GUG"] = 'V';
        m["GUU"] = 'V';
        m["UAA"] = ' ';
        m["UAC"] = 'Y';
        m["UAG"] = ' ';
        m["UAU"] = 'Y';
        m["UCA"] = 'S';
        m["UCC"] = 'S';
        m["UCG"] = 'S';
        m["UCU"] = 'S';
        m["UGA"] = ' ';
        m["UGC"] = 'C';
        m["UGG"] = 'W';
        m["UGU"] = 'C';
        m["UUA"] = 'L';
        m["UUC"] = 'F';
        m["UUG"] = 'L';
        m["UUU"] = 'F';

        m["AAA"] = 'K';
        m["AAC"] = 'N';
        m["AAA"] = 'K';
        m["AAC"] = 'N';
        m["AAG"] = 'K';
        m["AAT"] = 'N';
        m["ACA"] = 'T';
        m["ACC"] = 'T';
        m["ACG"] = 'T';
        m["ACT"] = 'T';
        m["AGA"] = 'R';
        m["AGC"] = 'S';
        m["AGG"] = 'R';
        m["AGT"] = 'S';
        m["ATA"] = 'I';
        m["ATC"] = 'I';
        m["ATG"] = 'M';
        m["ATT"] = 'I';
        m["CAA"] = 'Q';
        m["CAC"] = 'H';
        m["CAG"] = 'Q';
        m["CAT"] = 'H';
        m["CCA"] = 'P';
        m["CCC"] = 'P';
        m["CCG"] = 'P';
        m["CCT"] = 'P';
        m["CGA"] = 'R';
        m["CGC"] = 'R';
        m["CGG"] = 'R';
        m["CGT"] = 'R';
        m["CTA"] = 'L';
        m["CTC"] = 'L';
        m["CTG"] = 'L';
        m["CTT"] = 'L';
        m["GAA"] = 'E';
        m["GAC"] = 'D';
        m["GAG"] = 'E';
        m["GAT"] = 'D';
        m["GCA"] = 'A';
        m["GCC"] = 'A';
        m["GCG"] = 'A';
        m["GCT"] = 'A';
        m["GGA"] = 'G';
        m["GGC"] = 'G';
        m["GGG"] = 'G';
        m["GGT"] = 'G';
        m["GTA"] = 'V';
        m["GTC"] = 'V';
        m["GTG"] = 'V';
        m["GTT"] = 'V';
        m["TAA"] = ' ';
        m["TAC"] = 'Y';
        m["TAG"] = ' ';
        m["TAT"] = 'Y';
        m["TCA"] = 'S';
        m["TCC"] = 'S';
        m["TCG"] = 'S';
        m["TCT"] = 'S';
        m["TGA"] = ' ';
        m["TGC"] = 'C';
        m["TGG"] = 'W';
        m["TGT"] = 'C';
        m["TTA"] = 'L';
        m["TTC"] = 'F';
        m["TTG"] = 'L';
        m["TTT"] = 'F';
 

        return m;
    }
};

#endif
