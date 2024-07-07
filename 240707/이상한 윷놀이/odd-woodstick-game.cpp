#include <cstdio>
#include <vector>
#include <algorithm>
#include <stdlib.h>
using namespace std;

struct V {
	int x, y;
	int d;
	V* upper = NULL;

	V() {}
	V(int _x, int _y, int _d) :x(_x), y(_y), d(_d) { }
};

const int MAXN = 12, MAXK = 10;
// 격자 색깔
const int WHITE = 0;
const int RED = 1;
const int BLUE = 2;

const int MAX_TURN = 1000;

// 방향
const int RIGHT = 1;
const int LEFT = 2;
const int UP = 3;
const int DOWN = 4;

// 위, 오른쪽, 아래, 왼쪽 순서
const int dx[] = { -1, 0, 1, 0 };
const int dy[] = { 0, 1, 0, -1 };

int n, k;
int brd[MAXN][MAXN];
V* state[MAXN][MAXN];	// i, j에 놓여있는 가장 아래쪽 말 저장
vector<V*> horses;

bool is_in_range(int x, int y) {
	if (x < 0 || x >= n || y < 0 || y >= n) return false;
	return true;
}

V* find_upper_horse(V* start) {
	V* cur = start;
	return cur->upper;
}

V* find_lower_horse(V* bottom, V* target) {
	V* cur = bottom;

	if (cur == NULL) {
		int x = 1;
	}
	V* nxt = cur->upper;
	while (nxt != target) {
		cur = nxt;
		nxt = cur->upper;
	}
	return cur;
}

V* find_top_horse(V* start) {
	V* cur = start;
	V* nxt = cur->upper;
	while (nxt != NULL) {
		cur = nxt;
		nxt = cur->upper;
	}
	return cur;
}

void update_crd(V* bottom) {
	V* cur = bottom;
	if (cur == NULL) return;

	int x = bottom->x;
	int y = bottom->y;

	if (x == 3 && y == 1) {
		int yy = 1;
	}
	else if (x == 2 && y == 1) {
		int zz = 1;
	}
	while (cur != NULL) {
		cur->x = x;
		cur->y = y;
		cur = cur->upper;
	}
}

int count_horse(V* bottom) {
	int cnt = 0;
	V* cur = bottom;
	while (cur != NULL) {
		cnt++;
		cur = cur->upper;
	}
	return cnt; 
}

bool do_white(V* h) {
	// 움직이기 전, 맨 아래 위치였던 경우
	if (state[h->x][h->y] == h) {
		state[h->x][h->y] = NULL;
	}
	// 맨 아래가 아니었던 경우
	else {
		V* lower_horse = find_lower_horse(state[h->x][h->y], h);
		lower_horse->upper = NULL;
	}

	int nx = h->x + dx[h->d];
	int ny = h->y + dy[h->d];

	h->x = nx;
	h->y = ny;

	// 이동
	// 빈 자리면
	if (state[h->x][h->y] == NULL) {
		state[h->x][h->y] = h;
	}
	// 말이 이미 있으면
	else {
		V* top_horse = find_top_horse(state[h->x][h->y]);
		top_horse->upper = h;
	}
	update_crd(state[h->x][h->y]);
	return count_horse(state[h->x][h->y])>=4;
}

bool do_red(V* h) {
	// 움직이기 전, 맨 아래 위치였던 경우
	if (state[h->x][h->y] == h) {
		state[h->x][h->y] = NULL;
	}
	// 맨 아래가 아니었던 경우
	else {
		V* lower_horse = find_lower_horse(state[h->x][h->y], h);
		lower_horse->upper = NULL;
	}

	int nx = h->x + dx[h->d];
	int ny = h->y + dy[h->d];

	h->x = nx;
	h->y = ny;

	// 순서 뒤집기
	vector<V*> order;
	V* cur = h;
	while (cur != NULL) {
		cur->x = nx;
		cur->y = ny;
		order.push_back(cur);
		V* nxt = cur->upper;
		// 원래 방향 링크 삭제
		cur->upper = NULL;

		cur = nxt;
	}

	// 순서 거꾸로 링크
	reverse(order.begin(), order.end());
	cur = order[0];
	for (int i = 1; i < order.size(); i++) {
		cur->upper = order[i];
		cur = order[i];
	}

	// 이동
	// 비어있으면
	if (state[h->x][h->y] == NULL) {
		state[h->x][h->y] = order[0];
	}
	// 이미 말이 있으면
	else {
		V* top_horse = find_top_horse(state[h->x][h->y]);
		top_horse->upper = order[0];
	}
	update_crd(state[h->x][h->y]);
	return count_horse(state[h->x][h->y]) >= 4;
}

bool do_blue(V* h) {
	h->d = (h->d + 2) % 4;
	int nx = h->x + dx[h->d];
	int ny = h->y + dy[h->d];
	// 반대 방향도 격자 밖 혹은 파랑색 격자인 경우
	if (!is_in_range(nx, ny) || brd[nx][ny] == BLUE) {
		// 제자리에서 이동 종료
		return false;
	}
	
	// 움직이기 전, 맨 아래 위치였던 경우
	if (state[h->x][h->y] == h) {
		state[h->x][h->y] = NULL;
	}
	// 맨 아래가 아니었던 경우
	else {
		V* lower_horse = find_lower_horse(state[h->x][h->y], h);
		lower_horse->upper = NULL;
	}

	h->x = nx;
	h->y = ny;

	// 이동
	// 비어있으면
	if (state[h->x][h->y] == NULL) {
		state[h->x][h->y] = h;
	}
	else {
		V* top_horse = find_top_horse(state[h->x][h->y]);
		top_horse->upper = h;
	}
	update_crd(state[h->x][h->y]);
	return count_horse(state[h->x][h->y]) >= 4;
}


bool do_move(V* h) {
	int nx = h->x + dx[h->d];
	int ny = h->y + dy[h->d];

	if (!is_in_range(nx, ny)) {
		return do_blue(h);
	}
	else if (brd[nx][ny] == BLUE) {
		return do_blue(h);
	}
	else if (brd[nx][ny] == WHITE) {
		return do_white(h);
	}
	else if (brd[nx][ny] == RED) {
		return do_red(h);
	}
	// 도달할 일 없음
	return false;
}

int main() {
	scanf("%d %d", &n, &k);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%d", &brd[i][j]);
		}
	}

	for (int i = 0, x, y, d; i < k; i++) {
		scanf("%d %d %d", &x, &y, &d);
		// 0 베이스
		x--, y--;

		if (d == RIGHT) d = 1;
		else if (d == LEFT) d = 3;
		else if (d == UP) d = 0;
		else d = 2;

		V* h = (V*)malloc(sizeof(struct V));
		h->x = x;
		h->y = y;
		h->d = d;
		h->upper = NULL;
		// 같은 칸에 두 개의 말이 주어지는 경우는 없음
		horses.push_back(h);
		state[x][y] = h;
	}


	int turn = 0;
	while (turn < MAX_TURN) {
		turn++;

		if (turn == 63) {
			int x = 1;
		}

		bool is_end = false;
		// 순서대로 말 이동
		for (int hi = 0; hi < horses.size() && !is_end; hi++) {
			is_end = do_move(horses[hi]);
		}

		if (is_end) break;
	}
	printf("%d", turn == MAX_TURN ? -1 : turn);
}