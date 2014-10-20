#include "generics.h"
#include <iostream>
#include <sstream>

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
