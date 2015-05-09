#include <cstdio>
#include <algorithm>
#include <vector>

using namespace std;

typedef long long ll;

const int TOUR = 1<<19;
const ll INF = 1e18;

ll tleft[2*TOUR], tright[2*TOUR];
int a[TOUR], b[TOUR], c[TOUR], d[TOUR];
vector <int> V;

void Stavi(ll t[2*TOUR], int pos, ll val)
{
  for (t[pos+TOUR] = min(t[pos+TOUR], val), pos = (pos + TOUR) / 2; pos; pos /= 2)
    t[pos] = min(t[2*pos], t[2*pos+1]);
}

ll Vrati(ll t[2*TOUR], int pos, int lo, int hi, int begin, int end)
{
  if (lo >= end || hi <= begin)
    return INF;
  if (lo >= begin && hi <= end)
    return t[pos];
  return min(Vrati(t, 2*pos+0, lo, (lo+hi)/2, begin, end),
             Vrati(t, 2*pos+1, (lo+hi)/2, hi, begin, end) );
}

void Init()
{
  for (int i=0; i<2*TOUR; i++)
    tleft[i] = tright[i] = INF;
}

int main()
{
  int n, m;
  ll rje = INF;
  
  Init();
  scanf("%d%d", &m, &n);
  
  V.push_back(1);
  V.push_back(n);
  for (int i=0; i<m; i++) {
    scanf("%d%d%d%d", &a[i], &b[i], &c[i], &d[i]);
    V.push_back(a[i]);
    V.push_back(b[i]);
    V.push_back(c[i]); 
  }
  
  sort(V.begin(), V.end());

  for (int i=0; i<m; i++) {
    a[i] = lower_bound(V.begin(), V.end(), a[i]) - V.begin();
    b[i] = lower_bound(V.begin(), V.end(), b[i]) - V.begin();
    c[i] = lower_bound(V.begin(), V.end(), c[i]) - V.begin();
  }

  n = lower_bound(V.begin(), V.end(), n) - V.begin();
  Stavi(tleft, 0, 0);
  Stavi(tright, n, 0);

  for (int i=0; i<m; i++) {
    ll vl = Vrati(tleft, 1, 0, TOUR, a[i], TOUR);
    ll vr = Vrati(tright, 1, 0, TOUR, 0, b[i] + 1);
    rje = min(rje, d[i] + vl + vr);
    Stavi(tleft, c[i], vl + d[i]);
    Stavi(tright, c[i], vr + d[i]);
  }
  
  if (rje > 1e17)
    printf("-1\n");
  else
    printf("%lld\n", rje);

  return 0;
}
