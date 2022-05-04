//
// Created by Administrator on 2022/4/30.
//

#ifndef ABF_DICTIONARY_H
#define ABF_DICTIONARY_H
#include <set>
#include <string>
#include <vector>

using namespace std;

typedef struct HashNode{
    struct HashNode* next;
    char* value;
    HashNode():next(nullptr),value(nullptr){}
}Node;


class HashTable{
private:
    vector<Node*> table;
    size_t s=0;
    size_t n=0;
public:
    explicit HashTable(size_t s);
    ~HashTable();
    inline size_t size() const {return s;}
    inline size_t nelement() const {return n;}

public:
    bool insert(const char* element);
    bool find(const char* element);
    bool erase(const char* element);
    void clear();
};


class Dictionary {
private:
    set<string> data;
public:
    Dictionary()=default;
    inline void add(const char* element){
        data.insert(element);
    };
    inline bool test(const char* element){
        return data.find(element)!=data.end();
    };
    inline unsigned int len(){
        return data.size();
    };
    //bool is_disjoint(Dictionary& d);
    inline void clear(){
        data.clear();
    }
    inline bool count(const string& element){
        return data.count(element);
    }
};

class HashDictionary{
private:
    HashTable hashTable;
public:
    explicit HashDictionary(size_t size): hashTable(size){}
    inline void add(const char* element){
        hashTable.insert(element);
    };
    inline bool query(const char* element){
        return hashTable.find(element);
    };
    inline size_t len(){
        return hashTable.size();
    };
    inline size_t size(){
        return hashTable.nelement();
    }
    //bool is_disjoint(Dictionary& d);
    inline void clear(){
        hashTable.clear();
    }
};


#endif //ABF_DICTIONARY_H
