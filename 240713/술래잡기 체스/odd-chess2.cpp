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

int find_thief(int r, int c, vector<V>& ts) {
	for (int i = 1; i <= ROW_SIZE * COL_SIZE; i++) {
		V t = ts[i];
		if (t.d == -1) continue;

		if (t.r == r && t.c == c) return i;
	}
	return 0;
}

void move_thieves(V& chaser, vector<V>& ts) {
	vector<V> new_ts = ts;
	for (int i = 1; i <= 16; i++) {
		// 잡힌 도둑 패스
		if (ts[i].d == -1) continue;
		
		int d_st = ts[i].d;
		while (true) {
			int nr = ts[i].r + dr[ts[i].d];
			int nc = ts[i].c + dc[ts[i].d];
			// 격자 밖이거나, 술래가 있는 경우
			if (!is_in_range(nr, nc) 
				|| (ts[i].r==chaser.r && ts[i].c==chaser.c)) {
				ts[i].d++;

				// 이동 불가
				if (ts[i].d == d_st) {
					break;
				}
				continue;
			}
			
			// 이동
			int num_target = find_thief(nr, nc, ts);

			//빈칸
			if (num_target == 0) {
				ts[i].r = nr;
				ts[i].c = nc;
			}
			else {
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
void dfs(V& chaser, vector<V>& ts, int score) {
	move_thieves(chaser, ts);

	vector<int> cands;
	int r = chaser.r, c = chaser.c, d = chaser.d;
	while (true) {
		int nr = r + dr[d];
		int nc = c + dc[d];
		if (!is_in_range(nr, nc)) break;
		
		r = nr, c = nc;
		int num_thief = find_thief(r, c, ts);
		if (num_thief != 0) {
			cands.push_back(num_thief);
		}
	}
	// 잡을 있는 도둑말이 없는 경우
	if (cands.size() == 0) {
		ans = max(score, ans);
		return;
	}

	for (int num_thief: cands) {
		V cand = ts[num_thief];
		V chaser_next = V(cand.r, cand.c, ts[num_thief].d);
		vector<V> ts_next = ts;
		ts_next[num_thief] = V(-1, -1, -1);
		int score_next = score + num_thief;

		dfs(chaser_next, ts_next, score_next);
	}
}

int main() {
	V chaser;
	// d==-1 이면 사망
	vector<V> thieves(ROW_SIZE*COL_SIZE+1, V(-1, -1, -1));

	for (int i = 0, num, d; i < ROW_SIZE; i++) {
		for (int j = 0; j < COL_SIZE; j++) {
			scanf("%d %d", &num, &d);
			d--;
			thieves[num] = V(i, j, d);
		}
	}
	// (0, 0)에 있는 도둑말 잡기
	int score = 0;
	int num_theif = find_thief(0, 0, thieves);
	
	chaser = V(0, 0, thieves[num_theif].d);
	score += num_theif;
	thieves[num_theif] = V(-1, -1, -1);

	dfs(chaser, thieves, score);
	printf("%d", ans);
}