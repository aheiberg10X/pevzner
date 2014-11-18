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

void Expand(set< vector<int> >& peptides)
{
    set< vector<int> > old_peptides(peptides);
    peptides.clear();
    set< vector<int> >::iterator it;
    for( it=old_peptides.begin(); it != old_peptides.end(); ++it )
    {
        vector<int> peptide = *it;
        for( int acid=0; acid < 18; ++acid )
        {
            vector<int> added(peptide);
            added.push_back(AminoAcids::weights[acid]);
            peptides.insert(added);
        }
    }
}

bool ScoreCompare(const pair< vector<int> , int >& one, const pair< vector<int> , int >& two)
{
    return one.second > two.second;
}

void Trim( set< vector<int> >& peptides, map<int,int> weight_counts, int n )
{
    vector< pair< vector<int> , int > > scores;
    set< vector<int> >::iterator it;
    for(it = peptides.begin(); it != peptides.end(); ++it )
    {
        int score = Score2(*it, weight_counts, false);
        scores.push_back(pair< vector<int> ,int>(*it, score));
    }
    sort(scores.begin(), scores.end(), ScoreCompare);
    peptides.clear();

    vector< pair< vector<int> , int > >::iterator it2;
    int last_score = -1;
    for( it2=scores.begin(); it2!= scores.end(); ++it2)
    {

        pair< vector<int>,int> p = *it2;
        vector<int> peptide = p.first;
        int score = p.second;

        if( n <= 0 )
        { 
            if( score == last_score ) 
            {
                peptides.insert(peptide);
            }
            else
            {
                break;
            }
        }
        else
        {
            --n;
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

    set< vector<int> > peptides;
    peptides.insert( vector<int>(0) );
    vector< vector<int> > leader_peptides;
    leader_peptides.push_back( vector<int>(0) );
    set< vector<int> > final;
    int count = 0;

    int epoch = 0;
    while( peptides.size() > 0 )// && epoch++ < 2)
    {
        cout << "Epoch: " << epoch++ << endl;
        cout << "Parent mass: " << parent_mass << endl;
        Expand(peptides);

        set< vector<int> >::iterator it;
        for( it = peptides.begin(); it != peptides.end(); )
        {
            vector<int> peptide = *it;

            int peptide_mass = AminoAcids::Mass2(peptide);
            if( peptide_mass == parent_mass )
            {
                int peptide_score = Score2(peptide, cyclic_counts, true);
                //int peptide_cycle_score = Score2(peptide, cyclic_counts, true);
                if( peptide_score == 83 )
                {
                    final.insert(peptide);
                    //stringstream ss;
                    //string pep = *it;
                    //for( int i=0; i < pep.length()-1; ++i )
                    //{
                    //ss << AminoAcids::acid_weights.at(pep[i]) << "-";
                    //}
                    //ss << AminoAcids::acid_weights.at(pep[pep.length()-1]);
                    //count++;
                    //final.insert(ss.str());
                }

                int leader_score =  Score2(leader_peptides[0], cyclic_counts, true);
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
        Trim(peptides, cyclic_counts, n );
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

    set< vector<int> >::iterator it2;
    for( it2 = final.begin(); it2 != final.end(); ++it2)
    {
        vector<int> peptide = *it2;
        vector<int>::iterator it7;
        stringstream ss;
        for( it7 = peptide.begin(); it7 != peptide.end(); ++it7 )
        {
            ss << *it7 << "-";
        }
        string pep = ss.str();
        pep = pep.substr(0, pep.length()-1);
        cout << pep << " ";
    }
    cout << endl;
    cout << final.size() << endl;

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
