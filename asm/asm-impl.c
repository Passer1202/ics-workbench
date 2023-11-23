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
    int ans;//2
    asm volatile(
        ".L1:"
        "movl $0,%2;"
        "shrq $1,%1;"
        "jnc .L2;"
        "incl %2;"
        ".L2:"
        "cmpq $0,%1;"
        "jne .L1;"
        :"=r"(out) 
        :"r"(x),"r"(ans)
    );
    return out;
}

void *asm_memcpy(void *dest, const void *src, size_t n) {
  //return memcpy(dest, src, n);
  return dest;
}


int asm_setjmp(asm_jmp_buf env) {
  //return setjmp(env);
  return 0;
}

void asm_longjmp(asm_jmp_buf env, int val) {
  //longjmp(env, val);
  
}
  