#include <cstdio>
#include <algorithm>

using namespace std;

typedef long long ll;

const int LOG = 64, MAX = 3005;

int skok[LOG][MAX];
int rje[MAX];
int poc[MAX], p[MAX];
int n;
ll k;


void GenSkok()
{
  for (int i=1; i<=n; i++)
    skok[0][i] = p[i];

  for (int i=1; i<64; i++)
    for (int j=1; j<=n; j++)
      skok[i][j] = skok[i-1][skok[i-1][j]];

}

void Rijesi()
{
  int i=0, tmp=1;
  for (ll jos=k; jos; jos /= 2, i++)
    if (jos & 1)
      tmp = skok[i][tmp];

  rje[tmp]++;
}

int main()
{
  scanf("%d%lld", &n, &k);

  if (n > 3000)
      return 0;

  for (int i=1; i<=n; i++) {
    scanf("%d", &poc[i]);
    p[i] = poc[i];
  }

  for (int i=1; i<=n; i++) {
    for (int j=1; j<=n; j++) {
      p[i] = j;
      GenSkok();
      Rijesi();
      p[i] = poc[i];
    }
  }
  
  for (int i=1; i<=n; i++)
    printf("%d\n", rje[i]);

  return 0;
}
