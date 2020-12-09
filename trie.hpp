#pragma once

#include <iostream>
#include <stdexcept>
#include <queue>
#include <unordered_set>
#include <cstring>

struct node
{
    char ch;
    bool eow = false;
    node* next[26] = {nullptr};

    node(char c): ch(c) {};

    unsigned int childCnt();
};

unsigned int node::childCnt()
{
    int cnt = 0;
    for (int i = 0; i < 26; ++i)
        if (next[i] != nullptr) ++cnt;
    return cnt;
}

void printWord(std::ostream& out, node* cur, char str[], bool valid[], int pos = 0)
{
    if (cur->eow) {
        str[pos] = '\0';
        out << str << '\n';
    }

    for (int i = 0; i < 26; ++i)
        if (valid[i] && cur->next[i] != nullptr) {
            str[pos] = i + 'a';
            printWord(out, cur->next[i], str, valid, pos + 1);
        }
}

// Put all word that can be made from the given characters into queue
void listWord(std::queue<std::string>& qu, node* cur, char str[], bool valid[], int pos = 0)
{
    if (cur->eow) {
        str[pos] = '\0';
        std::string s = str;
        if (s.length() > 2) qu.push(s);
    }

    for (int i = 0; i < 26; ++i)
        if (valid[i] && cur->next[i] != nullptr) {
            str[pos] = i + 'a';
            listWord(qu, cur->next[i], str, valid, pos + 1);
        }
}

class trie
{
    private:
        node* root;

        int removeWord(const std::string& word, node* cur, int pos = 0);

        void clean(node* cur);

    public:
        trie();
        ~trie();

        void add(const std::string& word);
        
        bool remove(const std::string& word);

        void search(const std::string& word);
        void listValidate(std::ostream& out, const std::string& word, std::unordered_set<std::string>* st = nullptr);
        
        // The function to list all possible word from given characters
        void list(std::ostream& out, const std::string& word);

        friend std::ostream& operator<<(std::ostream& out, const trie& tr);
};

int trie::removeWord(const std::string& word, node* cur, int pos)
{
    if (pos >= word.length()) {
        cur->eow = false;

        if (cur->childCnt() <= 0) return 1;
        return 0;
    }

    node*& next = cur->next[word[pos] - 'a'];
    if (next != nullptr) {
        if (removeWord(word, next, pos + 1) > 0) {
            delete next;
            next = nullptr;
            if (cur->childCnt() <= 0 && !cur->eow) return 1;
        }
    }
    else return -1;

    return 0;
}

void trie::clean(node* cur)
{
    for (int i = 0; i < 26; ++i)
        if (cur->next[i] != nullptr) clean(cur->next[i]);

    delete cur;
}

trie::trie(): root(new node('1')) {};

trie::~trie()
{
    clean(root);
}

void trie::add(const std::string& word)
{
    node* cur = root;
    for (int i = 0, lim = word.length(); i < lim; ++i) {
        if (word[i] < 'a' || word[i] > 'z') throw std::out_of_range("Only characters between a and z");
        node*& next = cur->next[word[i] - 'a'];
        if (next == nullptr) next = new node(word[i]);
        cur = next;
    }
    cur->eow = true;
}

bool trie::remove(const std::string& word)
{
    if (removeWord(word, root) < 0) return false;
    return true;
}

void trie::listValidate(std::ostream& out, const std::string& word, std::unordered_set<std::string>* st)
{
    char str[128];
    bool valid[26];
    std::queue<std::string> qu;
    
    std::memset(valid, false, sizeof(valid));
    for (int i = 0, lim = word.length(); i < lim; ++i)
        if (word[i] - 'a' >= 0 || word[i] - 'a' <= 26) valid[word[i] - 'a'] = true;
    
    listWord(qu, root, str, valid);
    
    out << std::boolalpha << qu.size() << '\n';
    while (!qu.empty()) {
        if (st != nullptr) out << (st->count(qu.front()) > 0) << ' ';
        out << qu.front() << '\n';
        qu.pop();
    }
}

//
void trie::list(std::ostream& out, const std::string& word)
{
    char str[128];
    bool valid[26];
    std::queue<std::string> qu;
    
    std::memset(valid, false, sizeof(valid));
    for (int i = 0, lim = word.length(); i < lim; ++i)
        if (word[i] - 'a' >= 0 && word[i] - 'a' <= 26) valid[word[i] - 'a'] = true;
    
    listWord(qu, root, str, valid);
    
    out << std::boolalpha << qu.size() << '\n';
    while (!qu.empty()) {
        out << qu.front() << '\n';
        qu.pop();
    }
}

std::ostream& operator<<(std::ostream& out, const trie& tr)
{
    char str[128];
    bool valid[26];
    std::memset(valid, true, sizeof(valid));
    printWord(out, tr.root, str, valid);
    return out;
}

