#include <cstdio>
#include <vector>
using namespace std;

bitset<8> bs;

int rotate_clockwise(int x) {
	int ret = ((x << 1) & 0xFF);
	if ((x & (1 << 7))) ret += 1;
	return ret;
}

int rotate_counter_clockwise(int x) {
	int ret = ((x >> 1) & 0xFF);
	if ((x & 1)) ret += (1 << 7);
	return ret;
}

// x를 d방향으로 회전 d가 -1이면 >> / d가 1이면 <<
int rotate_bit(int x, int d) {
	if (d == -1) return rotate_counter_clockwise(x);
	return rotate_clockwise(x);
}

// x의 n번째 비트가 0이면 0, 1이면 1반환 
int extract_bit(int x, int n) {
	int ret = (x >> n) & 1;
	return ret;
}

vector<int> rotate(int n, int d, vector<int> brd) {
	// n번 팔각 의자를 방향 d로 45도 회전
	int bit2 = (1 << 2);
	int bit6 = (1 << 6);

	vector<int> new_brd = brd;
	
	// n의자와 6번째 비트와 n-1의자의 3번째 비트가 일치할 때까지 회전
	for (int i = n - 1, d2 = -d; i >= 0; i--, d2 *= -1) {
		if (extract_bit(brd[i + 1], 6) != extract_bit(brd[i], 2)) {
			new_brd[i] = rotate_bit(brd[i], d2);
		}
	}

	// n의자의 2번째 비트와 n+1의자의 6번째 비트가 일치할 때까지 회전
	for (int i = n + 1, d2 = -d; i < 4; i++, d2 *= -1) {
		if (extract_bit(brd[i - 1], 2) != extract_bit(brd[i], 6)) {
			new_brd[i] = rotate_bit(brd[i], d2);
		}
	}
	brd = new_brd;
	if (d == -1) brd[n] = rotate_counter_clockwise(brd[n]);
	else brd[n] = rotate_clockwise(brd[n]);
	return brd;
}

int main() {
	// 북쪽: 0, 남쪽: 1
	vector<int> brd;
	char buf[9];
	for (int i = 0; i < 4; i++) {
		scanf("%s", buf);
		int bit = 0;
		for (int j = 7; j >= 0; j--) {
			if (buf[j] == '1') {
				bit += 1;
			}
			if (j != 0) bit <<= 1;
		}
		brd.push_back(bit);
	}

	int k;
	scanf("%d", &k);
	for (int i = 0, n, d; i < k; i++) {
		// 반시계: -1, 시계: 1
		scanf("%d %d", &n, &d);
		n--;
		brd = rotate(n, d, brd);
	}

	int ans = 0;
	for (int i = 0; i < 4; i++) {
		ans += ((brd[i] & 1) << i);
	}
	printf("%d", ans);
}