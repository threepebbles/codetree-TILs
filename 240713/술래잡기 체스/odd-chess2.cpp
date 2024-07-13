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
	int num;
	V() {}
	V(int _r, int _c, int _d, int _num) : r(_r), c(_c), d(_d), num(_num) {}
};

bool is_in_range(int r, int c) {
	if (r < 0 || r >= ROW_SIZE || c < 0 || c >= COL_SIZE) return false;
	return true;
}

int find_thief_idx(int r, int c, vector<V>& ts) {
	for (int i = 0; i < ts.size(); i++) {
		V t = ts[i];
		if (t.num == -1) continue;

		if (t.r == r && t.c == c) return i;
	}
	return -1;
}

void move_thieves(V& chaser, vector<V>& ts) {
	for (int i = 0; i < ts.size(); i++) {
		// 잡힌 도둑 패스
		if (ts[i].num == -1) continue;

		int d_st = ts[i].d;
		int nd = ts[i].d;
		while (true) {
			int nr = ts[i].r + dr[nd];
			int nc = ts[i].c + dc[nd];
			// 격자 밖이거나, 술래가 있는 경우
			if (!is_in_range(nr, nc)
				|| (nr == chaser.r && nc == chaser.c)) {
				nd++;
				nd %= 8;

				// 이동 불가한 경우 (는 있을수가 없긴 함)
				if (nd == d_st) {
					break;
				}
				continue;
			}

			ts[i].d = nd;
			int ti = find_thief_idx(nr, nc, ts);
			//빈칸
			if (ti == -1) {
				ts[i].r = nr;
				ts[i].c = nc;
			}
			else {
				ts[ti].r = ts[i].r;
				ts[ti].c = ts[i].c;
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
		int ti = find_thief_idx(r, c, ts);
		if (ti!= -1) {
			cands.push_back(ti);
		}
	}
	// 잡을 있는 도둑말이 없는 경우
	if (cands.size() == 0) {
		ans = max(score, ans);
		return;
	}

	for (int ti : cands) {
		V cand = ts[ti];
		int score_next = score + cand.num;
		V chaser_next = V(cand.r, cand.c, cand.d, chaser.num);
		vector<V> ts_next = ts;
		ts_next[ti].num = -1;

		dfs(chaser_next, ts_next, score_next);
	}
}

int main() {
	V chaser;
	// ts[i].num == -1 이면 잡힌 상태
	vector<V> ts;

	for (int i = 0, num, d; i < ROW_SIZE; i++) {
		for (int j = 0; j < COL_SIZE; j++) {
			scanf("%d %d", &num, &d);
			d--;
			ts.push_back(V(i, j, d, num));
		}
	}
	sort(ts.begin(), ts.end(), [](V& t1, V& t2) {
		return t1.num < t2.num;
		});

	// (0, 0)에 있는 도둑말 잡기
	int ti = find_thief_idx(0, 0, ts);
	int score_next = ts[ti].num;
	chaser = V(0, 0, ts[ti].d, NUM_CHASER);
	
	vector<V> ts_next = ts;
	ts_next[ti].num = -1;

	dfs(chaser, ts_next, score_next);
	printf("%d", ans);
}