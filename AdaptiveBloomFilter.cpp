//
// Created by Administrator on 2022/4/30.
//

#include <cstring>
#include <cmath>
#include <iostream>
#include <cassert>
#include "AdaptiveBloomFilter.h"
#include "macros.h"

AdaptiveBloomFilter::AdaptiveBloomFilter(int w, int b, int nhash, int hash_groups):
words(w),
bits(b),
nhash(nhash),
hash_group(hash_groups),
SMR(words,bits,hash_groups),
hash_f(words,bits,nhash,hash_group)
{
    s = ceil((words*bits)/8);
    bloom_filter.resize(s);
    for (int i=0;i<s;i++) bloom_filter[i]=0;
    selector_bits.resize(words);
    for(int i=0;i<words;i++) selector_bits[i]=0;
    wordidx_size = int(log2(words));
    bitidx_size = int(log2(bits));

    printf("***:ABF:\n");
    printf("***:words: %d\n", words);
    printf("***:bits per words: %d\n", bits);
    printf("***:total bits: %d\n", words*bits);
    printf("***:---------------------------\n");
}

void AdaptiveBloomFilter::add(const char* element) {
    unsigned int wordidx = hash_f.getword_idx(element);
    assert(wordidx < words);
    unsigned int group_i = selector_bits[wordidx];
    for(int i=0;i<nhash;i++){
        unsigned int bitidx=hash_f.getbit_idx(element,i,group_i);
        auto idx = (unsigned int)(wordidx*bits+bitidx);
        if(idx >= words*bits){
            cerr<<"idx: "<<idx<<" >= total: "<<words*bits<<endl;
            cerr<<" wordidx: "<<wordidx<<" bitidx: "<<bitidx<<endl;
            exit(3);
        }
        SETBIT(bloom_filter[idx/8],7-(idx%8));
    }
    addslow(element);
}

bool AdaptiveBloomFilter::query(const char* element) {
    unsigned int wordidx = hash_f.getword_idx(element);
    unsigned int group_i = selector_bits[wordidx];
    for(int i=0;i<nhash;i++){
        unsigned int bitidx = hash_f.getbit_idx(element,i,group_i);
        auto idx =(unsigned int)(wordidx*bits+bitidx);
        if(TESTBIT(bloom_filter[idx / 8],7-(idx % 8)) == 0)
            return false;
    }
    return true;
}

void AdaptiveBloomFilter::adapt(const char* element) {
    unsigned int wordidx=hash_f.getword_idx(element);
    unsigned int qqq = selector_bits[wordidx];
    unsigned int new_group = (selector_bits[wordidx]+1) % hash_group;
    char* word=SMR.get(new_group,wordidx);
    unsigned int wordinit = wordidx*bits;
    unsigned int wordend = wordinit+bits-1;
    unsigned int size = wordend / 8 - wordinit / 8 + 1;
    for(int i=0;i<size;i++) {
        if(i == 0){
            bloom_filter[wordinit / 8] = SET_VAL0(8 - wordinit % 8,
                                                      word[0],
                                                      bloom_filter[wordinit / 8]);
        }
        else if(i == size-1){
            bloom_filter[wordend / 8] = SET_VAL0(7 - wordend % 8,
                                                      bloom_filter[wordend / 8],
                                                      word[i]);
        }
        else
            bloom_filter[wordinit / 8 + i] = word[i];
    }
    delete[] word;
    word= nullptr;
    selector_bits[wordidx] = new_group;
}

void AdaptiveBloomFilter::addslow(const char* element) {
    unsigned int wordidx=hash_f.getword_idx(element);
    set<unsigned int> idx_list;
    for(int g=0;g<hash_group;g++){
        idx_list.clear();
        for(int i=0;i<nhash;i++){
            unsigned int bitidx=hash_f.getbit_idx(element,i,g);
            idx_list.insert(bitidx);
        }
        SMR.add(g,wordidx,idx_list);
    }
}

void AdaptiveBloomFilter::clear() {
    bloom_filter.clear();
    bloom_filter.resize(s);
    for (int i=0;i<s;i++) bloom_filter[i]=0;
    selector_bits.clear();
    selector_bits.resize(words);
    for(int i=0;i<words;i++) selector_bits[i]=0;
    SMR.clear();
}

AdaptiveBloomFilter::~AdaptiveBloomFilter() = default;
