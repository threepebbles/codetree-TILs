#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;

struct V {
	int num;
	int cnt;
	V() {}
	V(int _num, int _cnt): num(_num), cnt(_cnt) {}
};

const int MAXN = 200;

int r_target, c_target, k_target;
int brd[MAXN][MAXN];
int new_brd[MAXN][MAXN];
int cnt[101];

int main() {
	scanf("%d %d %d", &r_target, &c_target, &k_target);
	r_target--, c_target--;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			scanf("%d", &brd[i][j]);
		}
	}

	int n = 3, m = 3;

	int ans = -1;
	for (int t = 0; t < 100; t++) {
		if (brd[r_target][c_target] == k_target) {
			ans = t;
			break;
		}
		
		// 행 기준 연산
		if (n >= m) {
			int m_max = -1;
			for (int r = 0; r < n; r++) {
				vector<V> freqs;
				for (int c = 0; c < m; c++) {
					cnt[brd[r][c]]++;
				}
				for (int i = 1; i <= 100; i++) {
					if (!cnt[i]) continue;
					freqs.push_back(V(i, cnt[i]));

					// initialize
					cnt[i] = 0;
				}
				sort(freqs.begin(), freqs.end(), 
					[](const V& v1, const V& v2) {
						if (v1.cnt == v2.cnt) return v1.num < v2.num;
						return v1.cnt < v2.cnt;
					}
				);
				m_max = max(m_max, (int)freqs.size() * 2);
				
				int c = 0;
				for (V& freq : freqs) {
					new_brd[r][c++] = freq.num;
					new_brd[r][c++] = freq.cnt;
				}
			}
			m = m_max;
		}
		// 열 기준 연산
		else {
			int n_max = -1;
			for (int c = 0; c < m; c++) {
				vector<V> freqs;
				for (int r = 0; r < n; r++) {
					cnt[brd[r][c]]++;
				}

				for (int i = 1; i <= 100; i++) {
					if (!cnt[i]) continue;
					freqs.push_back(V(i, cnt[i]));
					
					// initialize
					cnt[i] = 0;
				}

				sort(freqs.begin(), freqs.end(),
					[](const V& v1, const V& v2) {
						if (v1.cnt == v2.cnt) return v1.num < v2.num;
						return v1.cnt < v2.cnt;
					}
				);
				n_max = max(n_max, (int)freqs.size() * 2);

				int r = 0;
				for (V& freq : freqs) {
					new_brd[r++][c] = freq.num;
					new_brd[r++][c] = freq.cnt;
				}
			}
			n = n_max;
		}

		// 변경사항 반영
		for (int r = 0; r < n; r++) {
			for (int c = 0; c < m; c++) {
				brd[r][c] = new_brd[r][c];
				
				// initialize
				new_brd[r][c] = 0;
			}
		}
	}

	printf("%d", ans);
}