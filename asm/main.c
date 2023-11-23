#include "asm.h"
#include <assert.h>
#include <stdio.h>

int main() {
  asm_jmp_buf buf;
  int r = asm_setjmp(buf);
  if (r == 0) {
    assert(asm_add(1234, 5678) == 6912);
    //printf("%ld",asm_add(1234, 5678));
    // TODO: add more tests here.
    asm_longjmp(buf, 123);
  } else {
   assert(r == 123);
   printf("PASSED.\n");
  }
}
