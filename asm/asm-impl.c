#include "asm.h"
#include <string.h>
// 返回有符号 64 位整数 a + b 的数值
int64_t asm_add(int64_t a, int64_t b) {
  
  return a+b;
}
// 返回无符号 64 位整数 x 二进制表示中 1 的数量
int asm_popcnt(uint64_t x) {
  int s = 0;
  for (int i = 0; i < 64; i++) {
    if ((x >> i) & 1) s++;
  }
  return s;
}
// C 标准库中的 memcpy，用于复制两段不相交的内存
void *asm_memcpy(void *dest, const void *src, size_t n) {
  return memcpy(dest, src, n);
}

// C 标准库中的 setjmp/longjmp，用于控制流长跳转
int asm_setjmp(asm_jmp_buf env) {
  return setjmp(env);
}

void asm_longjmp(asm_jmp_buf env, int val) {
  longjmp(env, val);
}
