# fplll #

fplll contains implementations of several lattice algorithms. The implementation relies on floating-point orthogonalization, and LLL [LLL82] is central to the code, hence the name.  


It includes implementations of floating-point LLL reduction algorithms [NS09,MSV09], offering different speed/guarantees ratios. It contains a 'wrapper' choosing the estimated best sequence of variants in order to provide a guaranteed output as fast as possible [S10]. In the case of the wrapper, the succession of variants is oblivious to the user. 

It includes an implementation of the BKZ reduction algorithm [SE94], including the BKZ-2.0 [CN11] improvements (extreme enumeration pruning, pre-processing of blocks, early termination).

It also includes a floating-point implementation of the Kannan-Fincke-Pohst algorithm [K83,FP85] that finds a shortest non-zero lattice vector. For the same task, the GaussSieve algorithm [MV10] is also available in fplll. Finally, it contains a variant of the enumeration algorithm that computes a lattice vector closest to a given vector belonging to the real span of the lattice.

fplll is distributed under the [GNU Lesser General Public License](COPYING) (either version 2.1 of the License, or, at your option, any later version) as published by the Free Software Foundation.

## Dependencies ##

### Required ###

- GNU MP 4.2.0 or higher [http://gmplib.org/](http://gmplib.org/)
- MPFR 2.3.0 or higher, COMPLETE INSTALLATION [http://www.mpfr.org/](http://www.mpfr.org/)
- autotools 2.61 or higher

If GMP and/or MPFR include and lib files are not in the default directories `/usr/include` and
`/usr/lib`, you have to set the environment variables `CFLAGS` and `LDFLAGS` for instance through the
configure command line

```./configure CPPFLAGS="-I/mpfrinclude -I/gmpinclude" LDFLAGS="-L/mpfrlib -L/gmplib"```

or

```./configure CPPFLAGS="-I/mpfrinclude -I/gmpinclude $CPPFLAGD" LDFLAGS="-L/mpfrlib -L/gmplib $LDFLAGS"```

if these variables already exist in your environment. This should be modified soon for using
standard `--with-gmp` and `--with-mpfr` package specifications. The same philosophy applies to the
(optional) QD library.

### Optional ###
- QD 2.3.15 or higher (a C++/Fortran-90 double-double and quad-double package), compile and install
  the shared library (e.g., `./configure --enable-shared=yes`).
  [http://crd-legacy.lbl.gov/~dhbailey/mpdist/](http://crd-legacy.lbl.gov/~dhbailey/mpdist/)

## Installation ##

You should downloaded the source code from github and then run

    ./autogen.sh

which generates the `./configure` script used to configure fplll by calling the appropriate
autotools command.

Then, to compile and install type

	./configure
	make
	make install			# (as root)

You can remove the program binaries and object files from the source code directory by typing `make
clean`. To also remove the files that `./configure` created (so you can compile the package for a
different kind of computer), type `make distclean`.  By default, `make install` installs the package
commands under `/usr/local/bin`, include files under `/usr/local/include`, etc.  You can specify an
installation directory name other than `/usr/local` by giving `./configure` the option
`--prefix=dirname`.  Run `./configure --help` for further details.

## Check ##

Type

	make check


## How to use ##

Executable files `fplll` and `latticegen` are installed in the directory
`/usr/local/bin`. If you type 'make check', it will also generate the executable file `llldiff`.
(Note that the programs generated by 'make' in the `src/` directory are
only wrappers to the programs in `src/.libs/`).

### latticegen ###

`latticegen` is a utility for generating matrices (rows form input
lattice basis vectors).

The options are:

* `r` `d` `b` : generates a knapsack style matrix of dimension d x (d+1) and b bits.
* `s` `d` `b` `b2` : generates a simdioph matrix.
* `u` `d` `b` : generates an uniform matrix.
* `n` `d` `b` `q` : generates an ntru like matrix.
* `N` `d` `b` `q` : generates an ntru like matrix.
* `a` `d` `f` : generates an ajtai style matrix.
* `A` `d` : generates an ajtai style matrix. Also requires d coefficients.

The matrix is printed in stdout.

Note that by default, the random bits always use the same seed,
to ensure reproducibility. You can change the seed with the option
``` -randseed <integer>``` or use the current time (in seconds)
``` -randseed time```

If you use this option, it must be the first one on the command line.

### fplll ###

`fplll` does LLL, BKZ or SVP on a matrix (considered as a set of row vectors) given in stdin or in a
file as parameter.

The options are:

* `-a lll` : LLL-reduction (default).
* `-a bkz` : BKZ-reduction.
* `-a svp` : print a shortest vector of the lattice.
* `-r` `size`, `-c` `size` : ignored, provided for compatibility with previous versions of fplll.

Options for LLL-reduction:

* `-d delta` :     δ (default=0.99)
* `-e eta` :       η (default=0.51)
* `-l lovasz` :    if !=0 Lovasz's condition. Otherwise, Siegel's condition (default: Lovasz)
* `-p precision` : precision of the floating-point arithmetic, works only with `-f mpfr`.

* `-f mpfr` : sets the floating-point type to MPFR (default if `m=proved`).
* `-f dd` : sets the floating-point type to double-double.
* `-f qd` : sets the floating-point type to quad-double.
* `-f dpe` : sets the floating-point type to DPE (default if `m=heuristic/heuristicearly`).
* `-f double` : sets the floating-point type to double (default if `m=fast/fastearly`).
* `-f longdouble` : sets the floating-point type to long double.

* `-z int` : sets the integer type to int.
* `-z mpz` : sets the integer type to mpz, the integer type of GMP (default).
* `-z double` : sets the integer type to double.

* `-m wrapper` : uses the wrapper. (default if `z=mpz`)
* `-m fast` : uses the fast method, works only with `-f double`.
* `-m fastearly` : uses the fast method with early reduction, works only with `-f double`.
* `-m heuristic` : uses the heuristic method.
* `-m heuristicearly` : uses the heuristic method with early reduction.
* `-m proved` : uses the proved version of the algorithm.

With the wrapper or the proved version, it is guaranteed that the basis is LLL-reduced with δ'=2×δ-1
and η'=2×η-1/2. For instance, with the default options, it is guaranteed that the basis is
(0.98,0.52)-LLL-reduced.

Options for BKZ-reduction:

* `-b blocksize`             Block size, mandatory, between 2 and the number of rows.
* `-f float_type`            Same as LLL (`-p` is required if `float_type=mpfr`)
* `-p precision`             Precision of the floating-point arithmetic with `-f mpfr`
* `-bkzmaxloops loops`       Maximum number of full loops.
* `-bkzmaxtime time`         Stop after `time` seconds (up to loop completion).
* `-bkzautoabort`            Heuristic, stop when the average slope of log(||b_i*||) does not decrease fast enough.
* `-bpre blocksize`          Pre-processing block size. Between 2 and the block size.
* `-bkzlinearpruning level`  Enables linear pruning in enumeration, such that the last level steps drop with slope -1/blockSize
* `-bkzdumgso file_name`    Dumps the log(||b_i*||)'s in specified file.

### llldiff ###

`llldiff` compares two bases (b1,...,bd) and (c1,...c_d'): they are considered
equal iff d=d' and for any i, bi = +- ci.

### How to use as a library ###

See [API documentation](library.md).

### Alternative interfaces ###

- fplll is included in [Sage](http://sagemath.org), see documentation for [IntegerMatrix](http://doc.sagemath.org/html/en/reference/matrices/sage/matrix/matrix_integer_dense.html) and [IntegerLattice](http://doc.sagemath.org/html/en/reference/modules/sage/modules/free_module_integer.html).
- [fpylll](https://github.com/malb/fpylll) is a stand-alone Python interface for fplll.


## Examples ##

1. LLL reduction

   ``` 
   ./latticegen r 10 1000 | ./fplll
   ``` 

2. Fileinput for reduction. If the file `matrix` contains

   ``` 
   [[ 10 11]
   [11 12]]
   ``` 

   then

   ``` 
   ./fplll matrix
   ```

   produces

   ``` 
   [[0 1 ]
    [1 0 ]
   ]
   ``` 

3. Random generator

   ``` 
   ./latticegen -randseed 1234 r 10 1000 | ./fplll
   ./latticegen -randseed time u 10 16 | ./fplll
   ``` 
	
[//]: # (4. BKZ reduction)

[//]: # (5. Solving SVP)

4. Solving SVP

   ```
   ./latticegen r 30 3000 | ./fplll -a svp
   ```

## Bibliography ##

[CN11] Y. Chen and P. Q. Nguyen. BKZ 2.0: Better Lattice Security Estimates. ASIACRYPT 2011: 1-20

[K83] R. Kannan.  Improved algorithms for integer programming and related lattice problems. STOC 1983, 99-108

[FP85] U. Fincke and M. Pohst. Improved methods for calculating vectors of short length in a lattice, including a complexity analysis.
Math. Comp., 44(170):463–471 (1985)

[LLL82] A. K. Lenstra, H. W. Lenstra, Jr. and L. Lovasz. Factoring polynomials with rational coefficients. Math. Ann., 261: 515–534 (1982)

[MSV09] I. Morel, D. Stehle and G. Villard.
H-LLL: using Householder inside LLL. ISSAC 2009: 271-278

[MV10] D. Micciancio and P. Voulgaris.
Faster Exponential Time Algorithms for the Shortest Vector Problem. SODA 2010: 1468-1480

[NS09] P. Q. Nguyen and D. Stehle. An LLL Algorithm with Quadratic Complexity. SIAM J. Comput. 39(3): 874-903 (2009)

[S10] D. Stehle. Floating-Point LLL: Theoretical and Practical Aspects.  The LLL Algorithm 2010: 179-213

[SE94] C.-P. Schnorr and M. Euchner. Lattice basis reduction: Improved practical algorithms and solving subset sum problems. 
Math. Program. 66: 181-199 (1994)



## Credit ##

### Maintainers ###

fplll is currently maintained by:

- Martin Albrecht, <martinralbrecht@googlemail.com>
- Damien Stehle, <damien.stehle@gmail.com>
- Shi Bai, <shi.bai@gmail.com>

### Contributors ###

The following people have contributed to fplll:

- Martin Albrecht
- Shi Bai
- David Cade
- Xavier Pujol
- Damien Stehle
- Joop van de Pol
- Michael Walter

Please add yourself here if you make a contribution.

### Acknowledgments ###

- Patrick Pelissier and Paul Zimmermann for `dpe`.

- David H. Bailey for QD.

- Sylvain Chevillard, Christoph Lauter and Gilles Villard for the `configure/make/make install`
packaging.

- Timothy Abbott, Michael Abshoff, Bill Allombert, John Cannon, Sylvain Chevillard, Julien Clement,
Andreas Enge, Jean-Pierre Flori, Laurent Fousse, Guillaume Hanrot, Jens Hermans, Jerry James,
Christoph Lauter, Andrew Novocin, Willem Jan Palenstijn, Patrick Pelissier, Michael Schneider,
Thiemo Seufer, Allan Steel, Gilles Villard and Paul Zimmermann for their support and for many
suggestions that helped debugging and improving this code.

### How to cite ###

	@unpublished{fplll,
	    Note = {Available at \url{http://perso.ens-lyon.fr/damien.stehle}},
	    Title = {{fplll-4.0}, a floating-point {LLL} implementation},
	    Author = {Albrecht, M. and Bai, S. and Cad{\'e}, D. and Pujol, X. and Stehl{\'e}, D.}
	}

## New releases and bug reports ##

New releases will be announced on [https://groups.google.com/forum/#!forum/fplll-devel](https://groups.google.com/forum/#!forum/fplll-devel)

Bug reports may be sent to [https://groups.google.com/forum/#!forum/fplll-devel](https://groups.google.com/forum/#!forum/fplll-devel) or via
[https://github.com/fplll/fplll/issues](https://github.com/fplll/fplll/issues). 
