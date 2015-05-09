#include "secret.h"
#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

int p[1005][1005];
int n;

void Rek(int lo, int hi)
{
  if (lo == hi)
    return;

  int mid = (lo + hi) / 2;
  Rek(lo, mid);
  Rek(mid+1, hi);

  for (int i=mid, tmp=0; i>=lo; i--) {
    if (p[i][mid] == -1)
      p[i][mid] = Secret(p[i][i], tmp);
    tmp = p[i][mid];
  }

  for (int i=mid+1, tmp=0; i<=hi; i++) {
    if (p[mid+1][i] == -1)
      p[mid+1][i] = Secret(tmp, p[i][i]);;
    tmp = p[mid+1][i];
  }
}

void Init(int N, int A[]) {
  memset(p, -1, sizeof p);

  n = N;
  for (int i=0; i<n; i++)
    p[i][i] = A[i];

  Rek(0, n-1);
}

int RekQ(int lo, int hi, int begin, int end)
{
  if (hi < end || lo > begin)
    return -1;
  
  int mid = (lo + hi) / 2;
  if (mid >= begin && mid < end)
    return Secret(p[begin][mid], p[mid+1][end]);
  return max(RekQ(lo, mid, begin, end), RekQ(mid+1, hi, begin, end));
}

int Query(int L, int R) {
  if (L == R)
    return p[L][L];
  return RekQ(0, n-1, L, R);
}
