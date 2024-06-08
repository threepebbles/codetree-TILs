#include <cstdio>
#include <algorithm>
#include <queue>
using namespace std;

const int MAXN = 8, MAXM = 8;
const int dr[] = {-1, 0, 1, 0};
const int dc[] = {0, 1, 0, -1};

typedef struct V {
	int r, c;
	V() {};
	V(int _r, int _c) :r(_r), c(_c) {}
};

int n, m;
int brd[MAXN][MAXM];
int ans;

int get_value_in_brd(vector<int>& pos) {
	int sum = 0;
	for (int i = 0; i < 3; i++) {
		int r = pos[i] / m;
		int c = pos[i] % m;
		brd[r][c] = 1;
	}

	bool visited[MAXN][MAXM] = {};
	queue<V> q;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (brd[i][j] == 2) {
				q.push(V(i, j));
			}
		}
	}


	while (!q.empty()) {
		V cur = q.front();
		q.pop();
		if (visited[cur.r][cur.c]) continue;
		visited[cur.r][cur.c] = true;

		for (int d = 0; d < 4; d++) {
			int nr = cur.r + dr[d];
			int nc = cur.c + dc[d];
			if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;
			if (brd[nr][nc] == 1) continue;
			if (visited[nr][nc]) continue;

			q.push(V(nr, nc));
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (visited[i][j]) sum++;
			else if (brd[i][j] == 1) sum++;
		}
	}


	for (int i = 0; i < 3; i++) {
		int r = pos[i] / m;
		int c = pos[i] % m;
		brd[r][c] = 0;
	}
	return n * m - sum;
}

void dfs(int pi, int brd_idx, vector<int>& pos) {
	if (brd_idx == n * m) return;
	if (pi == 3) {
		ans = max(ans, get_value_in_brd(pos));
		return;
	}

	for (int i = brd_idx; i < n * m; i++) {
		int r = i / m;
		int c = i % m;
		if (brd[r][c] == 0) {
			pos[pi] = i;
			dfs(pi + 1, i + 1, pos);
		}
	}
}

int main() {
	scanf("%d %d", &n, &m);
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			// 도로: 0, 인도: 1
			scanf("%d", &brd[i][j]);
		}
	}
	vector<int> pos = vector<int>{ 0, 0, 0 };
	dfs(0, 0, pos);
	printf("%d", ans);
}