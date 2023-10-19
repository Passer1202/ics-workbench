#include <stdint.h>


const uint64_t MAX=-1;//64位全为1
const uint64_t M=(uint64_t)1<<(uint64_t)63;

uint64_t com(uint64_t x,uint64_t y){//比较，等于返回0，小于返回1，大于返回2

for(int i=63;i!=-1;i--)
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
    while(com(y,x)==(uint64_t)0||com(y,x)==(uint64_t)1)//高效减去m的倍数m<=x
    {
        while(com(m,M)==(uint64_t)1&&com(m,x)==(uint64_t)1)//m<M&&m<x
        {
            m=m<<1;
        }
        while(com(m,x)==(uint64_t)2&&com(m,y)==(uint64_t)2)//m>x&&m>y
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
    if(com(x,MAX-y)==(uint64_t)2&&x!=(uint64_t)0&&y!=(uint64_t)0)
    {
        //printf("sss\n");

        return plus(plus(mod(MAX,m),mod((uint64_t)1,m),m),x+y,m);
    }
    return x+y;
}

uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
  //return (a * b) % m; // buggy
  uint64_t ans=0;
  uint64_t temp=a;

  while(b!=0)
  {
    
    if(((b)&(1))!=0)
    {
        ans=mod(plus(ans,temp,m),m);
        //printf("%llu\n",ans);
    }
    temp=mod(plus(temp,temp,m),m);
    b=b>>(uint64_t)1;
  }

  return ans;
}


