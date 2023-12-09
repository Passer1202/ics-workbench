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
  for(int i=2;i<=n;i++){
    if(!is_not_prime[i])*p++ = i;

    for(int j=0;j<=n;j++){
      int k=primes[j];
      int z=k*i;
      if(k==0||z>n)break;

      is_not_prime[z]=true;

      if(i%k==0)break;

    }
  }
  //*p = 0;
  return primes;
}
