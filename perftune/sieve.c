#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define N 10000000

 static bool is_prime[N];
 static int  primes[N]; 

 int *sieve(int n) {
  assert(n + 1 < N);
  for (int i = 0; i <= n; i++)
    is_prime[i] = true;

  for (int i = 2; i*i <= n; i++) {
    if(!is_prime[i])continue;
    for (int j = i * i; j <= n; j += i) {
      is_prime[j] = false;
    }
  }

  int *p = primes;
  if(n>2)*p++ = 2;
  int k=0;
  for (int i = 1; 1; i++){
    k=(i<<1)+1;
    if(k>n)break;
    if (is_prime[k]) {
      *p++ = k;
    }
  }
  *p = 0;
  return primes;
}
