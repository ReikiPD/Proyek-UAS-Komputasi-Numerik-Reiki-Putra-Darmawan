// Steady-State 1D Heat Conduction with Variable Conductivity & Nonuniform Generation
// dengan visualisasi Gnuplot otomatis

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>   // Untuk operasi file (CSV dan skrip)
#include <iomanip>   // Untuk format output (setprecision)
#include <cstdlib>   // Untuk fungsi system()

using namespace std;

// Thomas algorithm for tridiagonal systems
template<typename T>
void thomas(const vector<T>& a, const vector<T>& b,
            const vector<T>& c, vector<T>& d, vector<T>& x) {
    int n = b.size();
    vector<T> cp(n), dp(n);

    cp[0] = c[0] / b[0];
    dp[0] = d[0] / b[0];

    for (int i = 1; i < n; ++i) {
        T m = b[i] - a[i] * cp[i - 1];
        cp[i] = (i < n - 1) ? c[i] / m : 0;
        dp[i] = (d[i] - a[i] * dp[i - 1]) / m;
    }

    x[n - 1] = dp[n - 1];
    for (int i = n - 2; i >= 0; --i) {
        x[i] = dp[i] - cp[i] * x[i + 1];
    }
}

int main() {
    // --- 1. Input Parameter dari Pengguna ---
    double L, T0, TL;
    int N;
    double k0, beta, q0;
    cout << "=== Solver Konduksi Panas 1D ===\n";
    cout << "Panjang L: "; cin >> L;
    cout << "Temperatur T(0), T(L): "; cin >> T0 >> TL;
    cout << "Konduktivitas dasar k0: "; cin >> k0;
    cout << "Variasi beta (k(x)=k0*(1+beta*x/L)): "; cin >> beta;
    cout << "Amplitudo sumber q0: "; cin >> q0;
    cout << "Jumlah segmen N: "; cin >> N;

    // --- 2. Setup Perhitungan Numerik ---
    int n = N - 1;          // Jumlah titik nodal internal
    double dx = L / N;
    const double pi = acos(-1.0);

    vector<double> a(n), b(n), c(n), d(n), T(n);
    auto k = [&](double x) { return k0 * (1 + beta * x / L); };
    auto q = [&](double x) { return q0 * sin(pi * x / L); };

    // --- 3. Perakitan Sistem Matriks Tridiagonal ---
    for (int i = 0; i < n; ++i) {
        double xi = (i + 1) * dx;
        double km = k(xi - dx / 2);
        double kp = k(xi + dx / 2);
        a[i] = km;
        c[i] = kp;
        b[i] = -(km + kp);
        d[i] = -q(xi) * dx * dx;
        if (i == 0)      d[i] -= a[i] * T0;
        if (i == n - 1)  d[i] -= c[i] * TL;
    }

    // --- 4. Solusi Sistem dengan Algoritma Thomas ---
    thomas<double>(a, b, c, d, T);

    // --- 5. Tampilkan Hasil di Terminal (Opsional) ---
    cout << "\n--- Distribusi Temperatur (Hasil Numerik) ---\n";
    cout << fixed << setprecision(6);
    cout << "x = " << 0.0 << ", T = " << T0 << "\n";
    for (int i = 0; i < n; ++i) {
        cout << "x = " << (i + 1) * dx << ", T = " << T[i] << "\n";
    }
    cout << "x = " << L << ", T = " << TL << "\n";

    // --- 6. Simpan Hasil ke File CSV untuk Plotting ---
    ofstream fout("heat_conduction_results.csv");
    fout << "position,temperature\n";
    fout << fixed << setprecision(6);
    fout << 0.0 << "," << T0 << "\n";
    for (int i = 0; i < n; ++i) {
        fout << (i + 1) * dx << "," << T[i] << "\n";
    }
    fout << L << "," << TL << "\n";
    fout.close();
    cout << "\n[INFO] Data hasil perhitungan tersimpan di 'heat_conduction_results.csv'\n";

    // --- 7. Buat Skrip Gnuplot Secara Otomatis ---
    ofstream gp("plot_heat_conduction.gp");
    gp << "set datafile separator ','\n";
    gp << "set title 'Distribusi Temperatur Steady-State 1D'\n";
    gp << "set xlabel 'Posisi (m)'\n";
    gp << "set ylabel 'Temperatur (K)'\n";
    gp << "set grid\n";
    gp << "set key top left\n";
    gp << "plot 'heat_conduction_results.csv' using 1:2 with linespoints pt 7 lc rgb 'red' title 'Temperatur Numerik'\n";
    gp << "pause -1 'Tekan tombol apa saja untuk menutup jendela plot'\n";
    gp.close();
    cout << "[INFO] Skrip Gnuplot tersimpan di 'plot_heat_conduction.gp'\n";

    // --- 8. Jalankan Gnuplot untuk Menampilkan Grafik ---
    // PENTING: Sesuaikan path ini dengan lokasi instalasi Gnuplot di komputer Anda!
    const char* gnuplotPath = "\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\"";
    string cmd = string(gnuplotPath) + " -p plot_heat_conduction.gp";

    cout << "[AKSI] Mencoba menjalankan Gnuplot...\n";
    if (system(cmd.c_str()) != 0) {
        cerr << "[WARNING] Gagal menjalankan Gnuplot. Pastikan Gnuplot terinstal dan path sudah benar.\n";
    }

    return 0;
}