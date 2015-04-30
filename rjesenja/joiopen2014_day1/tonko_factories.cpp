#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <vector>
#include <queue>
using namespace std;

#include "factories.h"

typedef long long llint;

const int MAXN = 500010;
const int MAXQ = 100010;
const int LOGMAXN = 20;

int N;
vector<pair<int, int>> V[MAXN];

int dad[MAXN][LOGMAXN];
int dep[MAXN];
llint sum[MAXN];

inline llint Min(llint x, llint y) { return y + ((x-y) & ((x-y) >> 63));  }

/// BEGIN LCA
void build_lca(int curr, int prev, int s) {
  dep[curr] = dep[prev] + 1;
  sum[curr] = sum[prev] + s;
  dad[curr][0] = prev;
  for (int i = 1; i < 19; ++i)
    dad[curr][i] = dad[dad[curr][i-1]][i-1];
  for (auto nxt: V[curr]) {
    if (nxt.first == prev) continue;
    build_lca(nxt.first, curr, nxt.second);
  }
}

int lca(int u, int v) {
  if (dep[u] < dep[v]) swap(u, v);
  for (int i = 18; i >= 0; --i)
    if (dep[u] - (1 << i) >= dep[v])
      u = dad[u][i];
  if (u == v) return u;
  for (int i = 18; i >= 0; --i) {
    if (dad[u][i] == dad[v][i]) continue;
    u = dad[u][i];
    v = dad[v][i];
  }
  return dad[u][0];
}

inline llint dist(int u, int v) {
  return sum[u] + sum[v] - 2*sum[lca(u, v)];
}
/// END LCA


/// BEGIN C DECOMPOSITION

int test_cnt = 0;

int val[MAXN];
int parent[MAXN];
bool flag[MAXN];
vector<int> tmp;

int dfs(int x, int prev) {
  if (flag[x]) return 0;
  val[x] = 1;
  tmp.push_back(x);
  for (auto y: V[x]) {
    if (y.first == prev) continue;
    val[x] += dfs(y.first, x); 
  }
  return val[x];
}

bool check_pivot(int x, int cnt) {
  for (auto y: V[x]) {
    int t = val[y.first];
    if (val[y.first] > val[x]) t = cnt - val[x];
    if (2*t > cnt) return false;
  }
  return true;
}

int find_pivot(int cnt) {
  for (int x: tmp) {
    if (check_pivot(x, cnt))
      return x;
  }
  return tmp[0];
}

void clear() {
  for (int x: tmp) val[x] = 0;
  tmp.clear();
}

void decompose(int x, int prev) {
  clear();
  int cnt = dfs(x, -1);
  int piv = find_pivot(cnt);
  flag[piv] = true;
  parent[piv] = prev;
  for (auto y: V[piv]) {
    if (flag[y.first]) continue;
    decompose(y.first, piv);
  }
}

/// END DECOMPOSITION


/// BEGIN OSTALO

const llint inf = 2000000000LL * 1000000000;

llint DIST[MAXN][LOGMAXN];
llint pq[MAXN];

vector<int> history;

int hcnt=0;

void set(int x) {
  int t = 0;
  for (int i = x; i != -1; i = parent[i]) {
    history[hcnt++] = i;
    pq[i] = Min(pq[i], DIST[x][t]);
    ++t;
  }
}

llint get(int x) {
  llint ret = inf;
  int t = 0;
  for (int i = x; i != -1; i = parent[i]) {
    ret = Min(ret, DIST[x][t] + pq[i]);
    ++t;
  }
  return ret;
}

/// END OSTALO

void Init(int n, int a[], int b[], int d[]) {
  N = n;
  for (int i = 0; i < N-1; ++i) {
    V[a[i]].push_back({b[i], d[i]});
    V[b[i]].push_back({a[i], d[i]});
  }
  for (int i = 0; i < N; ++i)
    pq[i] = inf;
  dep[0] = -1;
  build_lca(0, 0, 0);
  decompose(0, -1);
  for (int i = 0; i < N; ++i) {
    int t = 0;
    for (int j = i; j != -1; j = parent[j]) {
      DIST[i][t] = dist(i, j);
      ++t;
    } 
  }
}

llint Query(int S, int X[], int T, int Y[]) {
  llint ans = inf;
 // return ans;
//  for (int i = 0; i < S; ++i)
//    for (int j = 0; j < T; ++j)
//      ans = min(ans, dist(X[i], Y[j]));
  
  for (int i = 0; i < hcnt; ++i) pq[history[i]]=inf;
  hcnt = 0;
  history.clear();
  history.resize(S*LOGMAXN);


if (S < T) {
  for (int i = 0; i < S; ++i) set(X[i]);
  for (int i = 0; i < T; ++i) ans = Min(ans, get(Y[i]));
} else {
  for (int i = 0; i < T; ++i) set(Y[i]);
  for (int i = 0; i < S; ++i) ans = Min(ans, get(X[i]));
}

  return ans;
}

