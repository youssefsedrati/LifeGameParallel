set terminal png
set output 'speedupOMP.png'

unset logscale
set key top left

set xtics (256, 512, 1024, 2048, 4096, 8192, 16384)
set ytic auto
set grid mxtics mytics
set ylabel "Speed-up par rapport au séquentiel"
set xlabel "Nombre de threads utilisés"
plot "data/speedupOMP.dat" using 1:($2/$3) title 'Damier 8192x8192, schedule static' with linespoints,\
	 "data/speedupOMP.dat" using 1:($5/$6) title 'Damier 32768x32768, schedule static' with linespoints,\
	 x notitle with line