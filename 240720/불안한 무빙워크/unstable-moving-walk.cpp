#include <cstdio>
#include <vector>
using namespace std;

const int MAXN = 101;

struct V {
	int duration;
	bool person;
	V() {}
	V(int _d, bool _p) :duration(_d), person(_p) {};
};

int n, k;
vector<V> mw;

bool proceed() {
	// 한칸 시계방향 회전
	V last = mw[mw.size() - 1];
	mw.insert(mw.begin(), last);
	mw.pop_back();

	// 사람 한칸씩 이동
	for (int i = n - 1; i > 0; i--) {
		if (mw[i].person) {
			if (i + 1 == n) {
				mw[i].person = false;
				continue;
			}

			if (mw[i+1].person == true || mw[i + 1].duration == 0) {
				continue;
			}

			mw[i + 1].person = true;
			mw[i + 1].duration--;
		}
	}

	// 0번 칸에 사람 추가
	if (mw[0].person == false && mw[0].duration > 0) {
		mw[0].person = true;
		mw[0].duration--;
	}

	// 안정성이 0인칸이 k개 이상이면 종료
	int cnt = 0;
	for (int i = 0; i < 2 * n; i++) {
		if (mw[i].duration == 0) cnt++;
	}
	return cnt >= k;
}

int main() {
	scanf("%d %d", &n, &k);
	for (int i = 0, x; i < 2 * n; i++) {
		scanf("%d", &x);
		mw.push_back(V(x, false));
	}

	bool is_end = false;
	int turn = 0;
	while (!is_end) {
		turn++;
		is_end = proceed();
	}
	printf("%d", turn);
}