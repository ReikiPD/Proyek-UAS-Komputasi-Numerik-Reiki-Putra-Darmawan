# Solver Konduksi Panas 1D dengan Konduktivitas Variabel & Sumber Panas Non-Uniform

Disusun oleh:
**Reiki Putra Darmawan**
**2206062882**

---

## Deskripsi Program

Program ini adalah sebuah solver yang dibuat menggunakan C++ untuk menyelesaikan masalah **konduksi panas satu dimensi (1D) dalam kondisi steady-state**. Program ini secara khusus dirancang untuk menangani skenario yang lebih kompleks di mana:

1.  **Konduktivitas termal k bervariasi** secara linier di sepanjang batang.
2.  Terdapat **sumber panas internal q yang non-uniform** (distribusi sinusoidal).

### Metode Numerik

Solusi diperoleh melalui dua langkah utama:

1.  **Metode Beda Hingga (Finite Difference Method):** Persamaan diferensial di atas didiskretisasi menjadi sebuah sistem persamaan linear dengan matriks koefisien berbentuk **tridiagonal**.
2.  **Algoritma Thomas (TDMA):** Sistem linear tridiagonal tersebut diselesaikan secara efisien menggunakan Algoritma Thomas, yang merupakan varian dari eliminasi Gauss yang dioptimalkan.

## Fitur

- **Input Interaktif:** Pengguna dapat memasukkan semua parameter fisis secara langsung melalui terminal.
- **Perhitungan Cepat:** Menggunakan Algoritma Thomas untuk penyelesaian sistem linear yang efisien.
- **Penyimpanan Data:** Hasil perhitungan (posisi dan temperatur) secara otomatis disimpan dalam format `.csv` (`heat_conduction_results.csv`) untuk analisis lebih lanjut.
- **Visualisasi Otomatis:** Program secara otomatis membuat skrip Gnuplot (`plot_heat_conduction.gp`) dan menampilkannya dalam bentuk grafik.

## Cara Menggunakan

### 1. Prasyarat

- **Kompiler C++:** Seperti `g++` (MinGW di Windows atau GCC di Linux).
- **Gnuplot:** Perangkat lunak Gnuplot harus terinstal dan path ke `gnuplot.exe` di dalam kode harus sudah benar.
  > **PENTING:** Buka file `.cpp` dan sesuaikan path pada baris berikut:
  > `const char* gnuplotPath = "\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\"";`

### 2. Kompilasi

Buka terminal atau command prompt, navigasi ke folder proyek, dan kompilasi program dengan perintah:

```sh
g++ VariableConductivityHeat1D.cpp -o solver
```

## 3. Ekesekusi

Jalankan program yang sudah dikompilasi:

```sh
./solver
```

Program akan meminta Anda untuk memasukkan parameter-parameter yang dibutuhkan.

## Struktur File

- solver.exe // File eksekusi setelah kompilasi (Windows)
- heat_conduction_results.csv // (Dihasilkan) Data hasil perhitungan
- plot_heat_conduction.gp // (Dihasilkan) Skrip Gnuplot
- main.cpp // Kode sumber utama
