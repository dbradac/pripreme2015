#include <cstdio>
#include <algorithm>
#include "factories.h"
#include <vector>
#include <set>
#include <queue>

#define X first
#define Y second

using namespace std;

typedef long long ll;
typedef pair<int, ll> P;
typedef pair<int, int> P2;

const int LOG = 19, MAX = 1<<LOG;
const ll INF = 1e18;

int br;
int dub[MAX];
int disc[MAX], vrijeme;
P lca[LOG][MAX];
vector <P> V[MAX];

struct str {
  int node;
  ll val[2];
  str (int _n, ll v0, ll v1) {
    node = _n;
    val[0] = v0;
    val[1] = v1;
  }
  str () {}
};

bool operator < (const str &a, const str &b)
{
  return disc[a.node] < disc[b.node];
}

set <str> SN;
priority_queue <P2> SUd;

void DfsPoc(int node, int prosli, int c)
{
  dub[node] = dub[prosli] + 1;
  disc[node] = ++vrijeme;

  lca[0][node] = P(prosli, c);
  for (int i=1; i<LOG; i++)
    lca[i][node] = P(lca[i-1][lca[i-1][node].X].X, lca[i-1][lca[i-1][node].X].Y + lca[i-1][node].Y);

  for (int i=0; i<(int) V[node].size(); i++)
    if (V[node][i].X != prosli)
      DfsPoc(V[node][i].X, node, (int) V[node][i].Y);
}

void Init(int N, int A[], int B[], int D[]) {
  for (int i=0; i<N-1; i++) {
    V[A[i]].push_back(P(B[i], D[i]));
    V[B[i]].push_back(P(A[i], D[i]));
  }
  
  DfsPoc(0, 0, 0);
}

P Lca(int a, int b)
{
  if (dub[a] > dub[b])
    swap(a, b);

  int raz = dub[b] - dub[a];
  ll ret = 0;

  for (int i=0; raz; i++, raz /= 2) {
    if (raz & 1) {
      P t = lca[i][b];
      ret += t.Y;
      b = t.X;
    }
  }

  if (a == b)
    return P(a, ret);

  for (int i=LOG-1; i>=0; i--) {
    if (lca[i][a].X != lca[i][b].X) {
      P ta = lca[i][a], tb = lca[i][b];
      ret += ta.Y + tb.Y;
      a = ta.X;
      b = tb.X;
    }
  }
  
  return P(lca[0][a].X, ret + lca[0][a].Y + lca[0][b].Y);
}

void Stavi(const set <str> :: iterator &it)
{
  set <str> :: iterator it2 = it;

  if (it != SN.begin()) {
    it2 = it; it2--;
    P lc = Lca(it->node, it2->node);
    SUd.push(P2(dub[lc.X], it2->node));
  }
  it2 = it; it2++;
  if (it2 != SN.end()) {
    P lc = Lca(it->node, it2->node);
    SUd.push(P2(dub[lc.X], it->node));
  }
}

ll Rijesi()
{
  ll ret = INF;

  for (set <str> :: iterator it1 = SN.begin(); it1 != SN.end(); it1++) {
    set <str> :: iterator it2 = it1;
    it2++;
    if (it2 != SN.end()) {
      P lc = Lca(it1->node, it2->node);
      SUd.push(P2(dub[lc.X], it1->node));
    }
  }

  for (; SN.size() > 1; ) {
    P2 tmp = SUd.top();
    SUd.pop();
  
   // printf("SZ %u %u\n", SN.size(), SUd.size());

  
    set <str> :: iterator it1 = SN.find(str(tmp.Y, 0, 0));
    set <str> :: iterator it2 = it1; it2++;

    if (it1 == SN.end() || it2 == SN.end())
      continue;

    P lc = Lca(it1->node, it2->node);
    if (dub[lc.X] != tmp.X)
      continue;
    ret = min(ret, it1->val[0] + it2->val[1] + lc.Y);
    ret = min(ret, it1->val[1] + it2->val[0] + lc.Y);
    
    ll dist1 = Lca(it1->node, lc.X).Y;
    ll dist2 = Lca(it2->node, lc.X).Y;
    ll v0 = min(dist1 + it1->val[0], dist2 + it2->val[0]);
    ll v1 = min(dist1 + it1->val[1], dist2 + it2->val[1]);

    set <str> :: iterator it = SN.find(str(lc.X, 0, 0));

    if (it == SN.end()) {
      SN.erase(it1);
      SN.erase(it2);
      SN.insert(str(lc.X, min(INF, v0), min(INF, v1)));
    }
    else {
      ret = min(ret, v0 + it->val[1]);
      ret = min(ret, v1 + it->val[0]);
      str novi = str(lc.X, min(INF, min(v0, it->val[0])), min(INF, min(v1, it->val[1])));
      int da = 0;
      if (it != it1 && it != it2)
        da = 1;

      SN.erase(it1);
      SN.erase(it2);
      if (da)
        SN.erase(it);      

      /*int iii=0;
      for (set <str> :: iterator itt = SN.begin(); itt != SN.end(); itt++, iii++) {
          set <str> :: iterator asd = itt;
          asd++;
          if (asd == itt)
            printf("AAA\n");
//        printf("II %d\n", iii);
  //      printf("ITTT %d %d   %lld %lld\n", ittt->node, disc[ittt->node], ittt->val[0], ittt->val[1]);
      }

      printf("DDD\n");
      printf("INSB %d %d\n", novi.node, disc[novi.node]);      */
      SN.insert(novi);
    }

    Stavi(SN.find(str(lc.X, 0, 0)));
  }

  return ret;
}

long long Query(int S, int X[], int T, int Y[]) {
  SN.clear();
  for (; !SUd.empty(); )
    SUd.pop();

  for (int i=0; i<S; i++) {
//    printf("INSX %d %d\n", X[i], disc[X[i]]);
    SN.insert(str(X[i], 0, INF));
  }

  for (int i=0; i<T; i++) {
  //  printf("INSY %d %d\n", Y[i], disc[Y[i]]);
    SN.insert(str(Y[i], INF, 0));
  }

//  printf("DAAAA\n");

  return Rijesi();
}

