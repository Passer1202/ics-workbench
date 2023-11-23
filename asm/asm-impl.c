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
    int ans;//0
    //x       1
    asm volatile(

        "movl $0,%%eax;"
        ".L1:;"
        "cmpq $0,%%rcx;"
        "jz .L3;"
        "shrq $1,%%rcx;"
        "jnc .L2;"
        "incl %%eax;"
        ".L2:;"
        "jmp .L1;"
        ".L3:"
        
        :"=a"(ans) 
        :"c"(x)
    );
    return ans;
}

void *asm_memcpy(void *dest, const void *src, size_t n) {
  //return memcpy(dest, src, n);
  //return dest;
}


int asm_setjmp(asm_jmp_buf env) {
  //return setjmp(env);
  return 0;
}

void asm_longjmp(asm_jmp_buf env, int val) {
  //longjmp(env, val);
  
}
  