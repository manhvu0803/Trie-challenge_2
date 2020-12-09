#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <queue>
#include <cstring>

using namespace std;

struct node
{
    char ch;
    bool eow = false; // End of word
    node* next[26] = {nullptr};

    node(char c): ch(c) {};

    // Count the children of this node
    unsigned int childCnt(); 
};

unsigned int node::childCnt()
{
    int cnt = 0;
    for (int i = 0; i < 26; ++i)
        if (next[i] != nullptr) ++cnt;
    return cnt;
}

class trie
{
    private:
        node* root;

        int removeWord(const string& word, node* cur, int pos = 0);

        void clean(node* cur);

    public:
        trie();
        ~trie();

        void add(const string& word);
        
        //bool remove(const std::string& word);

        //void search(const std::string& word);
        //void listValidate(std::ostream& out, const std::string& word, std::unordered_set<std::string>* st = nullptr);
        
        // The function to list all possible word from given characters
        void list(std::ostream& out, const std::string& word);

        //friend std::ostream& operator<<(std::ostream& out, const trie& tr);
};

void trie::clean(node* cur)
{
    for (int i = 0; i < 26; ++i)
        if (cur->next[i] != nullptr) clean(cur->next[i]);

    delete cur;
}

trie::trie(): root(new node('1')) {};

trie::~trie()
{
    if (root == nullptr) return;
    clean(root);
}

void trie::add(const std::string& word)
{
    node* cur = root;
    for (int i = 0, lim = word.length(); i < lim; ++i) {
        if (word[i] < 'a' || word[i] > 'z') throw out_of_range("Only characters between a and z");
        node*& next = cur->next[word[i] - 'a'];
        if (next == nullptr) next = new node(word[i]);
        cur = next;
    }
    cur->eow = true;
}

void listWord(queue<string>& qu, node* cur, char str[], bool valid[], int pos = 0)
{
    // If reach end of word, put the string so far into queue
    if (cur->eow) {
        str[pos] = '\0';
        string s = str;
        if (s.length() > 2) qu.push(s);
    }

    // Go through every usable next character
    for (int i = 0; i < 26; ++i)
        if (valid[i] && cur->next[i] != nullptr) {
            str[pos] = i + 'a';
            listWord(qu, cur->next[i], str, valid, pos + 1);
        }
}

void trie::list(ostream& out, const std::string& word)
{
    char str[128];
    bool valid[26];
    queue<std::string> qu;

    // Mark the usable characters
    memset(valid, false, sizeof(valid));
    for (int i = 0, lim = word.length(); i < lim; ++i)
        if (word[i] >= 'a' && word[i] <= 'z') valid[word[i] - 'a'] = true;
    
    // Put all word that can be made from given characters into queue
    listWord(qu, root, str, valid);
    
    // Output
    out << qu.size() << '\n';
    while (!qu.empty()) {
        out << qu.front() << '\n';
        qu.pop();
    }
}

int main()
{
    trie tr;

    // Get the words from Dic.txt
    ifstream file("Dic.txt");
    string str;
    while (getline(file, str))
        tr.add(str);

    // Get list of characters to create words
    // All invalid characters (numeric, symbol, caps, ...) will be ignored
    getline(cin, str);

    // Output list of words
    try {
        tr.list(cout, str);
    }
    catch (out_of_range exc) {
        cerr << exc.what() << '\n';
    }
}
