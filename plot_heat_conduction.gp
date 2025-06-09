set datafile separator ','
set title 'Distribusi Temperatur Steady-State 1D'
set xlabel 'Posisi (m)'
set ylabel 'Temperatur (K)'
set grid
set key top left
plot 'heat_conduction_results.csv' using 1:2 with linespoints pt 7 lc rgb 'red' title 'Temperatur Numerik'
pause -1 'Tekan tombol apa saja untuk menutup jendela plot'
