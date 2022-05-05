//
// Created by Administrator on 2022/5/1.
//

#ifndef ABF_MACROS_H
#define ABF_MACROS_H
#include <cstdint>
#define WORD uint64_t
#define BIT_SIZE_WORD sizeof(WORD)*8
#define SETBIT(VAR,POS) (VAR |= 1ULL << (POS))
#define TESTBIT(VAR,POS) (VAR & 1ULL << (POS))
#define ONES(i) ((1ULL << (i))-1)
#define GET_S(s,val) (((val) >> (BIT_SIZE_WORD-(s))) & ONES(s))

#define UNSET0(i,val) ((val) & (~ONES((i))))
#define GET_VAL0(i,val) ((val) & ONES((i)))
#define SET_VAL0(i,val0,val1) (GET_VAL0((i),(val0)) | UNSET0((i),(val1)))

#define RESULT_PATH "../test_result/"
#define RESULT_FILE "result_file"

#endif //ABF_MACROS_H
