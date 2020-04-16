## FAST HISTOGRAM  
Computes the histogram of a two column input.  

### COMPILE  

```bash
$ make
```

## USAGE  
Runing the program without input will output this help:
```bash
Raul P. Pelaez 2020. Histogram utility.

  Given a two column input through stdin computes a weighted histogram.
  The first column of the output is the center position of each bin, the second contains the un normalized histogram (the sum of weigths falling inside each bin).

  USAGE:
    histogram -u [upper limit] -l [lower limit] -n [number of intervals]
    
  EXAMPLE:
   Histogram of an uniform distribution

   seq 1 10000 | awk '{print rand(), 1}' | histogram -l 0 -u 1 -n 10000 > flatHistogram.dat
```  
