#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;

const int ROW_SIZE = 4, COL_SIZE = 4;
const int NUM_CHASER = 17;

const int dr[] = { -1, -1, 0, 1, 1, 1, 0, -1 };
const int dc[] = { 0, -1, -1, -1, 0, 1, 1, 1 };

struct V {
	int r, c, d;
	V() {}
	V(int _r, int _c, int _d): r(_r), c(_c), d(_d) {}
};

bool is_in_range(int r, int c) {
	if (r < 0 || r >= ROW_SIZE || c < 0 || c >= COL_SIZE) return false;
	return true;
}

void move_thieves(vector<vector<int>>& brd, vector<V>& ts) {
	for (int i = 1; i <= 16; i++) {
		// 잡힌 도둑 패스
		if (ts[i].d == -1) continue;
		
		int d_st = ts[i].d;
		while (true) {
			int nr = ts[i].r + dr[ts[i].d];
			int nc = ts[i].c + dc[ts[i].d];
			if (!is_in_range(nr, nc) || brd[nr][nc] == NUM_CHASER) {
				ts[i].d++;

				// 이동 불가
				if (ts[i].d == d_st) {
					break;
				}
				continue;
			}
			
			// 이동
			// 빈칸
			if (brd[nr][nc] == 0) {
				brd[nr][nc] = i;
				brd[ts[i].r][ts[i].c] = 0;

				ts[i].r = nr;
				ts[i].c = nc;
			}
			// 다른 도둑과 swap
			else {
				int num_target = brd[nr][nc];
				
				brd[ts[i].r][ts[i].c] = num_target;
				brd[nr][nc] = i;

				ts[num_target].r = ts[i].r;
				ts[num_target].c = ts[i].c;

				ts[i].r = nr;
				ts[i].c = nc;
			}
			
			break;
		}
	}
}

int ans;
void dfs(V& chaser, vector<vector<int>>& brd, vector<V>& ts, int score) {
	move_thieves(brd, ts);

	vector<int> cands;
	int r = chaser.r, c = chaser.c, d = chaser.d;
	while (true) {
		int nr = r + dr[d];
		int nc = c + dc[d];
		if (!is_in_range(nr, nc)) break;
		
		r = nr, c = nc;
		int num_thief = brd[r][c];
		if (num_thief != 0) {
			cands.push_back(num_thief);
		}
	}
	// 잡을 있는 도둑말이 없는 경우
	if (cands.size() == 0) {
		ans = max(score, ans);
		return;
	}

	vector<vector<int>> brd_tmp = brd;
	vector<V> ts_tmp = ts;
	for (int num_thief: cands) {
		V cand = ts[num_thief];
		V chaser_next = V(cand.r, cand.c, ts[num_thief].d);
		int score_next = score + num_thief;

		brd[chaser.r][chaser.c] = 0;
		brd[cand.r][cand.c] = NUM_CHASER;
		ts[num_thief] = V(-1, -1, -1);

		dfs(chaser_next, brd, ts, score + num_thief);
		// 복원
		brd = brd_tmp;
		ts = ts_tmp;
	}
}

int main() {
	// board. 0이면 빈칸
	vector<vector<int>> brd(ROW_SIZE, vector<int>(COL_SIZE, 0));

	V chaser;
	// d==-1 이면 사망
	vector<V> thieves(ROW_SIZE*COL_SIZE+1, V(-1, -1, -1));

	for (int i = 0, num, d; i < ROW_SIZE; i++) {
		for (int j = 0; j < COL_SIZE; j++) {
			scanf("%d %d", &num, &d);
			d--;
			thieves[num] = V(i, j, d);
			brd[i][j] = num;
		}
	}
	// (0, 0)에 있는 도둑말 잡기
	int score = 0;
	int num_theif = brd[0][0];
	
	chaser = V(0, 0, thieves[num_theif].d);
	score += num_theif;
	thieves[num_theif] = V(-1, -1, -1);
	brd[0][0] = NUM_CHASER;

	dfs(chaser, brd, thieves, score);
	printf("%d", ans);
}