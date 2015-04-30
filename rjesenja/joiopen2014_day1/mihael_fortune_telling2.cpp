#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 200010;
const int MAXK = 200010;

class T {
 public:
  long long t;
  int index;

  T(long long _t, int _index) {
    t = _t;
    index = _index;
  }
};

int n, k;
long long a[MAXN], b[MAXN];
vector<T> V;

int tur[1<<19];
vector<int> tur2[1<<19];
int off = (1<<18);

bool cmp(const T &t1, const T &t2) {
  if (t1.t != t2.t) return t1.t < t2.t;
  return t1.index < t2.index;
}

void update(int l = 0, int r = off, int curr = 1) {
  if (l + 1 == r) return;
  int mid = (l + r) / 2;
  update(l, mid, curr * 2); update(mid, r, curr * 2 + 1);
  tur[curr] = max(tur[curr*2], tur[curr*2+1]);
  tur2[curr].resize(tur2[curr*2].size() + tur2[curr*2+1].size());
  merge(tur2[curr*2].begin(), tur2[curr*2].end(), tur2[curr*2+1].begin(), tur2[curr*2+1].end(), tur2[curr].begin());
}

int query(int lo, int hi, int l = 0, int r = off, int curr = 1) {
  if (l >= hi || r <= lo) return 0;
  if (l >= lo && r <= hi) return tur[curr];
  int mid = (l + r) / 2;
  return max(query(lo, hi, l, mid, curr * 2), query(lo, hi, mid, r, curr * 2 + 1));
}

int query2(int lo, int hi, long long val, int l = 0, int r = off, int curr = 1) {
  if (l >= hi || r <= lo) return 0;
  if (l >= lo && r <= hi) {
    int tmp = lower_bound(tur2[curr].begin(), tur2[curr].end(), val) - tur2[curr].begin();
    return tur2[curr].size() - tmp;
  }
  int mid = (l + r) / 2;
  return query2(lo, hi, val, l, mid, curr * 2) + query2(lo, hi, val, mid, r, curr * 2 + 1);
}

int main() {
  scanf("%d%d", &n, &k);
  for (int i = 0; i < n; i++) {
    scanf("%lld%lld", &a[i], &b[i]);
  }
  for (int i = 0; i < k; i++) {
    int tmp;
    scanf("%lld", &tmp);
    V.push_back(T(tmp, i+1));
  }

  sort(V.begin(), V.end(), cmp);

  for (int i = 0; i < k; i++) {
    tur[off + i] = V[i].index;
    tur2[off + i].push_back(V[i].index);
  }
  update();

  long long sol = 0;
  for (int i = 0; i < n; i++) {
//    printf("%d %d:\n", a[i], b[i]);
    int lo = lower_bound(V.begin(), V.end(), T(min(a[i],b[i]), 0), cmp) - V.begin();
    int hi = lower_bound(V.begin(), V.end(), T(max(a[i],b[i]), 0), cmp) - V.begin();
//    printf("%d %d\n", lo, hi);
    int base = 0;
    if (lo < hi) {
      base = query(lo, hi);
    }
//    printf("base = %d\n", base);
    int sw_num = query2(hi, off, base);
//    printf("broj zamjena = %d\n", sw_num);
    if (base == 0) {
      if (sw_num % 2 == 0) sol += a[i]; else sol += b[i];
    } else {
      if (sw_num % 2 == 0) sol += max(a[i], b[i]); else sol += min(a[i], b[i]);
    }
  }

  printf("%lld\n", sol);

  return 0;
}
