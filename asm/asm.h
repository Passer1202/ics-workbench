#include <stdint.h>
#include <stddef.h>

// TODO: replace with your definition
//#include <setjmp.h> // remove this
/*
typedef  struct
{
 unsigned long long __rbx; // 通用数据寄存器之一
 unsigned long long __rsp; // 栈指针寄存器
 unsigned long long __rbp; // 基址指针寄存器
 unsigned long long __r12;
 unsigned long long __r13;
 unsigned long long __r14;
 unsigned long long __r15;
 unsigned long long __rip;
}jmp_buf[1];
*/

typedef unsigned long long myjmp_buf[10];

#define asm_jmp_buf myjmp_buf

//struct int[16];
int64_t asm_add(int64_t a, int64_t b);
// 返回无符号 64 位整数 x 二进制表示中 1 的数量
int asm_popcnt(uint64_t x);
// C 标准库中的 memcpy，用于复制两段不相交的内存
void *asm_memcpy(void *dest, const void *src, size_t n);

// C 标准库中的 setjmp/longjmp，用于控制流长跳转
int asm_setjmp(asm_jmp_buf env) ;
void asm_longjmp(asm_jmp_buf env, int val);