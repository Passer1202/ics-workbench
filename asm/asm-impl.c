#include "asm.h"
#include <string.h>

inline int64_t asm_add(int64_t a, int64_t b) 
{ 
  int64_t c;
  asm volatile(
    "addq %1,%2;"
    "movq %2,%0;"
    :"=r"(c)
    :"r"(a),"r"(b)
  );

  return c;
}

inline int asm_popcnt(uint64_t x){
    int out;//0
    //x       1
    int ans=0;//2
    int i=0;//3
    uint64_t j=0;//4
    asm volatile(
        ".L1:"
        "movq %1,%4;"
        "andq $0x1,%4;"
        "cmpq $1,%4;"
        "jl .L2;"
        "addl $1,%2;"
        ".L2:"
        "shrq $1,%1;"
        "addl $1,%3;"
        "cmpl $63,%3;"
        "jle .L1;"
        "movl %2,%0;"
        :"=r"(out) 
        :"r"(x),"r"(ans),"r"(i),"r"(j)
    );
    return out;
}

void *asm_memcpy(void *dest, const void *src, size_t n) {
  //return memcpy(dest, src, n);
}


int asm_setjmp(asm_jmp_buf env) {
  //return setjmp(env);
  return 0;
}

void asm_longjmp(asm_jmp_buf env, int val) {
  //longjmp(env, val);
  
}
  