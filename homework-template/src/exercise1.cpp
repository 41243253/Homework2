#include <iostream>
#include <cmath>   // for ceil and log
#include <iomanip> // for setprecision

using namespace std;

int main() {
    // 已知參數
    double ts = 0.08;      // seek time (s)
    double tl = 0.02;      // latency time (s)
    double tt = 0.001;     // transmission time (s)
    int n = 200000;        // 總記錄數
    int m = 64;            // 初始 runs 數量

    // 計算每筆記錄的 I/O 時間
    double t_record = ts + tl + 2 * tt;

    cout << fixed << setprecision(6);
    cout << "k\tmerge_passes\ttotal_input_time (seconds)\n";

    // 計算 k=2 ~ m 時，總輸入時間
    for (int k = 2; k <= m; ++k) {
        // 計算需要幾次合併
        int merge_passes = static_cast<int>(ceil(log(m) / log(k)));

        // 總輸入時間
        double t_input = merge_passes * n * t_record;

        cout << k << "\t" << merge_passes << "\t\t" << t_input << endl;
    }

    return 0;
}
