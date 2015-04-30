#include <cstdio>
#include <algorithm>
#include <vector>
#include <cassert>
#include <map>
#include <set>
#include <ctime>
#include <iostream>
#include <cstring>

#define TIME(x) cerr << x << ": " << (double)clock()/CLOCKS_PER_SEC << endl;

using namespace std;

typedef long long llint;
typedef pair<int, int> P;

const int MAX = 200000;

int n;
int k;
int A[MAX];
int B[MAX];
int T[MAX];
int L[MAX];
int C[MAX];

map<int, int> I;
int R[3*MAX+1];

void compress() {
  int m = 0;
  for (int i=0; i<n; i++) { 
    R[m++] = A[i];
    R[m++] = B[i];
  }
  for (int i=0; i<k; i++) {
    R[m++] = T[i];
  }
  sort(R, R+m);
  m = unique(R, R+m)-R;  
  TIME("halfcompress");
  for (int i=0; i<n; i++) {
    A[i] = lower_bound(R, R+m, A[i])-R;
    B[i] = lower_bound(R, R+m, B[i])-R;
  }
  for (int i=0; i<k; i++) 
    T[i] = lower_bound(R, R+m, T[i])-R;
}

void findl() {
  int M = 2*n+k+1;
  vector<int> m(M, -1);
  vector<int> l(M, -1);
  for (int j=0; j<k; j++) {
    m[T[j]+1] = j;
    for (int i=T[j]+1; i<M; i+=i&(-i))
      l[i] = max(l[i], j);
  }
  for (int i=0; i<n; i++) {
    int hi = max(A[i], B[i]);
    int lo = min(A[i], B[i])+1;
    L[i] = -1;
    while (hi >= lo) {
      if (hi-(hi&(-hi)) >= lo) {
        L[i] = max(L[i], l[hi]);
        hi -= hi&(-hi);
      } else {
        L[i] = max(L[i], m[hi--]);
      }
    }
  }
}

void findl2() {
  for (int i=0; i<n; i++) {
    L[i] = -1;
    for (int j=0; j<k; j++) 
      if (T[j] >= min(A[i], B[i]) && T[j] < max(A[i], B[i]))
        L[i] = j;
  }
}

void findc() {
  int m = 0;
  static P event[3*MAX];
  for (int i=0; i<k; i++) 
    event[m++] = {T[i], i+1};
  for (int i=0; i<n; i++)
    event[m++] = {max(A[i], B[i]), -i};
  sort(event, event+m, greater<P>());
  static int l[MAX+1];
  memset(l, 0, sizeof l);
  int tot = 0;
  for (int z=0; z<m; z++) {
    P& e = event[z]; 
    if (e.second > 0) {
      for (int i=e.second; i<=k; i+=i&(-i))
        l[i]++;
      tot++;
    } else {
      int j = -e.second;
      int s = 0;
      for (int i=L[j]+1; i>0; i-=i&(-i))
        s += l[i];
      C[j] = (tot-s)%2;
    }
  }
}

void findc2() {
  for (int i=0; i<n; i++) {
    C[i] = 0;
    for (int j=L[i]+1; j<k; j++) 
      if (T[j] >= max(A[i], B[i]))
        C[i] = 1-C[i];
  }
}

int main() {
  scanf("%d%d", &n, &k);
  for (int i=0; i<n; i++) 
    scanf("%d%d", &A[i], &B[i]);
  for (int i=0; i<k; i++) 
    scanf("%d", &T[i]);
  TIME("input");
  compress();
  TIME("compress");
  findl();
  TIME("findl");
  findc();
  TIME("findc");
  llint sol = 0;
  for (int i=0; i<n; i++) 
    if (L[i] == -1)
      sol += C[i]?R[B[i]]:R[A[i]];
    else
      sol += C[i]?min(R[A[i]], R[B[i]]):max(R[A[i]], R[B[i]]);
  printf("%lld\n", sol);
  return 0;
}
