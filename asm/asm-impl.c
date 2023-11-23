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
    int out=0;//0
    //x       1
    return out;
}

inline void *asm_memcpy(void *dest, const void *src, size_t n) {
  //return memcpy(dest, src, n);
  return dest;
}


inline int asm_setjmp(asm_jmp_buf env) {
  //return setjmp(env);
  return 0;
}

inline void asm_longjmp(asm_jmp_buf env, int val) {
  //longjmp(env, val);
  
}
  