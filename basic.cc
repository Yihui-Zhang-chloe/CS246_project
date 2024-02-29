#include "basic.h"

bool strToInt(int *out,string input){
    for (int i=0;i<input.length();i++)
    {
        if ( input[i] < 48 || input[i] >57 ) return false;
    }
    *out = stoi(input);
    return true;
}

