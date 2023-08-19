#include <iostream>
#include <fstream>
#include "format.h"
using namespace std;

int main(int argc, char **argv)
{
    string compareFile(argv[1]);
    formater fs("input.txt");
    if(fs.checkFormat()){
        cout << "Correct Syntax" << endl;


        ifstream inputFile(compareFile);
        string text = "", temp;
        while(inputFile >> temp)
            text = text + temp;
        list<signature*>::iterator i = fs.signatures.end();
        do{
            --i;
            signature* sig = *i;
            if(fs.performCheck(sig->types, text, sig->check)){
                cout << "Matched" << endl;
            } else
                cout << "Not Matched" << endl;
        } while(i != fs.signatures.begin());

    }
    else
        cout << "Incorrect Syntax" << endl;
    return 0;
}
