unset key
set terminal pdf monochrome enhanced font "Helvetica,16"
set title 'Distribution of analysis cost'
set ylabel 'Number of function definitions'
set xlabel 'Execution time [ms]'
set xrange [-3:3]
set xtics ("0.001" -3, "0.01" -2, "0.1" -1, "1" 0, "10" 1, "100" 2, "1000" 3)
set size ratio 0.5
FILE='time.dat'
natlog(x)=log(x)/log(10)
bin(x,width)=width*floor(natlog(x)/width)+width/2.0
binwidth=0.1
plot FILE using (bin($1,binwidth)):(1) smooth freq with boxes