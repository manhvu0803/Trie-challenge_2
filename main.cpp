#include <iostream>
#include "trie.hpp"
#include <fstream>
#include <string>
#include <unordered_set>

using namespace std;

int main()
{
    ifstream file("Dic.txt");
    
    trie tr;

    // The set save every words in the file for double checking
    unordered_set<string> st;
    
    string str;
    while (getline(file, str)) {
        tr.add(str);
        st.insert(str);
    }

    getline(cin, str);

    tr.list(cout, str);
}
