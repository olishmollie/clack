#include <iostream>

#include "headers/parser.h"

using namespace std;

int main()
{
    string s;
    while (true) {
        try {
            getline(cin, s);
            Parser p(s.c_str());
            cout << p.parse() << endl;
        } catch (const char *e) {
            cout << e << endl;
        }
    }
}