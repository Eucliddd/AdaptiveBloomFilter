//
// Created by Administrator on 2022/4/30.
//

#include "SlowMemoryRepresentation.h"
#include <cstring>
#include <cmath>
#include <assert.h>
#include <iostream>
#include "macros.h"

SlowMemoryRepresentation::SlowMemoryRepresentation(int words, int bits, int hash_group):
words(words),
bits(bits),
hash_groups(hash_group)
{
    selector_size = ceil(log2(hash_group));
    back_bloom = new WORD[hash_groups * words + 1];
    int offset = BIT_SIZE_WORD - selector_size;
    for (int i=0;i<hash_groups;i++){
        for(int j=0;j<words;j++) {
            uint64_t x = ((uint64_t) i) << offset;
            back_bloom[i * words + j] = x;
        }
    }
    //wordidx_size=int(log2(words));
    //bitidx_size=int(log2(bits));
}

void SlowMemoryRepresentation::add(unsigned int group, unsigned int wordidx, const set<unsigned int>& idx) {
    unsigned int actual_idx = group * words + wordidx;
    WORD& w = back_bloom[actual_idx];
    for(auto i:idx){
        unsigned int actual_i = BIT_SIZE_WORD - (i + selector_size) - 1;
//        if(actual_i >= hash_groups*words*bits){
//            cerr<<"actual_i: "<<actual_i<<" >= total: "<<hash_groups*words*bits<<endl;
//            cerr<<"group: "<<group<<" wordidx: "<<wordidx<<" idx: "<<i<<endl;
//            exit(3);
//        }
        SETBIT(w,actual_i);
    }
}

WORD SlowMemoryRepresentation::get(unsigned int group, unsigned int wordidx) {
    assert(group<hash_groups);
    unsigned int actual_idx = group * words + wordidx;
    return back_bloom[actual_idx];
}

void SlowMemoryRepresentation::clear() {
    int offset = BIT_SIZE_WORD - selector_size;
    for (int i=0;i<hash_groups;i++){
        for(int j=0;j<words;j++) {
            uint64_t x = ((uint64_t) i) << offset;
            back_bloom[i * words + j] = x;
        }
    }
}

SlowMemoryRepresentation::~SlowMemoryRepresentation(){
    delete[] back_bloom;
    back_bloom = nullptr;
}
