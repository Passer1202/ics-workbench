#include "asm.h"
#include <assert.h>
#include <stdio.h>

int main() {
  asm_jmp_buf buf;
  int r = asm_setjmp(buf);

  printf("%d",r);
  
  assert(asm_popcnt(0x0123456789abcdefULL)==32);
  printf("%d",asm_popcnt(0x0123456789abcdefULL));
  if (r == 0) {
    assert(asm_add(1234, 5678) == 6912);
    //printf("%ld",asm_add(1234, 5678));
    // TODO: add more tests here.
    assert(asm_popcnt(0x0123456789abcdefULL)==32);
    asm_longjmp(buf, 123);
  } else {
   assert(r == 123);
   printf("PASSED.\n");
  }
  
}
