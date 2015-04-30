#include <cstdio>
#include <algorithm>
#include <vector>
#include <map>

#define FOR(a,b,c) for (int c = (a), _for = (b); c < _for; ++c)
#define REP(n) for (int _rep = 0, _for = (n); _rep < _for; ++_rep)
#define pii pair < int, int >
#define pb push_back
#define x first
#define y second
#define ll long long

using namespace std;

const int offset = (1 << 20);
int tur[offset * 2];
void set(int id, int lo, int hi, int pos, int val){
	if (lo > pos || hi < pos) return;
	if (lo == hi){tur[id] = val; return;}
	int mid = (lo + hi) / 2;
	set(id * 2, lo, mid, pos, val);
	set(id * 2 + 1, mid + 1, hi, pos, val);
	tur[id] = max(tur[id * 2], tur[id * 2 + 1]);
}
void set(int pos, int val){set(1, 0, offset - 1, pos, val);}
int query(int id, int lo, int hi, int slo, int shi){
	if (lo > shi || hi < slo) return -1;
	if (lo >= slo && hi <= shi) return tur[id];
	int mid = (lo + hi) / 2;
	return max(query(id * 2, lo, mid, slo, shi), query(id * 2 + 1, mid + 1, hi, slo, shi));
}
int query(int lo, int hi){return query(1, 0, offset - 1, lo, hi);}

int loga[1000005];
void add(int p){ ++p; //printf(" a %d\n", p);
	for (; p <= 1000000; p += p & -p)
		++loga[p];
}
int sum(int p){ ++p; //printf(" s %d\n", p);
	int r = 0;
	for (; p; p -= p & -p)
		r += loga[p];
	return r;
}

const int oo = 1000000000;

int n, k;
int a[200005], b[200005];
int t[200005];
int ch[200005];

vector < int > V;
map < int, int > M;
int cnt;

int main(){
	scanf("%d%d", &n, &k);
	FOR(0, n, i) scanf("%d%d", a + i, b + i);
	FOR(0, k, i) scanf("%d", t + i);
	
	FOR(0, n, i)
		if (a[i] < b[i])
			swap(a[i], b[i]), ch[i] = 1;
	
	FOR(0, n, i) V.pb(a[i]), V.pb(b[i]);
	FOR(0, k, i) V.pb(t[i]);
	sort(V.begin(), V.end());
	for (auto x : V){
		if (M.count(x)) continue;
		M[x] = cnt;
		V[cnt] = x;
		++cnt;
	}
	
	FOR(0, n, i) a[i] = M[a[i]], b[i] = M[b[i]];
	FOR(0, k, i) t[i] = M[t[i]];
	
	vector < pii > sweep;
	
	FOR(0, k, i) set(t[i], i + 1);
	FOR(0, n, i){
		int t = query(b[i], a[i] - 1);
		sweep.pb({t, i});
	}
	
	sort(sweep.begin(), sweep.end());
	reverse(sweep.begin(), sweep.end());
	int pt = k - 1;
	for (auto x : sweep){
		while (pt >= x.x) add(t[pt--]);
		if (x.x) ch[x.y] = 0;
		ch[x.y] += sum(1000000) - sum(b[x.y] - 1);
	}
	
	long long S = 0;
	FOR(0, n, i) if (ch[i] & 1) S += V[b[i]]; else S += V[a[i]];
	printf("%lld\n", S);
	
	return 0;
}

