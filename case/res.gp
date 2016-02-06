set multiplot layout 2, 1
set tmargin 2

set style data line
set xlabel 'Time(s)'
set ylabel 'ARPS'
#unset cblabel
set yzeroaxis
#set term postscript eps enhanced "Arial" 24
set term png
set output "res2.png"
#set yrange [0:80000]
set xrange[:300]

plot "tmp" using 1:2 notitle

set xrange [:7200]
plot "tmp" using 1:2 notitle
unset multiplot
