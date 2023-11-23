#include "asm.h"
#include <assert.h>
#include <stdio.h>

int main() {
  asm_jmp_buf buf;
  int r = asm_setjmp(buf);
  if (r == 0) {
    assert(asm_add(1234, 5678) == 6912);
    //printf("%ld",asm_add(1234, 5678));
    assert(asm_popcnt(0x0123456789abcdefULL) == 32);
    assert(asm_popcnt(19258096) == 13);
    assert(asm_popcnt(597399915) == 17);
    assert(asm_popcnt(20041202) == 14);
    assert(asm_popcnt(20040105) == 12);
    assert(asm_popcnt(3218382739) == 21);
    // TODO: add more tests here.
    asm_longjmp(buf, 123);
  } else {
   assert(r == 123);
   printf("PASSED.\n");
  }
}
