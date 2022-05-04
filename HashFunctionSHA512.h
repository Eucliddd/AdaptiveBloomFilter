//
// Created by Administrator on 2022/4/30.
//

#ifndef ABF_HASHFUNCTIONSHA512_H
#define ABF_HASHFUNCTIONSHA512_H
#include <openssl/sha.h>
#include <cstdio>
#include <bitset>

namespace mySHA512 {
#define GET(b,i) ((b) >> (8-(i)))

    class HashFunctionSHA512 {
    private:
        int hash_groups;
        int nhash;
        int wordidx_size;
        int bitidx_size;
        int bits;
        std::string lastelement;
        unsigned char lasthash[64]{};
    public:
        HashFunctionSHA512(int words = 1024, int bits = 64, int nhash = 2, int hash_groups = 2);

        unsigned int getword_idx(const char *element);

        unsigned int getbit_idx(const char *element, unsigned int n, unsigned int group);

        ~HashFunctionSHA512();
    };

    void array_2_bitset(const unsigned char *buf, uint32_t n_bytes, std::bitset<512> &bits);
}
#endif //ABF_HASHFUNCTIONSHA512_H
