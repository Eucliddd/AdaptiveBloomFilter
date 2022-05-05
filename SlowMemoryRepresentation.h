//
// Created by Administrator on 2022/4/30.
//

#ifndef ABF_SLOWMEMORYREPRESENTATION_H
#define ABF_SLOWMEMORYREPRESENTATION_H
#include<set>
#include <vector>
#include <cstddef>
#include <cstdint>
#include "macros.h"
using namespace std;
class SlowMemoryRepresentation {
    friend class AdaptiveBloomFilter;
private:
    int words;
    int bits;
    int hash_groups;
    WORD* back_bloom;
    unsigned short selector_size;
    //int wordidx_size;
    //int bitidx_size;
public:
    explicit SlowMemoryRepresentation(int words=2048,int bits=32, int hash_groups=2);
    ~SlowMemoryRepresentation();
private:
    void add(unsigned int group,unsigned int wordidx, const set<unsigned int>& idx);
    WORD get(unsigned int group,unsigned int wordidx);
    void clear();
};


#endif //ABF_SLOWMEMORYREPRESENTATION_H
