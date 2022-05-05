//
// Created by Administrator on 2022/4/30.
//

#include <cstring>
#include <cmath>
#include <iostream>
#include <cassert>
#include "AdaptiveBloomFilter.h"
#include "macros.h"

AdaptiveBloomFilter::AdaptiveBloomFilter(int w, int b, short ss, int nhash, int hash_groups):
words(w),
nhash(nhash),
hash_group(hash_groups),
selector_size(ss),
bits(b),
SMR(w,b,hash_groups),
hash_f(w,b,nhash,hash_groups)
{
    bloom_filter = new WORD[words+1];
    memset(bloom_filter,0,words * sizeof(WORD));
    //for (int i=0;i<words;i++) bloom_filter[i]=0;
    //for(int i=0;i<words;i++) selector_bits[i]=0;
    //wordidx_size = ceil(log2(words));
    //bitidx_size = int(log2(bits));

    printf("***:ABF:\n");
    printf("***:words: %d\n", words);
    printf("***:bits per words: %d\n", bits);
    printf("***:total bits: %d\n", words*bits);
    printf("***:---------------------------\n");
}

void AdaptiveBloomFilter::add(const char* element) {
    unsigned int wordidx = hash_f.getword_idx(element);
    assert(wordidx < words);
    WORD& w = bloom_filter[wordidx];
    unsigned short group_i = GET_S(selector_size,w);
    assert(group_i<hash_group);
    for(int i=0;i<nhash;i++){
        unsigned int bitidx=hash_f.getbit_idx(element,i,group_i);
        auto idx = BIT_SIZE_WORD-(bitidx+selector_size)-1;
/*        if(idx >= words*bits){
            cerr<<"idx: "<<idx<<" >= total: "<<words*bits<<endl;
            cerr<<" wordidx: "<<wordidx<<" bitidx: "<<bitidx<<endl;
            exit(3);
        }*/
        SETBIT(w,idx);
    }
    addslow(element);
}

bool AdaptiveBloomFilter::query(const char* element) {
    unsigned int wordidx = hash_f.getword_idx(element);
    assert(wordidx < words);
    WORD& w = bloom_filter[wordidx];
    unsigned short group_i = GET_S(selector_size,w);
    for(int i=0;i<nhash;i++){
        unsigned int bitidx = hash_f.getbit_idx(element,i,group_i);
        auto idx = BIT_SIZE_WORD - (bitidx + selector_size) - 1;
        if(TESTBIT(w,idx) == 0)
            return false;
    }
    return true;
}

void AdaptiveBloomFilter::adapt(const char* element) {
    unsigned int wordidx=hash_f.getword_idx(element);
    assert(wordidx < words);
    WORD& w = bloom_filter[wordidx];
    unsigned short qqq = GET_S(selector_size,w);
    unsigned short new_group = (qqq+1) % hash_group;
    WORD word=SMR.get(new_group,wordidx);
//    unsigned int wordinit = wordidx*bits;
//    unsigned int wordend = wordinit+bits-1;
//    unsigned int size = wordend / 8 - wordinit / 8 + 1;
//    for(int i=0;i<size;i++) {
//        if(i == 0){
//            bloom_filter[wordinit / 8] = SET_VAL0(8 - wordinit % 8,
//                                                      word[0],
//                                                      bloom_filter[wordinit / 8]);
//        }
//        else if(i == size-1){
//            bloom_filter[wordend / 8] = SET_VAL0(7 - wordend % 8,
//                                                      bloom_filter[wordend / 8],
//                                                      word[i]);
//        }
//        else
//            bloom_filter[wordinit / 8 + i] = word[i];
//    }
//    delete[] word;
//    word= nullptr;
    w = word;
    //selector_bits[wordidx] = new_group;
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
    for (int i=0;i<words;i++) bloom_filter[i]=0;
    SMR.clear();
}

AdaptiveBloomFilter::~AdaptiveBloomFilter() {
    delete[] bloom_filter;
    bloom_filter = nullptr;
}
