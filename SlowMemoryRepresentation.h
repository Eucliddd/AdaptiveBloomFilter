//
// Created by Administrator on 2022/4/30.
//

#ifndef ABF_SLOWMEMORYREPRESENTATION_H
#define ABF_SLOWMEMORYREPRESENTATION_H
#include<set>
#include <vector>
using namespace std;
class SlowMemoryRepresentation {
    friend class AdaptiveBloomFilter;
private:
    int words;
    int bits;
    int hash_groups;
    vector<char> back_bloom;
    unsigned int s;
    int wordidx_size;
    int bitidx_size;
public:
    SlowMemoryRepresentation(int words=2048,int bits=32, int hash_groups=2);
    ~SlowMemoryRepresentation();
private:
    void add(unsigned int group,unsigned int wordidx, const set<unsigned int>& idx);
    char* get(unsigned int group,unsigned int wordidx);
    void clear();
};


#endif //ABF_SLOWMEMORYREPRESENTATION_H
