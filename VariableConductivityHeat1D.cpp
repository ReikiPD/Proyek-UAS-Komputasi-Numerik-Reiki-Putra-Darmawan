// Steady-State 1D Heat Conduction with Variable Conductivity & Nonuniform Generation
// dengan visualisasi Gnuplot otomatis

// --- Termasuk Pustaka (Library) yang Diperlukan ---
#include <iostream>  // Untuk operasi input/output di terminal (cout, cin)
#include <vector>    // Untuk menggunakan struktur data std::vector (array dinamis)
#include <cmath>     // Untuk fungsi matematika seperti acos() dan sin()
#include <fstream>   // Untuk operasi file, seperti menulis ke file CSV dan skrip Gnuplot
#include <iomanip>   // Untuk memanipulasi output, seperti std::setprecision untuk angka desimal
#include <cstdlib>   // Untuk menjalankan perintah sistem, seperti memanggil Gnuplot melalui system()

// Menggunakan namespace std untuk menghindari penulisan std:: di depan setiap fungsi standar
using namespace std;

// --- Definisi Fungsi Algoritma Thomas ---
// Fungsi ini menyelesaikan sistem persamaan linear tridiagonal: a_i*T_{i-1} + b_i*T_i + c_i*T_{i+1} = d_i
// Template<typename T> membuatnya generik untuk berbagai tipe data (misal: float, double)
template<typename T>
void thomas(const vector<T>& a, const vector<T>& b,
            const vector<T>& c, vector<T>& d, vector<T>& x) {
    int n = b.size();           // n adalah jumlah persamaan atau ukuran matriks
    vector<T> cp(n), dp(n);      // Vektor sementara untuk menyimpan koefisien yang dimodifikasi

    // --- Tahap 1: Eliminasi Maju (Forward Sweep) ---
    // Inisialisasi untuk baris pertama (i=0)
    cp[0] = c[0] / b[0];
    dp[0] = d[0] / b[0];

    // Loop untuk sisa baris dari i=1 hingga n-1
    for (int i = 1; i < n; ++i) {
        T m = b[i] - a[i] * cp[i - 1];          // Hitung penyebut untuk baris saat ini
        cp[i] = (i < n - 1) ? c[i] / m : 0;     // Hitung koefisien c' (cp). Untuk baris terakhir, c' tidak ada (nol).
        dp[i] = (d[i] - a[i] * dp[i - 1]) / m;  // Hitung koefisien d' (dp)
    }

    // --- Tahap 2: Substitusi Mundur (Back Substitution) ---
    // Hitung solusi untuk variabel terakhir (x_{n-1})
    x[n - 1] = dp[n - 1];
    // Loop mundur dari n-2 hingga 0 untuk mencari sisa variabel
    for (int i = n - 2; i >= 0; --i) {
        x[i] = dp[i] - cp[i] * x[i + 1];
    }
}

// --- Fungsi Utama Program ---
int main() {
    // --- 1. Deklarasi dan Input Parameter dari Pengguna ---
    double L, T0, TL;   // L: Panjang, T0: Temperatur di x=0, TL: Temperatur di x=L
    int N;              // N: Jumlah segmen diskritisasi
    double k0, beta, q0;// k0: Konduktivitas dasar, beta: Koefisien variasi k, q0: Amplitudo sumber panas
    
    cout << "=== Solver Konduksi Panas 1D ===\n";
    cout << "Panjang L: "; cin >> L;
    cout << "Temperatur T(0), T(L): "; cin >> T0 >> TL;
    cout << "Konduktivitas dasar k0: "; cin >> k0;
    cout << "Variasi beta (k(x)=k0*(1+beta*x/L)): "; cin >> beta;
    cout << "Amplitudo sumber q0: "; cin >> q0;
    cout << "Jumlah segmen N: "; cin >> N;

    // --- 2. Setup Perhitungan Numerik ---
    int n = N - 1;          // Jumlah titik nodal internal yang temperaturnya tidak diketahui
    double dx = L / N;      // Menghitung ukuran langkah atau jarak antar titik nodal
    const double pi = acos(-1.0); // Menghitung nilai pi dengan presisi tinggi

    vector<double> a(n), b(n), c(n), d(n), T(n); // Deklarasi vektor untuk matriks tridiagonal (a,b,c), vektor hasil (d), dan solusi Temperatur (T)
    
    // Mendefinisikan fungsi konduktivitas k(x) menggunakan lambda expression
    auto k = [&](double x) { return k0 * (1 + beta * x / L); };
    // Mendefinisikan fungsi sumber panas q(x) menggunakan lambda expression
    auto q = [&](double x) { return q0 * sin(pi * x / L); };

    // --- 3. Perakitan Sistem Matriks Tridiagonal (Assembly) ---
    // Loop ini mengisi vektor a, b, c, dan d sesuai rumus Metode Beda Hingga
    for (int i = 0; i < n; ++i) {
        double xi = (i + 1) * dx;       // Posisi titik nodal ke-i saat ini
        double km = k(xi - dx / 2);     // Konduktivitas di titik tengah antara node i-1 dan i
        double kp = k(xi + dx / 2);     // Konduktivitas di titik tengah antara node i dan i+1
        
        a[i] = km;                      // Koefisien sub-diagonal
        c[i] = kp;                      // Koefisien super-diagonal
        b[i] = -(km + kp);              // Koefisien diagonal utama
        d[i] = -q(xi) * dx * dx;        // Suku sumber panas pada vektor hasil
        
        // Memasukkan pengaruh syarat batas (boundary conditions) ke vektor d
        if (i == 0)      d[i] -= a[i] * T0;    // Untuk persamaan pertama, pindahkan suku T0 ke kanan
        if (i == n - 1)  d[i] -= c[i] * TL;    // Untuk persamaan terakhir, pindahkan suku TL ke kanan
    }

    // --- 4. Solusi Sistem dengan Algoritma Thomas ---
    // Memanggil fungsi thomas untuk menyelesaikan sistem Ax=d dan menyimpan hasilnya di vektor T
    thomas<double>(a, b, c, d, T);

    // --- 5. Tampilkan Hasil di Terminal (Opsional) ---
    cout << "\n--- Distribusi Temperatur (Hasil Numerik) ---\n";
    cout << fixed << setprecision(6); // Atur format output agar menampilkan 6 angka di belakang koma
    cout << "x = " << 0.0 << ", T = " << T0 << "\n"; // Tampilkan temperatur di batas kiri
    for (int i = 0; i < n; ++i) { // Loop untuk menampilkan temperatur di setiap titik nodal internal
        cout << "x = " << (i + 1) * dx << ", T = " << T[i] << "\n";
    }
    cout << "x = " << L << ", T = " << TL << "\n"; // Tampilkan temperatur di batas kanan

    // --- 6. Simpan Hasil ke File CSV untuk Plotting ---
    ofstream fout("heat_conduction_results.csv"); // Membuat objek file output stream
    fout << "position,temperature\n"; // Menulis header untuk file CSV
    fout << fixed << setprecision(6); // Mengatur presisi angka untuk file
    fout << 0.0 << "," << T0 << "\n"; // Menulis data titik pertama (batas kiri)
    for (int i = 0; i < n; ++i) { // Loop untuk menulis data semua titik internal
        fout << (i + 1) * dx << "," << T[i] << "\n";
    }
    fout << L << "," << TL << "\n"; // Menulis data titik terakhir (batas kanan)
    fout.close(); // Menutup file setelah selesai menulis
    cout << "\n[INFO] Data hasil perhitungan tersimpan di 'heat_conduction_results.csv'\n";

    // --- 7. Buat Skrip Gnuplot Secara Otomatis ---
    ofstream gp("plot_heat_conduction.gp"); // Membuat objek file untuk skrip gnuplot
    gp << "set datafile separator ','\n"; // Atur gnuplot untuk membaca data yang dipisahkan koma
    gp << "set title 'Distribusi Temperatur Steady-State 1D'\n"; // Judul grafik
    gp << "set xlabel 'Posisi (m)'\n"; // Label sumbu-x
    gp << "set ylabel 'Temperatur (K)'\n"; // Label sumbu-y
    gp << "set grid\n"; // Aktifkan grid pada grafik
    gp << "set key top left\n"; // Posisikan legenda di kiri atas
    gp << "plot 'heat_conduction_results.csv' using 1:2 with linespoints pt 7 lc rgb 'red' title 'Temperatur Numerik'\n"; // Perintah plot
    gp << "pause -1 'Tekan tombol apa saja untuk menutup jendela plot'\n"; // Jaga jendela plot tetap terbuka
    gp.close(); // Tutup file skrip
    cout << "[INFO] Skrip Gnuplot tersimpan di 'plot_heat_conduction.gp'\n";

    // --- 8. Jalankan Gnuplot untuk Menampilkan Grafik ---
    // PENTING: Sesuaikan path ini dengan lokasi instalasi Gnuplot di komputer Anda!
    const char* gnuplotPath = "\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\"";
    // Membuat string perintah untuk dieksekusi oleh sistem
    string cmd = string(gnuplotPath) + " -p plot_heat_conduction.gp";

    cout << "[AKSI] Mencoba menjalankan Gnuplot...\n";
    // Menjalankan perintah menggunakan system(). Opsi -p (persistent) membuat jendela plot tidak langsung tertutup.
    if (system(cmd.c_str()) != 0) {
        // Menampilkan pesan error jika gnuplot tidak berhasil dijalankan
        cerr << "[WARNING] Gagal menjalankan Gnuplot. Pastikan Gnuplot terinstal dan path sudah benar.\n";
    }

    return 0; // Mengakhiri program
}