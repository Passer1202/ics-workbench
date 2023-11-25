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
        :"=a"(out) 
        :"b"(x),"c"(ans),"d"(i),"S"(j)
    );
    return out;
}
inline void *asm_memcpy(void *dest, const void *src, size_t n) {
  //return memcpy(dest, src, n);
  //for(int i=0;i<n;i++){
  //  *dest+n=*src+n; 
  //}
  //i,d,src,n(64)
  size_t i;
  void* d=dest;
  asm volatile(
    "movq $0,%0;"
    ".L3:;"
    "movb (%2),%%al;"
    "movb %%al,(%1);"
    "incq %2;"
    "incq %1;"
    "incq %0;"
    "cmpq %3,%0;"
    "jb .L3;"
    :"=b"(i)
    :"c"(d),"d"(src),"S"(n)
  );
  return dest;
}


inline int asm_setjmp(asm_jmp_buf env) {
  //return setjmp(env);
  size_t value;
  asm volatile(
    "movq %%rdx,%%rax;"//将env传到rax里；
    "movq %%rbx,(%%rax);"
    "movq %%rsp,8(%%rax);"
    "movq %%rbp,16(%%rax);"
    "movq %%r12,24(%%rax);"
    "movq %%r13,32(%%rax);"
    "movq %%r14,40(%%rax);"
    "movq %%r15,48(%%rax);"
    "movq (%%rbp),%%rbx;"
    "xorq %%rax,%%rax;"
    :"=a"(value)
    :"d"(env)
  );
  return 0;
}

inline void asm_longjmp(asm_jmp_buf env, int val) {
  //longjmp(env, val);
  
}
  