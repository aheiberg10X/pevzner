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
#include <numeric>

using namespace std;

void Expand(set<string>& peptides)
{
    set<string> old_peptides(peptides);
    peptides.clear();
    set<string>::iterator it;
    for( it=old_peptides.begin(); it != old_peptides.end(); ++it )
    {
        string peptide = *it;
        for( int acid=0; acid < 20; ++acid )
        {
            peptides.insert(peptide+AminoAcids::acids[acid]);
        }
    }
}


int main(int argc, char* argv[])
{
    //string cyclic_spectrum = "0 113 128 186 241 299 314 427";
    string cyclic_spectrum = "0 71 99 101 103 128 129 199 200 204 227 230 231 298 303 328 330 332 333";

    istringstream iss(cyclic_spectrum);
    string token;
    //char* pch = strtok( cyclic_spectrum.c_str(), " " );
    map<int,int> cyclic_counts;
    int parent_mass = 0;
    while( getline(iss,token,' ') )
    {
        int weight = atoi(token.c_str());
        if( cyclic_counts.find(weight) != cyclic_counts.end() )
        {
            cyclic_counts[weight]++;
        }
        else
        {
            cyclic_counts.insert( pair<int,int>(weight,1) );
        }
        parent_mass = weight;
    }

    set<string> peptides;
    peptides.insert("");

    int epoch = 0;
    set<string> matches;
    while( peptides.size() > 0 && epoch++ < 3)
    {
        cout << "Epoch: " << epoch << endl;
        Expand(peptides);

        set<string>::iterator it;
        for( it = peptides.begin(); it != peptides.end(); )
        {
            string peptide = *it;

            if( AminoAcids::Mass(peptide) == parent_mass )
            {
                vector<int> v = GetSpectrum(peptide,true);
                vector<int>::iterator it2;
                stringstream ss;
                for( it2 = v.begin(); it2 != v.end(); ++it2 )
                {
                    ss << *it2 << " ";
                }    
                string spectrum = ss.str(); 
                //cout << "|" + spectrum << "| |" << cyclic_spectrum+" |" << endl;
                if( spectrum == cyclic_spectrum+" " )
                {
                    matches.insert(peptide);
                }
                peptides.erase(it++);
            }
            else
            {
                
                bool consistent = true;
                const vector<int> linear_spectrum = GetSpectrum(peptide,false);
                map<int,int> cyclic_counts_copy(cyclic_counts);
                vector<int>::const_iterator it3;
                for(it3=linear_spectrum.begin(); it3 != linear_spectrum.end(); ++it3 )
                {
                    int weight = *it3;
                    //cout << "        " << weight << endl;
                    if( cyclic_counts_copy.find(weight) == cyclic_counts_copy.end() )
                    {
                        if( peptide == "ETC" || peptide == "TCE" || peptide == "CTQ" || peptide == "CET" ||
                            peptide == "TVQ" || peptide == "TCQ" )
                        {
                            cout << "   bad peptide: " << peptide << endl;
                        }
                        consistent = false;
                        break;
                    }
                    else
                    {
                        int count = cyclic_counts_copy[weight];
                        if( count == 0 )
                        {
                            if( peptide == "ETC" || peptide == "TCE" || peptide == "CTQ" || peptide == "CET" ||
                                peptide == "TVQ" || peptide == "TCQ" )
                            {
                                cout << "   bad peptide: " << peptide << endl;
                            }
                            consistent = false;
                            break;
                        }
                        cyclic_counts_copy[weight]--;
                    }
                }

                if( !consistent )
                {
                    //cout << peptide << " not consistent!" << endl;
                    peptides.erase(it++);
                }
                else
                {
                    cout << "    peptide: " << peptide << endl;
                    ++it;
                }
            }

        }
        //set<string>::iterator it5;
        //for( it5 = peptides.begin(); it5 != peptides.end(); ++it5)
        //{
        //cout << *it5 << " ";
        //}
        //cout << "press any key..." << endl;
        //string derp;
        //cin >> derp;
    }
    
    cout << "======================================= " << endl;

    set<string> final;

    set<string>::iterator it;
    for( it = matches.begin(); it != matches.end(); ++it)
    {
        stringstream ss;
        string peptide = *it;
        for( int i=0; i < peptide.length()-1; ++i )
        {
            ss << AminoAcids::acid_weights.at(peptide[i]) << "-";
        }
        ss << AminoAcids::acid_weights.at(peptide[peptide.length()-1]);

        final.insert( ss.str() );

        //cout << *it << " ";
    }

    
    for( it = final.begin(); it != final.end(); ++it)
    {
        cout << *it << endl;
    }

    cout << endl;

    return 0;
}
