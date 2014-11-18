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
    vector< vector<float> > profile;
    vector<float> one;
    one.push_back(.2);
    one.push_back(.1);
    one.push_back(0);
    one.push_back(.7);

    vector<float> two;
    two.push_back(.2);
    two.push_back(.6);
    two.push_back(0);
    two.push_back(.2);
    
    vector<float> three;
    three.push_back(0);
    three.push_back(0);
    three.push_back(1);
    three.push_back(0);

    vector<float> four;
    four.push_back(0);
    four.push_back(0);
    four.push_back(1);
    four.push_back(0);

    vector<float> five;
    five.push_back(0);
    five.push_back(0);
    five.push_back(.9);
    five.push_back(.1);

    vector<float> six;
    six.push_back(0);
    six.push_back(0);
    six.push_back(.9);
    six.push_back(.1);

    vector<float> seven;
    seven.push_back(.9);
    seven.push_back(0);
    seven.push_back(.1);
    seven.push_back(0);

    vector<float> eight;
    eight.push_back(.1);
    eight.push_back(.4);
    eight.push_back(0);
    eight.push_back(.5);

    vector<float> nine;
    nine.push_back(.1);
    nine.push_back(.1);
    nine.push_back(0);
    nine.push_back(.8);

    vector<float> ten;
    ten.push_back(.1);
    ten.push_back(.2);
    ten.push_back(0);
    ten.push_back(.7);

    vector<float> eleven;
    eleven.push_back(.3);
    eleven.push_back(.4);
    eleven.push_back(0);
    eleven.push_back(.3);

    vector<float> twelve;
    twelve.push_back(0);
    twelve.push_back(.6);
    twelve.push_back(0);
    twelve.push_back(.4);

    profile.push_back(one);
    profile.push_back(two);
    profile.push_back(three);
    profile.push_back(four);
    profile.push_back(five);
    profile.push_back(six);
    profile.push_back(seven);
    profile.push_back(eight);
    profile.push_back(nine);
    profile.push_back(ten);
    profile.push_back(eleven);
    profile.push_back(twelve);

    float entropy = Entropy(profile);
    cout << entropy << endl;
    return 0;

    //const set<string> neighbs = Neighbors("CCCAGAAA",3);
    //set<string>::const_iterator it;
    //for( it = neighbs.begin(); it != neighbs.end(); ++it )
    //{
    //cout << *it << endl;
    //}
    //return 0;

    gzFile fp = gzopen(argv[1],"r");
    kseq_t* seq = kseq_init(fp);

    int k = atoi(argv[2]);
    int d = atoi(argv[3]);

    set<string> kmers;
    vector<string> strings;
    int l;
    while ((l = kseq_read(seq)) >= 0 )
    {
        string dnastring = seq->seq.s;
        cout << dnastring << endl;
        strings.push_back(dnastring);
        for( int i=0; i < seq->seq.l - k + 1; ++i )
        {
            string kmer = dnastring.substr(i,k);
            if( kmers.find(kmer) == kmers.end() )
            {
                kmers.insert(kmer);
            }
        }
    }

    kseq_destroy(seq);
    gzclose(fp);

    /////////////////////////////////////////////////
    /////////////////////////////////////////////////
    set<string> patterns;
    set<string>::iterator it;
    for( it = kmers.begin(); it != kmers.end(); ++it )
    {
        //cout << "kmer : " << *it << endl;
        const set<string> neighbs = Neighbors(*it,d);
        set<string>::const_iterator itn;
        vector<string>::iterator its;
        for( itn = neighbs.begin(); itn != neighbs.end(); ++itn )
        {
            //cout << "    neighb: " << *itn << endl;
            bool allMatch = true;
            for( its = strings.begin(); its != strings.end(); ++its )
            {
                //cout << "        string: " << *its << endl;
                bool match = false;
                int len = (*its).length();
                for( int i=0; i < len-k+1; ++i )
                {
                    string sub = (*its).substr(i,k);
                    if( HammingDistance(sub, *itn) <= d )
                    {
                        //cout << "        match at: " << i <<  endl;
                        match = true;
                        break;
                    }
                }
                if( !match )
                {
                    allMatch = false;
                    break;
                }
            }
            if( allMatch )
            {
                //cout << "        all match" << endl;
                patterns.insert(*itn);
            }
        }
    }
    
    set<string>::iterator itp;
    for( itp = patterns.begin(); itp != patterns.end(); ++itp )
    {
        cout << *itp << endl;
    }

    return 0;
}
