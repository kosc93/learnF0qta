##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/Betriebssportgemeinschaft.png'

##### general settings #####
set datafile missing '0'
stats 'Betriebssportgemeinschaft.qtaf0sampled' using 2:3 nooutput
set xrange [0.332003:2.11921]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - Betriebssportgemeinschaft (rmse=0.544114)'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'

##### plot syllable bounds #####
set arrow from 0.332003,STATS_min_y-2 to 0.332003,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.404664,STATS_min_y-2 to 0.404664,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.804938,STATS_min_y-2 to 0.804938,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.156808,STATS_min_y-2 to 1.156808,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.260891,STATS_min_y-2 to 1.260891,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.531898,STATS_min_y-2 to 1.531898,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 2.119209,STATS_min_y-2 to 2.119209,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.332003,89.681977 to 0.404664,90.408591 nohead dt 4
set arrow from 0.404664,98.618519 to 0.804938,94.615781 nohead dt 4
set arrow from 0.804938,86.195115 to 1.156808,89.713819 nohead dt 4
set arrow from 1.156808,93.055337 to 1.260891,92.014511 nohead dt 4
set arrow from 1.260891,90.350383 to 1.531898,89.386075 nohead dt 4
set arrow from 1.531898,92.285515 to 2.119209,86.412412 nohead dt 4

###### plot curves from data points #####
plot 'Betriebssportgemeinschaft.semitonef0' using 2:3 title 'Original F0' with points lt 3 lc '#0000FF' lw 1,\
 'Betriebssportgemeinschaft.qtaf0' using 2:3 title 'Resynthesized F0' with points lt 1 lc black lw 2,\
 'Betriebssportgemeinschaft.qtaf0sampled' using 2:3 notitle with linespoints lt -1 lc black lw 1
