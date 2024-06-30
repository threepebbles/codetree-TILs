#include <cstdio>
#include <vector>
#include <algorithm>
#include <memory.h>
using namespace std;

struct V {
	int r, c;
	int s, d, b;
	V() { s = -1, d = -1, b = 0; }
	V(int _r, int _c, int _s, int _d, int _b): r(_r), c(_c), s(_s), d(_d), b(_b) {}
};

const int MAXN = 100, MAXM = 100;
// 위, 아래, 오른쪽, 왼쪽 순서
const int dr[] = {-1, 1, 0, 0};
const int dc[] = {0, 0, 1, -1};

int n, m, k;
V brd[MAXN][MAXM];
V new_brd[MAXN][MAXM] = {};

// O(m*log(m))
V find_pange_and_erase(int col) {
	V ret = V();
	for (int r = 0; r < n; r++) {
		if (brd[r][col].b != 0) {
			ret = brd[r][col];
			brd[r][col] = V();
			break;
		}
	}
	return ret;;
}

int reverse_direction(int d) {
	if (d == 0) return 1;
	else if (d == 1) return 0;
	else if (d == 2) return 3;
	return 2;
}

V choose(V v1, V v2) {
	return v1.b > v2.b ? v1 : v2;
}


void move_for_second() {
	memset(new_brd, 0, sizeof(new_brd));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if(brd[i][j].b > 0) {
				V x = brd[i][j];
				int nr = x.r + dr[x.d]*x.s;
				int nc = x.c + dc[x.d] * x.s;
				// 상하
				if (x.d <= 1) {
					nr += 2 * (n - 1);
					nr %= 2 * (n - 1);
					int nd = x.d;
					if (nr <= n - 1) {
						new_brd[nr][nc] = choose(new_brd[nr][nc], V(nr, nc, x.s, nd, x.b));
					}
					else {
						nr = 2 * (n - 1) - nr;
						int nd = reverse_direction(x.d);

						new_brd[nr][nc] = choose(new_brd[nr][nc], V(nr, nc, x.s, nd, x.b));
					}
				}
				// 좌우
				else {
					nc += 2 * (m - 1);
					nc %= 2 * (m - 1);
					int nd = x.d;
					if (nc <= m - 1) {
						new_brd[nr][nc] = choose(new_brd[nr][nc], V(nr, nc, x.s, nd, x.b));
					}
					else {
						nc = 2 * (m - 1) - nc;
						int nd = reverse_direction(x.d);

						new_brd[nr][nc] = choose(new_brd[nr][nc], V(nr, nc, x.s, nd, x.b));
					}
				}
			}
		}
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			brd[i][j] = new_brd[i][j];
		}
	}
}

int main() {
	scanf("%d %d %d", &n, &m, &k);
	for (int i = 0, x, y, s, d, b; i < k; i++) {
		scanf("%d %d %d %d %d", &x, &y, &s, &d, &b);
		// 0베이스
		x--, y--;
		d--;
		// 상하
		if (d <= 1) {
			s %= 2 * (n-1);
		}
		// 좌우
		else {
			s %= 2 * (m-1);
		}
		// s: 초속, d: 이동방향, b: 곰팡이 크기
		brd[x][y] = V(x, y, s, d, b);
	}

	int sum = 0;
	// 채취 시작
	for (int col = 0; col < m; col++) {
		
		// 2. 곰팡이 채취
		V target = find_pange_and_erase(col);
		sum += target.b;
		
		// 곰팡이 이동
		// 같은 칸에 있는 경우 큰 곰팡이가 잡아먹음
		move_for_second();
	}
	printf("%d", sum);
}