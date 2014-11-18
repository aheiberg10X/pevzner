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
            string pep = peptide+AminoAcids::acids[acid];
            peptides.insert(pep);
        }
    }
}

bool ScoreCompare(const pair<string,int>& one, const pair<string,int>& two)
{
    return one.second > two.second;
}

void Trim( set<string>& peptides, map<int,int> weight_counts, int n )
{
    vector< pair<string,int> > scores;
    set<string>::iterator it;
    for(it = peptides.begin(); it != peptides.end(); ++it )
    {
        int score = Score(*it, weight_counts, false);
        scores.push_back(pair<string,int>(*it, score));
    }
    sort(scores.begin(), scores.end(), ScoreCompare);
    peptides.clear();

    vector< pair<string,int> >::iterator it2;
    int last_score = -1;
    for( it2=scores.begin(); it2!= scores.end(); ++it2)
    {

        pair<string,int> p = *it2;
        string peptide = p.first;
        int score = p.second;

        if( n <= 0 ) { 
            if( score == last_score ) 
            {
                peptides.insert(peptide);
            }
            break;
        }
        else{
            if( score != last_score ) --n;
            peptides.insert(peptide);
        }

        last_score = score;
    }
}


int main(int argc, char* argv[])
{
    //string cyclic_spectrum = "0 113 128 186 241 299 314 427";
    string cyclic_spectrum = "0 97 99 113 114 115 128 128 147 147 163 186 227 241 242 244 244 256 260 261 262 283 291 309 330 333 340 347 385 388 389 390 390 405 435 447 485 487 503 504 518 544 552 575 577 584 599 608 631 632 650 651 653 672 690 691 717 738 745 770 779 804 818 819 827 835 837 875 892 892 917 932 932 933 934 965 982 989 1039 1060 1062 1078 1080 1081 1095 1136 1159 1175 1175 1194 1194 1208 1209 1223 1322";
    int n = 1000;

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
    vector<string> leader_peptides;
    leader_peptides.push_back("");
    set<string> final;
    int count = 0;

    int epoch = 0;
    set<string> matches;
    while( peptides.size() > 0 )// && epoch++ < 2)
    {
        //cout << "Epoch: " << epoch << endl;
        Expand(peptides);

        set<string>::iterator it;
        for( it = peptides.begin(); it != peptides.end(); )
        {
            string peptide = *it;

            int peptide_mass = AminoAcids::Mass(peptide);
            if( peptide_mass == parent_mass )
            {
                int peptide_score = Score(peptide, cyclic_counts, true);
                int peptide_cycle_score = Score(peptide, cyclic_counts, true);
                if( peptide_cycle_score == 83 )
                {
                    stringstream ss;
                    string pep = *it;
                    for( int i=0; i < pep.length()-1; ++i )
                    {
                        ss << AminoAcids::acid_weights.at(pep[i]) << "-";
                    }
                    ss << AminoAcids::acid_weights.at(pep[pep.length()-1]);
                    count++;
                    final.insert(ss.str());
                }

                int leader_score =  Score(leader_peptides[0], cyclic_counts, true);
                if( peptide_score > leader_score )
                {
                    leader_peptides.clear();
                    leader_peptides.push_back(peptide);
                    cout << peptide_score << endl;
                }
                if( peptide_score == leader_score )
                {
                    leader_peptides.push_back(peptide);
                }
                it++;
            }
            else if( peptide_mass > parent_mass )
            {
                peptides.erase(it++);
            }
            else
            {
                it++;
            }
        }
        Trim(peptides, cyclic_counts, 2*n );
    }
    
    cout << "======================================= " << endl;

    /*
    vector<string>::iterator it;
    //set<string>::iterator it;
    int count = 0;
    for( it = leader_peptides.begin(); it != leader_peptides.end(); ++it)
        //for( it = peptides.begin(); it != peptides.end(); ++it)
    {
        stringstream ss;
        string peptide = *it;
        int score = Score(peptide, cyclic_counts, true);
        cout << peptide << " " << score << endl;
        for( int i=0; i < peptide.length()-1; ++i )
        {
            ss << AminoAcids::acid_weights.at(peptide[i]) << "-";
        }
        ss << AminoAcids::acid_weights.at(peptide[peptide.length()-1]);
        //if( score == 83 )
        //{
        count++;
        final.insert(ss.str());
        //}
    }
    cout << count << " ";
    */

    set<string>::iterator it2;
    for( it2 = final.begin(); it2 != final.end(); ++it2)
    {
        cout << *it2 << endl;
    }

    //stringstream ss;
    //for( int i=0; i < leader_peptides.length()-1; ++i )
    //{
    //ss << AminoAcids::acid_weights.at(leader_peptides[i]) << "-";
    //}
    //ss << AminoAcids::acid_weights.at(leader_peptides[leader_peptides.length()-1]);
    //
    //cout << ss.str() << endl;


    cout << endl;

    return 0;
}
