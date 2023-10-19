#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

const uint64_t MAX=-1;//64位全为1
const uint64_t M=(uint64_t)1<<(uint64_t)63;

uint64_t com(uint64_t x,uint64_t y){//比较，等于返回0，小于返回1，大于返回2

for(int i=63;i>=0;i--)
{
    uint64_t a=(x>>i)&1;
    uint64_t b=(y>>i)&1;
    if(a==1&&b==0)
    {
        return (uint64_t)2;
    }
    if(a==0&&b==1)
    {
        return (uint64_t)1;
    }
}

return (uint64_t)0;

}

uint64_t mod(uint64_t x,uint64_t y){
    uint64_t m=y;
    while(com(m,x)==0||com(m,x)==1)//高效减去m的倍数m<=x
    {
        while(com(m,M)==1&&com(m,x)==1)//m<M&&m<x
        {
            m=m<<1;
        }
        while(com(m,x)==2&&com(m,y)==2)//m>x&&m>y
        {
            m=m>>1;
        }
        x-=m;
    }

    return x;
}

uint64_t plus(uint64_t x,uint64_t y,uint64_t m){
    x=mod(x,m);
    y=mod(y,m);
    if(com(x,MAX-y)==2&&x!=0&&y!=0)
    {
        //printf("sss\n");

        return plus(plus(mod(MAX,m),mod(1,m),m),x+y,m);
    }
    return x+y;
}

uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
  //return (a * b) % m; // buggy
  uint64_t ans=0;
  uint64_t temp=a;

  while(b!=0)
  {
    
    if(b&1!=0)
    {
        ans=mod(plus(ans,temp,m),m);
        //printf("%llu\n",ans);
    }
    temp=mod(plus(temp,temp,m),m);
    b=b>>1;
  }

  return ans;
}


void test(uint64_t a, uint64_t b, uint64_t m) {
  #define U64 "%" PRIu64
  printf(U64 " * " U64 " mod " U64 " = " U64 "\n", a, b, m, multimod(a, b, m));
}

int main() {
  test(123, 456, 789);
  test(123, 456, -1ULL);
  test(-2ULL, -2ULL, -1ULL); // should be 1
}
