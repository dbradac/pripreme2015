#include <cstdio>
#include <algorithm>
#include <vector>
#include <stack>

#define X first
#define Y second

using namespace std;

typedef long long ll;
typedef pair<int, ll> P;

const int LOG = 19, MAX = 1<<LOG;
const ll INF = 1e18;

struct str {
  int node;
  ll min0, min1;
  str (int node, ll min0, ll min1) : node(node), min0(min0), min1(min1) {}
  str () {}
};

int lca[MAX][LOG];
int dub[MAX];
int disc[MAX], fin[MAX];
int vr;
ll gore[MAX];
vector <P> V[MAX];
stack <str> S;

bool Manji(const str &a, const str &b)
{
  return disc[a.node] < disc[b.node];
}

void DfsPoc(int node, int prosli, ll prosval)
{
  dub[node] = dub[prosli] + 1;
  gore[node] = gore[prosli] + prosval;
  disc[node] = vr++;

  lca[node][0] = prosli;
  for (int i=1; i<LOG; i++)
    lca[node][i] = lca[lca[node][i-1]][i-1];

  for (int i=0; i<(int) V[node].size(); i++)
    if (V[node][i].X != prosli)
      DfsPoc(V[node][i].X, node, V[node][i].Y);

  fin[node] = vr++;
}

int Lca(int a, int b)
{
  if (dub[a] > dub[b])
    swap(a, b);
  
  int raz = dub[b] - dub[a];

  for (int i=0; raz; i++, raz /= 2)
    if (raz & 1)
      b = lca[b][i];

  if (a == b)
    return a;

  for (int i=LOG-1; i>=0; i--) {
    if (lca[a][i] != lca[b][i]) {
      a = lca[a][i];
      b = lca[b][i];
    }
  }

  return lca[a][0];
}

str Merge(str a, str b)
{
  int lc = Lca(a.node, b.node);
  return str(lc, min(INF, min(a.min0 + gore[a.node] - gore[lc], b.min0 + gore[b.node] - gore[lc])),
                 min(INF, min(a.min1 + gore[a.node] - gore[lc], b.min1 + gore[b.node] - gore[lc])) );
}

void Init(int n, int a[], int b[], int d[])
{
  for (int i=0; i<n-1; i++) {
    V[a[i]].push_back(P(b[i], d[i]));
    V[b[i]].push_back(P(a[i], d[i]));
  }

  DfsPoc(0, 0, 0);
}

ll Query(int s, int x[], int t, int y[])
{
  ll ret = INF;
  vector <str> T;

  for (int i=0; i<s; i++)
    T.push_back(str(x[i], 0, INF));

  for (int i=0; i<t; i++)
    T.push_back(str(y[i], INF, 0));

  sort(T.begin(), T.end(), Manji);
  
  str zadnji = str(-1, -1, -1);
  for (int i=0; i<(int) T.size(); i++) {
    for (;;) {
      str tmp = T[i];
      if (S.empty() || fin[S.top().node] > disc[tmp.node]) {
        if (zadnji.node != -1) {
          S.push(Merge(zadnji, tmp));
          zadnji = str(-1, -1, -1);
        }
        S.push(tmp);
        break;
      }
      else {
        zadnji = S.top();
        ret = min(ret, zadnji.min0 + zadnji.min1);
        S.pop();
        if (!S.empty()) {
          str tmp = S.top();
          S.pop();
          S.push(Merge(tmp, zadnji));
        }
      }
    }
  }

  for (; !S.empty(); ) {        
    zadnji = S.top();
    ret = min(ret, zadnji.min0 + zadnji.min1);
    S.pop();
    if (!S.empty()) {
      str tmp = S.top();
      S.pop();
      S.push(Merge(tmp, zadnji));
      str abc = S.top();
    }
  }
 
  return ret;
}
