//
// Created by Administrator on 2022/4/30.
//

#include <cmath>
#include "HashFunctionSHA512.h"
#include "macros.h"
#include <cstring>
#include <iostream>
#include <cassert>
using namespace mySHA512;
using namespace std;
void mySHA512::array_2_bitset(const unsigned char *buf, uint32_t n_bytes, std::bitset<512> &bits)
{
    for (int i = n_bytes-1; i >= 0; --i)
    {
        unsigned char ch = buf[i];
        unsigned int n_offset = (n_bytes-i-1) * 8;
        for (int j = 0; j < 8; ++j)
        {
            bits.set(n_offset + j, ch & (1 << j));	// 第j位为是否为1
        }
    }
}
HashFunctionSHA512::HashFunctionSHA512(int words, int bits, int nhash, int hash_groups):
bits(bits),
nhash(nhash),
hash_groups(hash_groups)
{
    wordidx_size = int(log2(words));
    int alt_val = int((512-wordidx_size)/(hash_groups*nhash));
    int min_val = ceil(log2(bits));
    bitidx_size = min_val<alt_val ? min_val:alt_val;
    lastelement = "black alert";
    SHA512(reinterpret_cast<const unsigned char *>(lastelement.c_str()),
           lastelement.length(), lasthash);
    //cout<<lastelement<<endl<<lasthash<<endl;
    assert(hash_groups * nhash * bitidx_size + wordidx_size);
}

unsigned int HashFunctionSHA512::getword_idx(const char* element) {
    if(strcmp(element, lastelement.c_str())!=0){
        lastelement = element;
        SHA512(reinterpret_cast<const unsigned char *>(lastelement.c_str()),
               lastelement.length(), lasthash);
    }
    unsigned int result=0;
    for(int i=0;i<wordidx_size/8;i++) {
        result <<= 8;
        result += lasthash[i];
    }
    result <<= wordidx_size%8;
    result += GET(lasthash[wordidx_size/8],wordidx_size%8);

    return result;
}

unsigned int HashFunctionSHA512::getbit_idx(const char* element, unsigned int n, unsigned int group) {
    assert(group<hash_groups);
    n = group * nhash + n;
    if(strcmp(element, lastelement.c_str())!=0){
        lastelement = element;
        SHA512(reinterpret_cast<const unsigned char *>(lastelement.c_str()),
               lastelement.length(), lasthash);
    }
    unsigned int start = wordidx_size + bitidx_size * n;
    unsigned int end = start + bitidx_size;
    assert(end > start);
    unsigned int result = 0;
    for(unsigned int i=start;i<end;i++){
        if(i==start){
            unsigned int k = 8 - i % 8;
            result = (unsigned int)lasthash[i/8] & ONES(k);
        }
        else if(i == end-1){
            result <<= (end % 8);
            unsigned int k = 8 - end % 8;
            result += (unsigned int)lasthash[end/8] >> k;
        }
        else{
            result <<= 8;
            result += (unsigned int)lasthash[i/8];
        }
    }
    return result%bits;
}

HashFunctionSHA512::~HashFunctionSHA512() = default;


