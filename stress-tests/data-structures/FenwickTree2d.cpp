#include "../utilities/template.h"

#include "../../content/data-structures/FenwickTree2d.h"

int main() {
	for (int it = 0; it < (1000000); ++it) {
		FenwickTree2D ft(12);
		vector<tuple<int, int, int>> upd;
		int c = rand() % 20;
		for (int i = 0; i < (c); ++i) {
			upd.emplace_back(rand() % 12 + 1, rand() % 12 + 1,
				rand() % 10 - 5);
		}

		vector<vector<int>> grid(12, vector<int>(12)), sumto(13, vector<int>(13));
		for(auto &pa: upd)
			ft.fake_update(get<0>(pa), get<1>(pa));
		ft.init();
		for(auto &pa: upd) {
			grid[get<0>(pa) - 1][get<1>(pa) - 1] += get<2>(pa);
			ft.update(get<0>(pa), get<1>(pa), get<2>(pa));
		}

		for (int i = 0; i < (13); ++i) {
			for (int j = 0; j < (13); ++j) {
				ll v = ft.query(i, j);
				if (i == 0 || j == 0) assert(v == 0);
				else {
					sumto[i][j] = grid[i-1][j-1] + sumto[i-1][j] + sumto[i][j-1] - sumto[i-1][j-1];
					assert(v == sumto[i][j]);
				}
			}
		}
	}
	cout<<"tests passed!"<<endl;
}
