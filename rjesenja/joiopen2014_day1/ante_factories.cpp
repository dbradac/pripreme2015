#include "factories.h"
#include <list>
#include <cstring>
#include <cstdio>
#include <vector>
#include <stack>
#include <algorithm>
#include <cassert>

using namespace std;

typedef long long L;
typedef pair<L, L> P;
typedef pair<L, L> Q;
typedef pair<L, P> T;

const int MAXN = 500000;
const int LG = 20;
const L INF = (1LL<<59)-1;

int n;
list<P> g[MAXN];
int parent[MAXN][LG];
int rtime;
int entry[MAXN];
int ext[MAXN];
int depth[MAXN];
L length[MAXN];

void dfs(int k, int p, int d, L l) {
  depth[k] = d;
  length[k] = l;
  entry[k] = rtime++;
  parent[k][0] = p;
  for (auto x : g[k])
    if (x.first != p)
      dfs(x.first, k, d+1, l+x.second);
  ext[k] = rtime++;
}

inline int below(int a, int b) {
  return entry[b] > entry[a] && ext[b] < ext[a];
}

int lca2(int a, int b) {
  if (depth[a] < depth[b])
    swap(a, b);
  for (int j = LG-1; j >= 0; --j)
    if (depth[a]-(1<<j) >= depth[b])
      a = parent[a][j];
  if (a == b) return a;
  for (int i=0; i<LG; i++) 
    for (int j = LG-1; j >= 0; --j)
    if (parent[a][j] != parent[b][j]) {
      a = parent[a][j], b = parent[b][j];
    }
   return parent[a][0];
}


int lca(int a, int b) {
  while (a != b) {
    if (depth[a] < depth[b])
      swap(a, b);
    a = parent[a][0];
  }
  return a;
}

void Init(int N, int A[], int B[], int D[]) {
  rtime = 0;
  n = N;
  for (int i=0; i<n; i++) 
    g[i].clear();
  for (int i=0; i<n-1; i++) {
    g[A[i]].push_back({B[i], D[i]});
    g[B[i]].push_back({A[i], D[i]});
  }
  dfs(0, -1, 0, 0);
  for (int j=1; j<LG; j++)
    for (int i=0; i<n; i++) {
      if (parent[i][j-1] == -1)
        parent[i][j] = -1;
      else
        parent[i][j] = parent[parent[i][j-1]][j-1];
  }
}

struct selem {
  int k;
  L cw;
  L cb;
};

struct stk {
  L sol;
  stack<selem> s;
  stk(): sol(INF) {}
  void do_push(int k, L cw, L cb) {
    //printf("Push %d\n", k);
    if (s.empty()) {
      s.push({k, cw, cb});
      return ;
    }
    if (k == s.top().k) {
      s.top().cw = min(s.top().cw, cw);
      s.top().cb = min(s.top().cb, cb);
      //printf("Merge\n"); 
      return ;
    }
    assert(below(s.top().k, k));
    s.push({k, cw, cb});
  }
  void pop() {
    //printf("Pop %d\n", s.top().k);
    //printf("Size %d\n", s.size());
    assert(!s.empty());
    L dist = length[s.top().k];
    L cw = s.top().cw;
    L cb = s.top().cb;
    sol = min(sol, cw+cb);
    s.pop();
    if (s.empty())
      return ;
    s.top().cw = min(s.top().cw, cw+dist-length[s.top().k]);
    s.top().cb = min(s.top().cb, cb+dist-length[s.top().k]);
  }

  void push(int k, int c) {
    selem last{-1, -1, -1};
    while (!s.empty() && !below(s.top().k, k)) {
      last = s.top();
      pop();
    }
    if (last.k != -1) {
      int x = lca2(last.k, k);
      L dist = length[last.k]-length[x];
      do_push(x, min(INF, last.cw+dist), min(INF, last.cb+dist));
    }
    do_push(k, c==0?0:INF, c==1?0:INF);
  }
  int empty() {
    return s.empty();
  }
};

long long Query(int s, int X[], int t, int Y[]) {
  vector<T> event;
  for (int i=0; i<s; i++) 
    event.push_back({entry[X[i]], {X[i], 0}});
  for (int i=0; i<t; i++) 
    event.push_back({entry[Y[i]], {Y[i], 1}});
  sort(event.begin(), event.end());
  stk v;
  for (int i=0; i<(int)event.size(); i++) 
    v.push(event[i].second.first, event[i].second.second);
  while (!v.empty())
    v.pop();
  return v.sol;
}
