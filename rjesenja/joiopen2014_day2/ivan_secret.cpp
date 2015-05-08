#include "secret.h"

#include <cstdio>
#include <algorithm>
#include <vector>
#include <assert.h>

#define FOR(a,b,c) for (int c = (a), _for = (b); c < _for; ++c)
#define REP(n) for (int _rep = 0, _for = (n); _rep < _for; ++_rep)
#define pb push_back
#define x first
#define y second
#define ll long long
#define pii pair < int, int >
#define all(x) (x).begin(), (x).end()

using namespace std;

int n;
int a[1005];
int mat[1005][1005];
int m2[1005][1005];
int d[1005];

void Build(int l, int r, int d){
	if (r - l <= 1) return;
	int m = (l + r) / 2;
	::d[m] = d;
	mat[m][m] = a[m];
	FOR(m + 1, r + 1, i) mat[i][m] = mat[m][i] = Secret(mat[i - 1][m], a[i]);
	m2[m][m - 1] = m2[m - 1][m] = a[m - 1];
	for (int i = m - 2; i >= l; --i) m2[i][m] = m2[m][i] = Secret(a[i], m2[i + 1][m]);
	Build(l, m - 1, d + 1);
	Build(m + 1, r, d + 1);
}

void Init(int N, int A[]) {
	n = N;
	FOR(0, n, i) a[i] = A[i];
	FOR(0, n, i) FOR(0, n, j) mat[i][j] = m2[i][j] = -1;
	FOR(0, n, i) d[i] = 1000000000;
	Build(0, n - 1, 0);
}

int Query(int L, int R) {
	if (L == R) return a[L];
	if (R - L == 1) return Secret(a[L], a[R]);
	int T = L;
	FOR(L, R + 1, i) if (d[i] < d[T]) T = i;
	if (T == L) return mat[T][R];
	return Secret(m2[L][T], mat[T][R]);
}
