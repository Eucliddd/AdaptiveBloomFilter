//
// Created by Administrator on 2022/4/30.
//

#ifndef ABF_ADAPTIVEBLOOMFILTER_H
#define ABF_ADAPTIVEBLOOMFILTER_H
#include "SlowMemoryRepresentation.h"
#include "HashFunctionSHA512.h"
#include "Dictionary.h"
using namespace mySHA512;
class AdaptiveBloomFilter {
private:
    // number of words
    int words;
    //number of bits per words
    int bits;
    unsigned int s;
    //filter in memory
    vector<char> bloom_filter;
    //the selector array with an integer per word
    vector<unsigned char> selector_bits;
    //void* hash_f;
    //the number of hashes per group
    int nhash;
    //the number of total hash groups (f, g ...)
    int hash_group;
    //the size of the word index
    int wordidx_size;
    //the size of each bit index to set/get a bit from the word
    int bitidx_size;
    //SlowMemoryRepresentation
    SlowMemoryRepresentation SMR;
    HashFunctionSHA512 hash_f;
public:
    AdaptiveBloomFilter(int w=1024,int b=64,int nhash=2,int hash_groups=2);
    ~AdaptiveBloomFilter();
    void add(const char* element);
    bool query(const char* element);
    void adapt(const char* element);
    void clear();
private:
    void addslow(const char* element);
};


#endif //ABF_ADAPTIVEBLOOMFILTER_H
