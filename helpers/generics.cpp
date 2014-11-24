#include "generics.h"
#include "translation.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <math.h>

void ReverseComplement(string input, string& output)
{
    int length = input.length();
    if( length != output.length() )
    {
       cout << "input and output length aren't equal" << endl;
       throw 1;
    }

    for( int i=0; i<length; ++i)
    {
        char base = input[i];
        int j = length-1-i;
        switch(base)
        {
            case 'A' : 
                output[j] = 'T';
                break;
            case 'C' :
                output[j] = 'G';
                break;
            case 'G' :
                output[j] = 'C';
                break;
            case 'T' :
                output[j] = 'A';
                break;
            default :
                cout << "base: " << output[j] << " is not one of A,C,G,T" << endl;
                throw 1;       
        }
    }   
}

vector<int> PatternMatch(string text, string substring)
{
    vector<int> matches;

    int length = text.length();
    int sublength = substring.length();
    for( int i=0; i<length-sublength+1; ++i )
    {
        bool match = true;
        for( int j=0; j<sublength; ++j )
        {
            if( text[i+j] != substring[j] )
            {
                match = false;
                break;
            }
        }
        if( match )
        {
            matches.push_back(i);
        }
    }
    return matches;
}

int PatternSearch(string text, int length, string pattern, int pattern_len)
{
    int num_matches = 0;
    for( int i=0; i<length-pattern_len+1; ++i )
    {
        bool match = true;
        for( int j=0; j<pattern_len; ++j )
        {
            if( text[i+j] != pattern[j] )
            {
                match = false;
                break;
            }
        }
        if( match )
            ++num_matches;
    }
    return num_matches;
}

set<string> FrequentWords( string text, int length, int k, int thresh )
{
    int* count = new int[length];
    
    int max_count = 0;
    for( int i=0; i<length-k+1; ++i)
    {
        string pattern = text.substr(i,k);
        count[i] = PatternSearch(text, length, pattern, k);
        if( count[i] > max_count )
        {
            max_count = count[i];
        }   
    }   
    
    if( thresh < 0 )
        thresh = max_count;
    
    set<string> frequentWords;
    for( int i=0; i<length-k+1; ++i )
    {   
        if( count[i] == thresh )
        {   
            frequentWords.insert( text.substr(i,k) );
        }
    }
    
    delete[] count;
    return frequentWords;
}

set<string> FrequentWordsFast( string text, int length, int k, int thresh )
{
    set<string> frequentWords;

    int numKmers = 1 << 2*k;
    vector<int> frequencies (numKmers);
    int maxCount = FillFrequencies(text, k, frequencies);

    for( int i=0; i < numKmers; ++i )
    {
        if( frequencies[i] == maxCount )
        {
            frequentWords.insert(NumberToPattern(i,k));
        }
    }
    return frequentWords; 
}

int PatternToNumber(string pattern)
{
    int length = pattern.length();
    int sum = 0;
    int multiplier = 1;
    const int base = 4;
    
    for(int i=length-1; i>=0; --i)
    {
        sum += multiplier * BaseToNumber(pattern[i]);
        multiplier *= base;
    }
    return sum;
}

int BaseToNumber(char b)
{
    switch( b )
    {
        case 'A' :
            return 0; 
        case 'C' :
            return 1;
        case 'G' :
            return 2;
        case 'T' :
            return 3;
        default :
            cout << "base is not ACGT" << endl;
            throw 1;
    }
}

char NumberToBase(int i)
{
    switch( i )
    {
        case 0 :
            return 'A';
        case 1 :
            return 'C';
        case 2 :
            return 'G';
        case 3 :
            return 'T';
        default :
            cout << "number not 0123" << endl;
            throw 1;
    }
}

string NumberToPattern(int number, int length)
{
    string output;
    output.resize(length);

    int digit = length-1;
    while(true)
    {
        int quotient = number / 4;
        int remainder = number % 4;
        output[digit--] = NumberToBase(remainder);
        if( quotient < 4 )
        {
            output[digit--] = NumberToBase(quotient);
            break;
        }
        else
        {
            number = quotient;
        }
    }
    while(digit >= 0)
    {
        output[digit--] = 'A';
    }
    return output;
}

int FillFrequencies(string text, int k, vector<int>& frequencies)
{
    int maxCount = 0;
    fill(frequencies.begin(), frequencies.end(), 0);
    int length = text.length();
    for( int i=0; i < length-k+1; ++i )
    {
        string substr = text.substr(i,k);
        int number = PatternToNumber(substr);
        if( ++frequencies[number] > maxCount )
            maxCount = frequencies[number];

    }
    return maxCount;
}

int PopulateSkew(string sequence, int length, vector<int>& skews, bool min)
{
    skews.push_back(0);
    int count = 0;
    int minCount = 9999;
    if( min )
    {
        for(int i=0; i<length; ++i)
        {
            //cout << sequence[i] << " ";
            switch(sequence[i])
            {
                case 'A' :
                    break;
                case 'T':
                    break;
                case 'G':
                    ++count;
                    break;
                case 'C' :
                    --count;
                    break;
                default :
                    cout << "PopulateSkew(): not ACTG" << endl;
                    throw 1;
            }
            skews.push_back(count);
            if( count < minCount )
            {
                minCount = count;
            }
        }
        return minCount;
    }
    else
    {
        cout  << "ELLLEOE?" << endl;
        int maxCount = -9999;
        for(int i=0; i<length; ++i)
        {
            switch(sequence[i])
            {
                case 'A' :
                    break;
                case 'T':
                    break;
                case 'G':
                    ++count;
                    break;
                case 'C' :
                    --count;
                    break;
            }
            //cout << i << " " << count << endl;
            if( count > maxCount )
            {
                maxCount = count;
                cout << i << endl;
            }
            skews.push_back(count);
        }
        return maxCount;
    }
}

int HammingDistance(string a, string b)
{
    if( a.length() != b.length())
    {
        cout << a << " | " << b << endl;
        cout << "Hamming distance input lengths don't match" << endl;
        throw 1;
    }
    int distance = 0;
    for( int i=0; i<a.length(); ++i)
    {
        if(a[i] != b[i])
            ++distance;
    }
    return distance;
}

const vector<int> ApproxMatches(string genome, string pattern, int mismatches)
{
    vector<int> locations;
    int patternLength = pattern.length();
    int length = genome.length();
    for( int i=0; i < length-patternLength+1; ++i )
    {
        string ref = genome.substr(i,patternLength);
        int dist = HammingDistance(ref, pattern);
        if( dist <= mismatches )
        {
            locations.push_back(i);
        }
    }
    return locations;
}

const vector<int> GetSpectrum2(const vector<int>& peptide, bool cyclic)
{
    vector<int> prefix_mass;
    prefix_mass.push_back(0);

    vector<int>::const_iterator it;
    int i = 0;
    for( it=peptide.begin(); it != peptide.end(); ++it )
    {
        prefix_mass.push_back( prefix_mass[i++] + *it );
    }

    int peptide_length = peptide.size();
    int peptide_mass = prefix_mass[peptide_length];
    
    vector<int> spectrum;
    int len = prefix_mass.size(); //peptide.length+1
    for( int i=0; i<len-1; ++i )
    {
        for( int j=i+1; j < len; ++j)
        {
            int diff = prefix_mass[j] - prefix_mass[i];
            spectrum.push_back(diff);
            if( cyclic && i > 0 && j < len-1 )
            {
                spectrum.push_back( peptide_mass - diff );
            }
        }
    }
   
    spectrum.push_back(0); 
    std::sort(spectrum.begin(), spectrum.end());
    return spectrum;
}

const vector<int> GetSpectrum(string peptide, bool cyclic)
{
    vector<int> prefix_mass;
    prefix_mass.push_back(0);
    
    for(int i=0; i < peptide.length(); ++i)
    {
        char aa = peptide[i];
        int mass = AminoAcids::acid_weights.at(aa);
        int cum_mass = prefix_mass[i] + mass;
        prefix_mass.push_back( cum_mass );
    }

    int peptide_mass = prefix_mass[peptide.length()];
    
    vector<int> spectrum;
    int len = prefix_mass.size(); //peptide.length+1
    for( int i=0; i<len-1; ++i )
    {
        for( int j=i+1; j < len; ++j)
        {
            int diff = prefix_mass[j] - prefix_mass[i];
            spectrum.push_back(diff);
            if( cyclic && i > 0 && j < len-1 )
            {
                spectrum.push_back( peptide_mass - diff );
            }
        }
    }
   
    spectrum.push_back(0); 
    std::sort(spectrum.begin(), spectrum.end());
    return spectrum;
}

int Score(string peptide, map<int,int> weight_counts, bool cyclic )
{
    map<int,int> counts(weight_counts);
    int score = 0;
    const vector<int> spectrum = GetSpectrum(peptide, cyclic);
    vector<int>::const_iterator it;
    for( it=spectrum.begin(); it != spectrum.end(); ++it )
    {
        if( counts.find(*it) != counts.end() )
        {
            if( counts[*it] > 0 )
            {
                ++score;
                counts[*it]--;
            } 
        }
    }
    return score;
}

int Score2(const vector<int>& peptide, map<int,int> weight_counts, bool cyclic )
{
    map<int,int> counts(weight_counts);
    int score = 0;
    const vector<int> spectrum = GetSpectrum2(peptide, cyclic);
    vector<int>::const_iterator it;
    for( it=spectrum.begin(); it != spectrum.end(); ++it )
    {
        if( counts.find(*it) != counts.end() )
        {
            if( counts[*it] > 0 )
            {
                ++score;
                counts[*it]--;
            } 
        }
    }
    return score;
}

const map<int,int> SpectralConvolution(const vector<int>& spectrum)
{
    //const vector<int> spectrum = GetSpectrum2(peptide, false);
    int length = spectrum.size();
    cout << "sepctrum size: " << length << endl;
    map<int,int> counts;
    for( int i=0; i<length; ++i )
    {
        int a = spectrum[i];
        for( int j = i+1; j < length; ++j )
        {
            
            int b = spectrum[j];
            int diff = b-a;
            if (diff == 0 )
            {
                continue;
            }
            if( diff < 0 )
            {
                cout << "Incoming peptide should be sorted" << endl;
                throw 1;
            }
            if( counts.find(diff) != counts.end() )
            {
                counts[diff]++;
            }
            else
            {
                counts[diff] = 1;
            }
        }
    }
    return counts;
}

const set<string> Neighbors( string pattern, int d )
{
    const char bases[4] = {'A','C','G','T'}; 

    set<string> neighbs;
    if( d == 0 )
    {
        neighbs.insert(pattern);
        return neighbs;
    }
    int pattern_len = pattern.length();
    if( pattern_len == 1)
    {
        neighbs.insert("A");
        neighbs.insert("C");
        neighbs.insert("G");
        neighbs.insert("T");
        return neighbs;
    }

    char first = pattern[0];
    string suffix = pattern.substr(1,pattern_len-1);
    const set<string> suffixNeighbors = Neighbors( suffix, d );
    set<string>::const_iterator it;
    for( it = suffixNeighbors.begin(); it != suffixNeighbors.end(); ++it )
    {
        if(HammingDistance(suffix,*it) == d)
        {
            neighbs.insert(first + *it);
        }
        else
        {
            for( int i=0; i<4; ++i )
            {
                neighbs.insert( bases[i] + *it );
            }
        }
    }
    return neighbs;
}

float Entropy(const vector< vector<float> >& profile)
{
    float entropy = 0;
    vector< vector<float> >::const_iterator it;
    for( it = profile.begin(); it != profile.end(); ++it )
    {
        vector<float> row = *it;
        vector<float>::iterator it2;
        for( it2 = row.begin(); it2 != row.end(); ++it2 )
        {
            if( *it2 == 0 )
                continue;
            entropy += *it2 * log2(*it2);
        }
    }
    return -1*entropy; 
}

float ProfileProbability(string pattern, const vector< vector<float> >& profile){
    float prob = 1;
    for( int col=0; col < pattern.length(); ++col )
    {
        int row = BaseToNumber(pattern[col]);
        prob *= profile[row][col];
    }
    return prob;
}

int MotifScore( const vector<string>& motif )
{
    int score = 0;
    int counts[4];
    for( int col=0; col < motif[0].length(); ++col )
    {
        int max = 0;
        for(int i=0; i<4; ++i) counts[i] = 0;
        for(int row=0; row < motif.size(); ++row)
        {
            int baseNum = BaseToNumber(motif[row][col]);
            if( ++counts[baseNum] > max )
            {
                max = counts[baseNum];
            }
        }
        score += motif.size() - max;
    }
    return score;
}

string ProfileMostProbable( const vector< vector<float> >& profile, string pattern, int k)
{
    float maxProb = -1;
    string maxKmer;
    for(int i=0; i<pattern.length()-k+1; ++i )
    {
        string kmer = pattern.substr(i,k);
        float prob = ProfileProbability(kmer, profile);
        if( prob > maxProb )
        {
            maxProb = prob;
            maxKmer = kmer;
        }
    }
    return maxKmer;
}

const vector< vector<float> > BuildProfile( vector<string> motif )
{
    vector< vector<float> > profile;
    int len = motif[0].length();
    vector<float> a(len,0);
    vector<float> c(len,0);
    vector<float> g(len,0);
    vector<float> t(len,0);

    profile.push_back(a);
    profile.push_back(c);
    profile.push_back(g);
    profile.push_back(t);

    vector<string>::iterator row;
    for( row = motif.begin(); row != motif.end(); ++row )
    {
        for( int col=0; col<len; ++col )
        {
            char base = (*row)[col];
            int ix = BaseToNumber(base);
            ++profile[ix][col];
        }
    }
   
    int div = motif.size();    
    for( int row = 0; row < 4; ++row )
    {
        for( int col=0; col<len; ++col )
        {
            profile[row][col] /= (float) div;
        }
    }
    return profile;
}
