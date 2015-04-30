#include <cstdio>
#include <algorithm>
#include <vector>

#define FOR(a,b,c) for (int c = (a), _for = (b); c < _for; ++c)
#define REP(n) for (int _rep = 0, _for = (n); _rep < _for; ++_rep)
#define pb push_back
#define x first
#define y second
#define ll long long

using namespace std;

int n;
long long k;
int a[200005];

int lca[20][200005];
int next(int a, int k){
	FOR(0, 20, i) if ((k >> i) & 1) a = lca[i][a];
	return a;
}

int dis[200005];
bool acc[200005];
ll sol[200005];
void Solve3(){
	FOR(1, n + 1, i) lca[0][i] = a[i];
	FOR(1, 20, i) FOR(1, n + 1, j) lca[i][j] = lca[i - 1][lca[i - 1][j]];
	int Cnt = 0, Curr = 1;
	do {
		dis[Curr] = Cnt;
		acc[Curr] = true;
		++Cnt;
		Curr = a[Curr];
	} while (Curr != 1);
	FOR(1, Cnt + 1, T){
		FOR(0, Cnt / T + 1, i){
			// povecaj (k % T + i * T)-ti sljedbenik za T najcesce
			if (i != Cnt / T) sol[next(1, k % T + i * T)] += T;
			else sol[next(1, k % T - T + Cnt)] += Cnt % T;
		}
	}
	sol[next(1, k % Cnt)] += (ll)n * (ll)(n - Cnt);
	FOR(1, n + 1, i){
		if (!acc[i]) printf("%d\n", Cnt);
		else printf("%lld\n", sol[i]);
	}
}

bool ch[200005];
int main(){
	scanf("%d%lld", &n, &k);
	FOR(1, n + 1, i) scanf("%d", a + i), ch[a[i]] = true;
	int t = 0;
	FOR(1, n + 1, i) t += ch[i];
	if (t == n){Solve3(); return 0;}
	return 0;
}

