#include <cstdio>
#include <algorithm>
#include <vector>

#define X first
#define Y second

using namespace std;

typedef long long ll;
typedef pair<int, int> P;

const int TOUR = 1<<20;

struct event {
  int val, stanje, ind;
  event (int val, int stanje, int ind) : val(val), stanje(stanje), ind(ind) {}
  event () {}
};

bool operator < (const event &a, const event &b)
{
  if (a.val != b.val)
    return a.val > b.val;
  return a.stanje > b.stanje;
}

int p[TOUR];
int a[TOUR], b[TOUR], st[TOUR];
int fen[TOUR];
int val[3*TOUR];
vector <event> E;
vector <int> V;
P t[2*TOUR];

void StaviT(int pos, P val)
{
  for (t[pos+TOUR] = val, pos = (pos + TOUR) / 2; pos; pos /= 2)
    t[pos] = max(t[2*pos], t[2*pos+1]);
}

P Desni(int pos, int lo, int hi, int val)
{
  if (lo + 1 == hi)
    return t[pos];
  if (t[2*pos+1].X >= val)
    return Desni(2*pos+1, (lo+hi)/2, hi, val);
  return Desni(2*pos, lo, (lo+hi)/2, val);
}

void StaviF(int pos, int val)
{
  for (pos++; pos<TOUR; pos += pos & -pos)
    fen[pos] ^= val;
}

int VratiF(int pos)
{
  int ret=0;

  for (pos++; pos; pos -= pos & -pos)
    ret ^= fen[pos];

  return ret;
}

int main()
{
  int n, q;
  ll rje=0;

  scanf("%d%d", &q, &n);

  for (int i=0; i<q; i++) {
    scanf("%d%d", &a[i], &b[i]);
    V.push_back(a[i]);
    V.push_back(b[i]);
  }

  for (int i=0; i<n; i++) {
    scanf("%d", &p[i]);
    V.push_back(p[i]);
  }

  sort(V.begin(), V.end());

  for (int i=0; i<q; i++) {
    int pa = lower_bound(V.begin(), V.end(), a[i]) - V.begin() + 1;
    int pb = lower_bound(V.begin(), V.end(), b[i]) - V.begin() + 1;
    val[pa] = a[i];
    val[pb] = b[i];
    a[i] = pa;
    b[i] = pb;

    if (a[i] > b[i]) {
      swap(a[i], b[i]);
      st[i] = 1;
    }
    E.push_back(event(b[i], 0, i));
  }

  for (int i=0; i<n; i++) {
    p[i] = lower_bound(V.begin(), V.end(), p[i]) - V.begin() + 1;
    E.push_back(event(p[i], 1, i));
    StaviT(i, P(p[i], i));
  }

  sort(E.begin(), E.end());
  
  for (int i=0; i<(int) E.size(); i++) {
    event tmp = E[i];
    if (!tmp.stanje) {
      P des = Desni(1, 0, TOUR, a[tmp.ind]);
      int f=0;
      
      if (des.X < a[tmp.ind] || des.X > b[tmp.ind]) {
        f = VratiF(TOUR-5);
        if (f ^ st[tmp.ind])
         rje += val[b[tmp.ind]];
        else
          rje += val[a[tmp.ind]];
      }
      else {
        f = VratiF(TOUR-5) ^ VratiF(des.Y);

        if (!f)
          rje += val[b[tmp.ind]];
        else
          rje += val[a[tmp.ind]];
      }
    }
    else {
      StaviT(tmp.ind, P(0, i));
      StaviF(tmp.ind, 1);
    }
  }

  printf("%lld\n", rje);

  return 0;
}
