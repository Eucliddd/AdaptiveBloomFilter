//
// Created by Administrator on 2022/4/30.
//

#ifndef ABF_ADAPTIVEBLOOMFILTER_H
#define ABF_ADAPTIVEBLOOMFILTER_H
#include "SlowMemoryRepresentation.h"
#include "HashFunctionSHA512.h"
#include "Dictionary.h"
#include "macros.h"

using namespace mySHA512;
class AdaptiveBloomFilter {
private:
    // number of words
    int words;
    //the number of hashes per group
    int nhash;
    //the number of total hash groups (f, g ...)
    int hash_group;

    short selector_size;
    //number of bits per words
    int bits;
    //filter in memory
    WORD* bloom_filter;
    //the selector array with an integer per word
    //vector<unsigned char> selector_bits;
    //void* hash_f;
    //the size of the word index
    //int wordidx_size;
    //the size of each bit index to set/get a bit from the word
    //int bitidx_size;
    //SlowMemoryRepresentation
    SlowMemoryRepresentation SMR;
    HashFunctionSHA512 hash_f;
public:
    explicit AdaptiveBloomFilter(int w=1024,int bits=63,short selector_size=1, int nhash=2,int hash_groups=2);
    ~AdaptiveBloomFilter();
    void add(const char* element);
    bool query(const char* element);
    void adapt(const char* element);
    void clear();
private:
    void addslow(const char* element);
};


#endif //ABF_ADAPTIVEBLOOMFILTER_H
