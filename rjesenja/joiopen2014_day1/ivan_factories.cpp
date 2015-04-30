#include "factories.h"

#include <cstdio>
#include <algorithm>
#include <vector>
#include <stack>

#define FOR(a,b,c) for (int c = (a), _for = (b); c < _for; ++c)
#define REP(n) for (int _rep = 0, _for = (n); _rep < _for; ++_rep)
#define pb push_back
#define x first
#define y second
#define ll long long
#define pii pair < int, int >
#define pll pair < ll, ll >

using namespace std;

inline ll min(ll a, ll b){return (a < b) ? a : b;}
inline ll max(ll a, ll b){return (a > b) ? a : b;}
inline int max(int a, int b){return (a > b) ? a : b;}

class vektor{
public:
	int V[500005];
	int sz;
	vektor() : sz(0){}
	void push_back(int x){V[sz++] = x;}
	void pop_back(){--sz;}
	int size(){return sz;}
	void clear(){sz = 0;}
	int& operator[](int x){return V[x];}
	int* begin(){return V;}
	int* end(){return V + sz;}
};

const long long ool = 1000000000000000000LL;

int n;
vector < pii > E[500005];
//vector < int > ch[500005];
ll D[500005];
int lca[20][500005];
int dep[500005];
int disc[500005], fin[500005], cnt = 0;
void rootaj(int id){
	disc[id] = cnt++;
	for (auto x : E[id])
		if (x.x != lca[0][id]){
			//ch[id].pb(x.x);
			D[x.x] = D[id] + x.y;
			lca[0][x.x] = id;
			dep[x.x] = dep[id] + 1;
			rootaj(x.x);
		}
	fin[id] = cnt - 1;
}
int LCA(int a, int b){
	if (dep[a] < dep[b]) swap(a, b);
	int t = dep[a] - dep[b];
	FOR(0, 20, i) if ((t >> i) & 1) a = lca[i][a];
	for (int i = 19; i >= 0; --i) if (lca[i][a] != lca[i][b]) a = lca[i][a], b = lca[i][b];
	if (a != b) return lca[0][a];
	return a;
}
void Init(int N, int A[], int B[], int D[]) {
	n = N;
	FOR(0, n - 1, i) E[A[i]].pb({B[i], D[i]}), E[B[i]].pb({A[i], D[i]});
	rootaj(0);
	FOR(1, 20, i) FOR(0, n, j) lca[i][j] = lca[i - 1][lca[i - 1][j]];
}

bool cmp(int a, int b){return disc[a] < disc[b];}

int clr[500005];
vector < int > CH[500005];
bool used[500005];
vektor V;

//int ParentOpt[500005];

ll RES;
pll DFS2(int id){
	pll r = {ool, ool};
	if (clr[id] == 1) r.x = 0;
	if (clr[id] == 2) r.y = 0;
	for (auto x : CH[id]){
		pll tr = DFS2(x);
		ll d = D[x] - D[id];
		r.x = min(r.x, tr.x + d);
		r.y = min(r.y, tr.y + d);
	}
	RES = min(RES, r.x + r.y);
	return r;
}

stack < pii > S;
stack < int > Sid;

bool Contain(pii A, int X){return A.x <= X && X <= A.y;}

long long Query(int S, int X[], int T, int Y[]) {//if (n > 5000) return 0;
	//set(0, offset - 1, -5);
	//if (S + T <= n / 3){
	FOR(0, S, i){
		clr[X[i]] |= 1;
		if (!used[X[i]]) used[X[i]] = true, V.pb(X[i]);
	}
	FOR(0, T, i){
		clr[Y[i]] |= 2;
		if (!used[Y[i]]) used[Y[i]] = true, V.pb(Y[i]);
	}
	
	sort(V.begin(), V.end(), cmp);
	FOR(1, V.size(), i){
		int t = LCA(V[i - 1], V[i]);
		if (!used[t]) used[t] = true, V.pb(t);
		//if (t != V[i] && dep[t] > dep[ParentOpt[V[i]]]) ParentOpt[V[i]] = t;
		//if (t != V[i - 1] && dep[t] > dep[ParentOpt[V[i - 1]]]) ParentOpt[V[i - 1]] = t;
	}
	sort(V.begin(), V.end(), cmp);
	
	::S.push({disc[V[0]] + 1, fin[V[0]]});
	Sid.push(V[0]);
	FOR(1, V.size(), i){
		int x = V[i];
		while (!Contain(::S.top(), disc[x])) ::S.pop(), Sid.pop();
		CH[Sid.top()].pb(x);
		Sid.push(x);
		::S.push({disc[x] + 1, fin[x]});
	}
	
	RES = ool;
	DFS2(V[0]);
	
	for (auto x : V){
		//ParentOpt[x] = 0;
		used[x] = false;
		clr[x] = 0;
		CH[x].clear();
	} V.clear();
	
	return RES;

}
