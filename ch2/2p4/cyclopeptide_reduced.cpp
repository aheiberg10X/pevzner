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

void Expand(set< vector<int> >& peptides, vector<int>& possibilities )
{
    set< vector<int> > old_peptides(peptides);
    peptides.clear();
    set< vector<int> >::iterator it;
    for( it=old_peptides.begin(); it != old_peptides.end(); ++it )
    {
        vector<int> peptide = *it;
        vector<int>::iterator it2;
        for( it2 = possibilities.begin(); it2 != possibilities.end(); ++it2 )
        {
            vector<int> added(peptide);
            added.push_back(*it2);
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

bool FrequencyCompare( const pair<int,int>& one, const pair<int,int>& two )
{
    return one.second > two.second;
}
vector<int> TopWeights( const map<int,int>& convolution, int m )
{
    vector< pair<int,int> > keys;
    map<int,int>::const_iterator it;
    for( it=convolution.begin(); it!=convolution.end(); ++it )
    {
        keys.push_back(*it);
    }
    sort(keys.begin(), keys.end(), FrequencyCompare);

    vector<int> top;

    int last_freq = -1;
    vector< pair<int,int> >::iterator it2;
    for(it2 = keys.begin(); it2 != keys.end(); ++it2)
    {
        int weight = (*it2).first;
        int freq = (*it2).second;
 
        if( (m > 0 || last_freq == freq) )           
        {
            if( 57 <= weight && weight <= 200 )
            {
                cout << "weight : " << weight << " freq: " << freq << " m: " << m << endl;
                m--;
                top.push_back(weight);
                last_freq = freq;
                exit;
            } 
        } 
        else break;
    }
    return top;
}

int main(int argc, char* argv[])
{
    //string cyclic_spectrum = "0 113 128 186 241 299 314 427";
    string input_spectrum = "0 57 118 179 236 240 301";
    int n = 1000;
    int m = 20;

    istringstream iss(input_spectrum);
    string token;
    map<int,int> cyclic_counts;
    vector<int> expt_spectrum;
    int parent_mass = 0;
    while( getline(iss,token,' ') )
    {
        int weight = atoi(token.c_str());
        expt_spectrum.push_back(weight);
        if( cyclic_counts.find(weight) != cyclic_counts.end() )
        {
            cyclic_counts[weight]++;
        }
        else
        {
            cyclic_counts.insert( pair<int,int>(weight,1) );
        }
        if( weight > parent_mass )
            parent_mass = weight;
    }
    sort(expt_spectrum.begin(), expt_spectrum.end());

    const map<int,int> convolution = SpectralConvolution(expt_spectrum);
    vector<int> top_weights = TopWeights(convolution, m);

    set< vector<int> > peptides;
    peptides.insert( vector<int>(0) );
    int leader_score = 0;
    set< vector<int> > leader_peptides;
    leader_peptides.insert( vector<int>(0) );
    set< vector<int> > final;
    int count = 0;

    int epoch = 0;
    while( peptides.size() > 0 )// && epoch++ < 2)
    {
        cout << "Epoch: " << epoch++ << endl;
        cout << "Parent mass: " << parent_mass << endl;
        Expand(peptides, top_weights);

        set< vector<int> >::iterator it;
        for( it = peptides.begin(); it != peptides.end(); )
        {
            vector<int> peptide = *it;

            int peptide_mass = AminoAcids::Mass2(peptide);
            if( peptide_mass == parent_mass )
            {
                int peptide_score = Score2(peptide, cyclic_counts, true);
                //if( peptide_score == 83 )
                //{
                //final.insert(peptide);
                //}

                //int leader_score =  Score2(leader_peptides[0], cyclic_counts, true);
                if( peptide_score > leader_score )
                {
                    leader_peptides.clear();
                    leader_peptides.insert(peptide);
                    leader_score = peptide_score;
                    //cout << peptide_score << endl;
                }
                if( peptide_score == leader_score )
                {
                    leader_peptides.insert(peptide);
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

    set< vector<int> >::iterator it2;
    for( it2 = leader_peptides.begin(); it2 != leader_peptides.end(); ++it2)
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
    cout << leader_peptides.size() << endl;

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
