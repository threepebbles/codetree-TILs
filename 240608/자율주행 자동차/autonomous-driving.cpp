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

	queue<V> q;
	q.push(V(sx, sy, left_curve(sd)));
	while (!q.empty()) {
		V cur = q.front();
		q.pop();
		if (visited2[cur.x][cur.y][cur.d]) continue;
		visited[cur.x][cur.y] = true;
		visited2[cur.x][cur.y][cur.d] = true;

		int cx = cur.x, cy = cur.y, cd = cur.d;
		
		while (true) {
			cx = cx + dx[cd];
			cy = cy + dy[cd];
			if (!is_out(cx, cy, n, m) && !brd[cx][cy] && !visited[cx][cy]) {
				q.push(V(cx, cy, left_curve(cd)));
				break;
			}
			cx = (cx - dx[cd] + 4) % 4;
			cy = (cy - dy[cd] + 4) % 4;
			cd = left_curve(cd);

			// 4방향 다 막힌 경우
			if (cd == cur.d) {
				cd = right_curve(cd);
				cx = (cx - dx[cd]+4)%4;
				cy = (cy - dy[cd]+4)%4;
				
				// 후진, 좌회전 상태로 큐에 삽입
				if (!is_out(cx, cy, n, m) && !brd[cx][cy] && !visited2[cx][cy][cd]) {
					q.push(V(cx, cy, left_curve(cd)));
				}
				break;
			}
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