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
    s = ceil(hash_groups*words*bits/8);
    back_bloom.resize(s);
    for (int i=0;i<s;i++) back_bloom[i]=0;
    wordidx_size=int(log2(words));
    bitidx_size=int(log2(bits));
}

void SlowMemoryRepresentation::add(unsigned int group, unsigned int wordidx, const set<unsigned int>& idx) {
    for(auto i:idx){
        unsigned int actual_i = (group * words + wordidx) * bits + i;
        if(actual_i >= hash_groups*words*bits){
            cerr<<"actual_i: "<<actual_i<<" >= total: "<<hash_groups*words*bits<<endl;
            cerr<<"group: "<<group<<" wordidx: "<<wordidx<<" idx: "<<i<<endl;
            exit(3);
        }
        SETBIT(back_bloom[actual_i/8],7-(actual_i%8));
    }
}

char* SlowMemoryRepresentation::get(unsigned int group, unsigned int wordidx) {
    assert(group<hash_groups);
    unsigned int wordinit = (group * words + wordidx) * bits;
    unsigned int wordend = wordinit+bits-1;
    unsigned int size = wordend / 8 - wordinit / 8 + 1;
    char* result=new char[size];
    for(int i = 0;i < size;i++)
        result[i] = back_bloom[wordinit / 8 + i];
    return result;
}

void SlowMemoryRepresentation::clear() {
    back_bloom.clear();
    back_bloom.resize(s);
    for (int i=0;i<s;i++) back_bloom[i]=0;
}

SlowMemoryRepresentation::~SlowMemoryRepresentation() = default;
