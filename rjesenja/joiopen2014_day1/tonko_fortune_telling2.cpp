#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

typedef long long llint;

int N, M;
int A[234567];
int B[234567];
int main(void) {
  
  scanf("%d%d", &N, &M);
  if (N>1000) return 0;

  for (int i = 0; i < N; ++i) {
    scanf("%d%d", &A[i],&B[i]);
  }

  while (M--) {
    int k;
    scanf("%d", &k);
    for (int i = 0; i < N; ++i) {
      if (A[i] <= k) swap(A[i], B[i]);
    }
  }

  llint ans = 0;
  for (int i = 0; i < N; ++i)  ans+=A[i];
  printf("%lld\n", ans);

  return 0;
}

