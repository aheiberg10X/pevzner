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
#include <map>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

int main(int argc, char* argv[])
{
    //string cyclic_spectrum = "0 113 128 186 241 299 314 427";
    string experimental_spectrum =  "316 579 288 103 636 131 795 0 277 735 852 579 319 390 394 259 710 575 522 447 536 476 272 163 419 563 895 348 128 607 952 291 824 1026 666 416 520 185 898 792 232 504 980 231 648 693 895 57 982 158 969 289 71 955 898 504 114 435 1012 852 101 707 185 724 806 376 851 1083 87 667 955 561 128 764 508 811 767 920 794 996 188 405 417 925 547 689 231 359 678 664 188 373";

    vector<int> spectrum;
    istringstream ss(experimental_spectrum);
    string token;
    while( getline(ss,token,' ') )
    {
        spectrum.push_back(atoi(token.c_str()));
    }
    sort(spectrum.begin(), spectrum.end());

    const map<int,int> convolution = SpectralConvolution(spectrum);
    cout << "convol size: " << convolution.size() << endl;
    map<int,int>::const_iterator it;
    for( it=convolution.begin(); it != convolution.end(); ++it )
    {
        pair<int,int> p = *it;
        for( int i=0; i<p.second; ++i )
        {
            cout << p.first << " ";
        }
    }
    cout << endl;



    return 0;
}
