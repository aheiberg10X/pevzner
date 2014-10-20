#include "../../helpers/generics.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <zlib.h>
#include "../../helpers/kseq.h"
#include <vector>
#include <set>
#include <stdio.h>

using namespace std;

int main(int argc, char* argv[])
{
    string input = "asdfas";
    string output;
    output.resize(input.length());
    ReverseComplement(input, output);
    cout << output << endl;
    return 0;
}
