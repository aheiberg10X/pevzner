#include <iostream>
#include "derp.h"


int A::a(int i) {
    return i + 2;
}

using namespace std;
int main() {
    cout << A::a(4) << endl;
}
