#include <cstdio>
#include <queue>
using namespace std;

typedef struct V {
	int x, y, d;
	V() {}
	V(int _x, int _y, int _d) :x(_x), y(_y), d(_d) {}
};

const int MAXN = 50, MAXM = 50;
// d : 0123 - 북동남서
const int dx[] = {-1, 0, 1, 0};
const int dy[] = {0, 1, 0, -1};
int brd[MAXN][MAXM];
bool visited[MAXN][MAXM];
bool visited2[MAXN][MAXN][4];

int left_curve(int cur_d) {
	return (cur_d - 1 + 4) % 4;
}
int right_curve(int cur_d) {
	return (cur_d + 1) % 4;
}

bool is_out(int x, int y, int n, int m) {
	return (x < 0 || y < 0 || x >= n || y >= m);
}

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	
	int sx, sy, sd;
	scanf("%d %d %d", &sx, &sy, &sd);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			// 도로: 0, 인도: 1
			scanf("%d", &brd[i][j]);
		}
	}

	int cx = sx, cy = sy, cd = sd;
	visited[cx][cy] = true;
	visited2[cx][cy][cd] = true;
	int cnt = 0;
	while (true) {
		int nd = left_curve(cd);
		int nx = cx + dx[nd];
		int ny = cy + dy[nd];
		if (!is_out(nx, ny, n, m) && !brd[nx][ny]) {
			if (!visited[nx][ny]) {
				cx = nx, cy = ny, cd = nd;
				visited[cx][cy] = true;
				visited2[cx][cy][cd] = true;
				cnt = 0;
				continue;
			}
		}

		cd = nd;
		cnt++;

		if (cnt == 5) {
			nd = right_curve(cd);
			nx = cx - dx[nd];
			ny = cy - dy[nd];

			cx = nx, cy = ny, cd = nd;
			if (is_out(cx, cy, n, m)) break;
			cnt = 0;
			continue;
		}
	}

	int sum = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			sum += visited[i][j];
		}
	}
	printf("%d", sum);
}