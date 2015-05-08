#include <cstdio>
#include <algorithm>
#include <vector>

#define FOR(a,b,c) for (int c = (a), _for = (b); c < _for; ++c)
#define REP(n) for (int _rep = 0, _for = (n); _rep < _for; ++_rep)
#define pb push_back
#define x first
#define y second
#define ll long long
#define pii pair < int, int >
#define all(x) (x).begin(), (x).end()

using namespace std;

const int oo = 1000000000;
const ll ool = (ll)oo * oo;

class Turnament{
public:
	static const int offset = 1 << 20;
	vector < ll > V;
	Turnament(){V.resize(offset * 2, ool);}
	ll TurnamentMin(int id, int lo, int hi, int slo, int shi){
		if (lo > shi || hi < slo) return ool;
		if (lo >= slo && hi <= shi) return V[id];
		int mid = (lo + hi) / 2;
		return ::min(TurnamentMin(id * 2, lo, mid, slo, shi), TurnamentMin(id * 2 + 1, mid + 1, hi, slo, shi));
	}
	ll min(int lo, int hi){return TurnamentMin(1, 0, offset - 1, lo, hi);}
	void TurnamentSet(int id, int lo, int hi, int pos, ll val){
		if (lo > pos || hi < pos) return;
		if (lo == hi){V[id] = ::min(V[id], val); return;}
		int mid = (lo + hi) / 2;
		TurnamentSet(id * 2, lo, mid, pos, val);
		TurnamentSet(id * 2 + 1, mid + 1, hi, pos, val);
		V[id] = ::min(V[id * 2], V[id * 2 + 1]);
	}
	void set(int pos, ll val){TurnamentSet(1, 0, offset - 1, pos, val);}
} T1, T2;

int m, n;
int a[100005], b[100005], c[100005], d[100005];

int main(){
	scanf("%d%d", &m, &n);
	FOR(0, m, i) scanf("%d%d%d%d", a + i, b + i, c + i, d + i);
	
	vector < int > Saz = {1, n};
	FOR(0, m, i) Saz.pb(a[i]), Saz.pb(b[i]), Saz.pb(c[i]);
	sort(all(Saz));
	int T = unique(all(Saz)) - Saz.begin();
	while ((int)Saz.size() != T) Saz.pop_back();
	FOR(0, m, i){
		a[i] = lower_bound(all(Saz), a[i]) - Saz.begin();
		b[i] = lower_bound(all(Saz), b[i]) - Saz.begin();
		c[i] = lower_bound(all(Saz), c[i]) - Saz.begin();
	}
	
	ll R = ool;
	T1.set(0, 0);
	T2.set(Saz.size() - 1, 0);
	FOR(0, m, i){
		ll A = d[i] + T1.min(a[i], b[i]);
		ll B = d[i] + T2.min(a[i], b[i]);
		R = min(R, A + B - d[i]);
		T1.set(c[i], A);
		T2.set(c[i], B);
	}
	
	if (R >= ool) printf("-1\n"); else printf("%lld\n", R);
	
	return 0;
}
