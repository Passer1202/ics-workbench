#include <stdint.h>
#include <stddef.h>

// TODO: replace with your definition
#include <setjmp.h> // remove this
#define asm_jmp_buf jmp_buf


int64_t asm_add(int64_t a, int64_t b){
    int64_t c;
    asm volatile(
        "addq %1,%2;"
        "movq %2,%0;"
        :"=r"(c)
        :"r"(a),"r"(b)
    );
    return c;
};
// 返回无符号 64 位整数 x 二进制表示中 1 的数量
int asm_popcnt(uint64_t x);
// C 标准库中的 memcpy，用于复制两段不相交的内存
void *asm_memcpy(void *dest, const void *src, size_t n);

// C 标准库中的 setjmp/longjmp，用于控制流长跳转
int asm_setjmp(asm_jmp_buf env) ;
void asm_longjmp(asm_jmp_buf env, int val);