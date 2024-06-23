#include <cstdio>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

struct Virus {
	int r, c;
	int age;
	bool alive;
	Virus(int _r, int _c, int _age, bool _alive) : r(_r), c(_c), age(_age), alive(_alive) {}
};

struct cmp {
	bool operator() (Virus a, Virus b) {
		return a.age > b.age;
	}
};

const int MAXN = 10;
const int dr[] = { -1, -1, -1, 0, 1, 1, 1, 0 };
const int dc[] = { -1, 0, 1, 1, 1, 0, -1, -1 };

int n, m, k;
int brd[MAXN][MAXN];
int add_amount[MAXN][MAXN];	// 추가되는 양분의 양

priority_queue<Virus, vector<Virus>, cmp> do_cycle(priority_queue<Virus, vector<Virus>, cmp>& vs) {
	queue<Virus> new_vs;

	// age순 오름차순 정렬
	/*sort(vs.begin(), vs.end(),
		[](const Virus& x, const Virus& y) -> bool {
			return x.age < y.age;
		});*/

	queue<Virus> dead;
	// 양분 섭취
	while(!vs.empty()) {
		Virus v = vs.top();
		vs.pop();

		if (brd[v.r][v.c] >= v.age) {
			brd[v.r][v.c] -= v.age;
			v.age++;
			new_vs.push(v);
		}
		else {
			// 아사
			v.alive = false;
			dead.push(v);
		}
	}

	// 죽은 바이러스는 양분으로
	
	while (!dead.empty()) {
		Virus v = dead.front();
		dead.pop();

		if (v.alive) continue;
		brd[v.r][v.c] += v.age / 2;
	}

	priority_queue<Virus, vector<Virus>, cmp> ret;
	queue<Virus> tmp;

	// 번식 진행
	while (!new_vs.empty()) {
		Virus v = new_vs.front();
		new_vs.pop();

		ret.push(v);

		if (!v.alive) continue;
		if (v.age % 5 != 0) continue;

		// 살아있는 바이러스 중에 나이가 5의 배수인 경우, 번식
		for (int d = 0; d < 8; d++) {
			int nr = v.r + dr[d];
			int nc = v.c + dc[d];
			if (nr < 0 || nr >= n || nc < 0 || nc >= n) continue;
			
			tmp.push(Virus(nr, nc, 1, true));
		}
	}

	while (!tmp.empty()) {
		Virus v = tmp.front();
		tmp.pop();

		ret.push(v);
	}

	// 양분 추가
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			brd[i][j] += add_amount[i][j];
		}
	}
	return ret;
}

int main() {
	scanf("%d %d %d", &n, &m, &k);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			brd[i][j] = 5;
			scanf("%d", &add_amount[i][j]);
		}
	}

	priority_queue<Virus, vector<Virus>, cmp> vs;
	for (int i = 0, r, c, age; i < m; i++) {
		scanf("%d %d %d", &r, &c, &age);
		r--, c--;
		vs.push(Virus(r, c, age, true));
	}

	while (k--) {
		vs = do_cycle(vs);
	}
	printf("%d", vs.size());
}