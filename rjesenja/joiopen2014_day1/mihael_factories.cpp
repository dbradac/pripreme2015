#include "factories.h"

#include <algorithm>
#include <cstdio>
#include <map>
#include <queue>
#include <vector>

using namespace std;

const int MAXN = 500001;

int root;
int cent_par[MAXN];
vector<long long> M[MAXN];

int num[MAXN];
int maksi[MAXN];
int cent[MAXN];
vector<pair<int,long long> > V[MAXN];
vector<int> L;

vector<int> curr_st;
vector<int> counter_st;
vector<int> par_st;

inline long long max(long long a, long long b) {
  return a>b?a:b;
}

inline long long min(long long a, long long b) {
  return a>b?b:a;
}

void DFS1(int _curr, int _par = -1) {
//  printf("%d %d\n", _curr, _par);

  curr_st.push_back(_curr);
  par_st.push_back(_par);
  counter_st.push_back(-1);
  int curr, par;
  while (!curr_st.empty()) {
    curr = curr_st.back();
    par = par_st.back();
//    printf("%d %d\n", curr, par);
    if (counter_st.back() == -1) {
      L.push_back(curr);
      num[curr] = 1;
      maksi[curr] = 0;
    } else {
      num[curr] += num[V[curr][counter_st.back()].first];
      maksi[curr] = max(maksi[curr], num[V[curr][counter_st.back()].first]);
    }
    counter_st.back()++;

    for (; counter_st.back() < V[curr].size(); counter_st.back()++) {
      if (V[curr][counter_st.back()].first == par) continue;
      if (cent[V[curr][counter_st.back()].first]) continue;
      par_st.push_back(curr);
      curr_st.push_back(V[curr][counter_st.back()].first);
//      printf("%d\n", V[curr][counter_st.back()].first);
      break;
    }
    if (V[curr].size() == counter_st.back()) {
      counter_st.pop_back();
      par_st.pop_back();
      curr_st.pop_back();
    } else counter_st.push_back(-1);
  }

//  L.push_back(curr);
//  num[curr] = 1;
//  maksi[curr] = 0;
//  for (counter.push_back(0); counter.back() < V[curr].size(); counter.back()++) {
//    if (V[curr][counter.back()].first == par) continue;
//    if (cent[V[curr][counter.back()].first]) continue;
//    DFS1(V[curr][counter.back()].first, curr);
//    num[curr] += num[V[curr][counter.back()].first];
//    maksi[curr] = max(maksi[curr], num[V[curr][counter.back()].first]);
//  }
//  counter.pop_back();
  return;
}

queue<pair<int,int> > Q;

void DFS2(int _curr, int tr_cent, long long _dist = 0, int _par = -1) {

  Q.push(make_pair(_curr, _par));
  M[tr_cent].push_back(0);
  pair<int,int> curr;
  int next;
  while (!Q.empty()) {
    curr = Q.front(); Q.pop();
    for (int i = 0; i < V[curr.first].size(); i++) {
      next = V[curr.first][i].first;
      if (next == curr.second) continue;
      if (cent[next]) continue;
      M[next].push_back(M[curr.first].back() + V[curr.first][i].second);
      Q.push(make_pair(next, curr.first));
    }
  }



//  M[tr_cent][curr] = dist;
//  for (int i = 0; i < V[curr].size(); i++) {
//    pair<int,long long> next = V[curr][i];
//    if (next.first == par) continue;
//    if (cent[next.first]) continue;
//    DFS2(next.first, tr_cent, dist + next.second, curr);
//  }
}


int decomp(int curr) {
  L.clear();
  DFS1(curr);
  int sz = num[curr];
  int next;
  for (int i = 0; i < L.size(); i++) {
    next = L[i];
    maksi[next] = max(maksi[next], sz - num[next]);
  }

  int tr_cent = curr;
  for (int i = 0; i < L.size(); i++) {
    next = L[i];
    if (maksi[next] < maksi[tr_cent]) tr_cent = next;
  }

  DFS2(tr_cent, tr_cent);

//  printf("Trenutni cent %d\n", tr_cent);
//  for (int i = 0; i < M[tr_cent].size(); i++) {
//    printf("%d ", M[tr_cent][i]);
//  }
//  printf("\n");

  cent[tr_cent] = 1;
  pair<int, long long> next2;
  for (int i = 0; i < V[tr_cent].size(); i++) {
    pair<int,long long> next2 = V[tr_cent][i];
    if (cent[next2.first]) continue;
    cent_par[decomp(next2.first)] = tr_cent;
  }

  return tr_cent;
}


void Init(int N, int A[], int B[], int D[]) {
//  printf("INIT\n");
  for (int i = 0; i < N - 1; i++) {
    V[A[i]].push_back(make_pair(B[i], D[i]));
    V[B[i]].push_back(make_pair(A[i], D[i]));
  }

  root = decomp(0);
  cent_par[root] = root;
//  printf("INIT gotov\n");
}

long long best_dist[MAXN];
int bio_dist[MAXN];
long long color = 0;

long long Query(int S, int X[], int T, int Y[]) {
  color++;
//  printf("QUERY\n");
  int curr, dub;
  for (int i = 0; i < S; i++) {
    curr = X[i];
    dub = M[curr].size() - 1;
//    printf("%d\n", curr);

    bio_dist[curr] = color;
    best_dist[curr] = 0;
//    printf("Best distance to %d is %d\n", curr, best_dist[curr]);
    while (curr != root) {
      curr = cent_par[curr];
      dub--;
      if (bio_dist[curr] != color) {
        bio_dist[curr] = color;
        best_dist[curr] = M[X[i]][dub];
      } else {
        best_dist[curr] = min( best_dist[curr], M[X[i]][dub] );
      }
//      printf("Best distance to %d is %d\n", curr, best_dist[curr]);
    }
  }

  long long sol = 1e18;
  for (int i = 0; i < T; i++) {
    curr = Y[i];
    dub = M[curr].size() - 1;
    if (bio_dist[curr] == color) sol = min(sol, best_dist[curr] + M[Y[i]][dub]);
    while (curr != root) {
      curr = cent_par[curr];
      dub--;
      if (bio_dist[curr] == color) sol = min(sol, best_dist[curr] + M[Y[i]][dub]);
//      printf("Current best dist is %lld, %d, %lld\n", sol, curr, M[Y[i]][dub+1]);
    }
  }
//  printf("QUERY gotov\n");

  return sol;
}
