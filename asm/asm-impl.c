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
    "movq %%rdi,%%rax;"//将env传到rax里；
    "movq %%rbx,(%%rax);"
    "movq %%rsp,8(%%rax);"
    "movq %%rbp,16(%%rax);"
    "movq %%r12,24(%%rax);"
    "movq %%r13,32(%%rax);"
    "movq %%r14,40(%%rax);"
    "movq %%r15,48(%%rax);"
    "movq 8(%%rbp),%%rbx;"
    "movq %%rbx,56(%%rax);"
    "xorq %%rax,%%rax;"
    :"=a"(value)
    :"D"(env)
  );

  return value;
}

inline void asm_longjmp(asm_jmp_buf env, int val) {
  //longjmp(env, val);
  asm volatile(
    "movq %%rdi,%%rax;"//env到rax
    "movq %%rsi,%%rdx;"//val到rdx
    "test %%rdx,%%rdx;"//and
    "je .L4;"
    "incq %%rdx;"
    ".L4:;"
    "movq (%%rax),%%rbx;"
    "movq 8(%%rax),%%rsp;"
    "movq 16(%%rax),%%rbp;"
    "movq 24(%%rax),%%r12;"
    "movq 32(%%rax),%%r13;"
    "movq 40(%%rax),%%r14;"
    "movq 48(%%rax),%%r15;"
    :
    :"a"(env),"d"(val)
  );
  
}
  