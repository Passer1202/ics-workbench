#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define N 10000000

 static bool is_not_prime[N];
 static int  primes[N]; 

 int *sieve(int n) {
  assert(n + 1 < N);
  int *p = primes;
  if(n>2)*p++=2;
  for(int i=1;(i<<1)+1<=n;i++){
    int i1=(i<<1)+1;
    if(!is_not_prime[i1])*p++ = i1;

    for(int j=0;j<=n;j++){
      int k=primes[j];
      int z=k*i1;
      if(k==0||z>n)break;
      is_not_prime[z]=true;
      if(i1%k==0)break;
    }
  }
  //*p = 0;
  return primes;
}
