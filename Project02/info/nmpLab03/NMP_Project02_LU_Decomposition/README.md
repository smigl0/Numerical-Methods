# Numerical Methods in Physics — LAB 2
## Solving Systems of Linear Equations Using LU Decomposition

**Hamzeh Khanpour · Faculty of Physics and Applied Computer Science · AGH University of Krakow**  
Release: 16 October 2026. Presentation: 56 pages. Core examples: E01–E09. Optional: E10.

Read the mathematical slides, predict a small example's result, run it, and explain why its checks pass. 
Then implement the official assignment independently in your own project directory.

## 1. Start in the extracted package root

The root contains this README, `examples/`, `plotting/`, `tests/`, and the Makefile. Open a **Linux terminal**. 
Change into the actual extracted directory, then run:

```bash
pwd
ls
ls examples
mkdir -p bin data plots
g++ --version
python3 --version
make help
```

Every `.cpp` contains one `main()`. Compile one file at a time. **Do not run `g++ examples/*.cpp`**: that attempts to link ten different programs together. Spaces in an extraction path are supported when the path is quoted in your `cd` command.

The PDF is `NMP_LAB02_LU_Decomposition.pdf`. The student project guide separates the official in-lab tasks from the home report. The editable TEX and instructor notes are retained separately by the instructor.

## 2. Dependencies and a real preflight

E01–E05 and E09 need a C++17 compiler and the standard library only. E06–E08 and optional E10 require a compatible real-double LAPACKE header and library. BLAS or the Fortran LAPACK library alone is **not** proof that the LAPACKE C interface is installed. The plotter requires Python 3 and Matplotlib; the public test runner needs only Python's standard library unless `--plot` is requested.

```bash
printf '#include <lapacke.h>\n' | g++ -x c++ -std=c++17 -fsyntax-only -
python3 -c "import matplotlib; print(matplotlib.__version__)"
```

The header test does not test linking. Build and run E06 and E07 below to test the actual library. On a conventional Linux development installation the flags are `-llapacke -llapack -lblas -lm`. A managed machine may need additional include/library paths or a provider-specific library. Ask the instructor or system administrator for its verified settings; do not install or change system packages without authorization.

**Validation environment:** all ten programs were really compiled and tested with GCC 14.2.0. The LAPACKE programs used the installed OpenBLAS 0.3.21 LAPACKE implementation through a provider-specific link route, not a mock. The conventional `-llapacke` route was unavailable in the authoring environment, so the default lines below are portable instructions conditional on that development installation, not a claim that this exact default link line was tested there. AGH classroom computers and Taurus were not accessed during production. Detailed provider paths and logs are retained in the instructor package.

## 3. Direct compilation commands — one per file

Run `mkdir -p bin data plots` first. E10 is optional. The source files also contain their corresponding build/run command in the opening comments.

### E01

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 examples/01_small_linear_system.cpp -o bin/01_small_linear_system
./bin/01_small_linear_system
```

### E02

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 examples/02_row_major_storage.cpp -o bin/02_row_major_storage
./bin/02_row_major_storage
```

### E03

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 examples/03_gaussian_elimination.cpp -o bin/03_gaussian_elimination
./bin/03_gaussian_elimination
```

### E04

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 examples/04_triangular_substitution.cpp -o bin/04_triangular_substitution
./bin/04_triangular_substitution
```

### E05

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 examples/05_partial_pivoting.cpp -o bin/05_partial_pivoting
./bin/05_partial_pivoting
```

### E06

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 examples/06_lapacke_lu_factorization.cpp -o bin/06_lapacke_lu_factorization -llapacke -llapack -lblas -lm
./bin/06_lapacke_lu_factorization
```

### E07

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 examples/07_lapacke_solve.cpp -o bin/07_lapacke_solve -llapacke -llapack -lblas -lm
./bin/07_lapacke_solve
```

### E08

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 examples/08_determinant_and_residual.cpp -o bin/08_determinant_and_residual -llapacke -llapack -lblas -lm
./bin/08_determinant_and_residual
```

### E09

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 examples/09_toy_polynomial_output.cpp -o bin/09_toy_polynomial_output
./bin/09_toy_polynomial_output data/toy_run01.csv
```

### E10 — optional

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 examples/10_conditioning_experiment.cpp -o bin/10_conditioning_experiment -llapacke -llapack -lblas -lm
./bin/10_conditioning_experiment
```

The E09 output path must be new. For a second run use another filename such as `data/toy_run02.csv`; the program deliberately refuses silent overwrite.

### A verified nonstandard installation

Use `-I` for a header directory and `-L` for a library directory. Library flags belong after the source/object being linked. A runtime search path may also be needed. Keep the integer ABI of the header and library compatible; the tested provider uses 32-bit `lapack_int`.

For an administrator-verified provider, override the Makefile variables rather than edit every source file:

```bash
make clean
make core optional \
  LAPACKE_CPPFLAGS="-I/path/to/verified/include" \
  LDFLAGS="-L/path/to/verified/lib -Wl,-rpath,/path/to/verified/lib" \
  LAPACKE_LIBS="-lverified_provider -lm"
```

The paths and provider name in this last block are explicit placeholders, not executable defaults. Use the actual values supplied for your machine. There is no automatic fake or handwritten fallback for a failed LAPACKE build.

## 4. Convenient Makefile workflow

```bash
make standard          # E01-E05 and E09 only
make check-standard
make core              # all NINE core examples; requires LAPACKE
make check-core
make optional          # optional E10
make check-optional
make check-all         # all ten, plus real CSV/plot and error tests
```

`make lapacke` and `make check-lapacke` select E06–E08. `make clean` removes only the named example binaries; it does not delete your data or plots. Make does not infer that compiler flags changed: run `make clean` before switching library providers, optimization options, or sanitizer settings. These examples are separate programs, not a single project executable.

## 5. Expected results and slide references

| Example | Source file | Expected observations |
|---|---|---|
| E01 | `01_small_linear_system.cpp` | x=[1,2]; residual_inf=0 |
| E02 | `02_row_major_storage.cpp` | packed=[1,2,3,4,5,6]; padding unchanged |
| E03 | `03_gaussian_elimination.cpp` | multipliers=[2,4,3]; U=[[2,1,1],[0,1,1],[0,0,2]]; x=[1,1,1] |
| E04 | `04_triangular_substitution.cpp` | y=[4,2,2]; x=[1,1,1] |
| E05 | `05_partial_pivoting.cpp` | one row swap; x=[1,2]; det=-2 |
| E06 | `06_lapacke_lu_factorization.cpp` | ipiv=[2,2]; packed LU=[3,4,1/3,2/3]; PA-LU near zero |
| E07 | `07_lapacke_solve.cpp` | factor/solve info=0; x=[1,2]; original residual near zero |
| E08 | `08_determinant_and_residual.cpp` | swaps=1; product=+2; det=-2; scale-aware residual near zero |
| E09 | `09_toy_polynomial_output.cpp` | CSV header x,y; y=[1,1.25,1,0.25,-1] |
| E10 (optional) | `10_conditioning_experiment.cpp` | five nonsingular solves, two stored-singular cases; response delta/stored_eps |

E01–E05 explain the mathematics without a numerical library. E03 deliberately omits pivoting only for its safe, fixed demonstration matrix; it is not a recommended general-purpose solver. E06 shows factorization and packed storage. E07 adds the solve. E08 adds determinant parity and a scale-aware residual. E09 shows the data-to-plot workflow using a different polynomial. E10 is optional preparation for interpreting sensitivity; it is not the official node-convergence experiment.

The library's floating-point values may differ in their last few digits. Compare with justified tolerances, not exact string equality. All programs print a `PASS E..` message only after their internal checks; on failure they return a nonzero exit code.

## 6. E09: produce actual data, then plot it

```bash
./bin/09_toy_polynomial_output data/toy_run01.csv
cat data/toy_run01.csv
python3 plotting/plot_toy_polynomial.py data/toy_run01.csv \
  --output plots/toy_run01.png
```

Expected CSV:

```csv
x,y
0,1
0.5,1.25
1,1
1.5,0.25
2,-1
```

`make plot` performs only the last step, reading `data/toy_run01.csv`. It does not synthesize the CSV if it is missing. The helper validates the header, finite numbers, five expected sample positions, and row structure. It plots the supplied y values; it does not replace them with a hidden polynomial calculation. The public tests separately check the numerical y values.

The figure displays five markers with **straight-line joins**. These joins are a visual guide, not a smooth interpolation or proof that the entire continuous curve was sampled. The plot is labelled as the unrelated teaching polynomial `q(x)=1+x-x²`, not the official assigned polynomial. To overwrite an existing figure deliberately, add `--overwrite`; without that option an existing output is protected. E09 itself never overwrites an existing CSV. Input/output directories must already exist. PNG and PDF figure output are supported.

## 7. Numerical checking and common errors

Preserve `A_original` and `b_original` before the library calls. `dgetrf` overwrites the matrix with packed LU; `dgetrs` overwrites the right-hand-side array with x. Residuals must use the **original** arrays.

For a tightly packed row-major n×n matrix, use `lda=n`. For a row-major n×1 right-hand side, use `nrhs=1` and `ldb=1`, **not n**. Store pivot entries in `lapack_int`. LAPACK pivot values are one-based row numbers; `ipiv[i]` is the row exchanged with row `i+1` at that elimination step, not a final permutation vector.

Our convention is `P A = L U`: apply the recorded row swaps in increasing step order to A. The Netlib description `A=P_N L U` uses the inverse permutation. Do not permute b manually before `dgetrs`; the routine applies the stored pivots. The unit diagonal of L is implicit in packed storage.

The determinant is `(-1)^s` times the diagonal product of U, with one swap counted for each `ipiv[i] != i+1`. After `dgetrf`, a positive `info` reports an exactly zero U pivot and the solve must be skipped. Zero `info` does not certify good conditioning. Check `dgetrs` status, finite results and residuals too. The basic pair does not perform iterative refinement or return a condition estimate.

A small residual means the equations are nearly satisfied; an ill-conditioned matrix can still yield large forward solution errors. Determinant magnitude alone is not a scale-independent conditioning diagnostic. See S40–S42 and the optional experiment on S55.

## 8. Run the public tests

```bash
python3 tests/check_examples.py --group core
python3 tests/check_examples.py --group all --plot
python3 tests/check_examples.py --group all --plot --log example_checks.json
```

Build the selected binaries first. A missing executable is reported as a failure, not silently skipped. The test runner executes real binaries, parses observations, checks numerical fixtures, and uses temporary directories for CSV/plot tests. It checks missing input, malformed rows, nonfinite values, wrong grid positions, and protected overwrite behavior. Existing student outputs are not used as hidden answer files. `--log` creates a JSON record of the actual commands and results; choose a suitable new log path to retain previous evidence.

## 9. Your assessed project remains yours

Use the official handout as the authoritative task definition. Your independent work includes constructing its six equations, using the prescribed nodes, solving, computing the pivot-corrected determinant, saving coefficients, plotting, and investigating the requested node changes. Do not submit a toy example as the project. The supplied examples intentionally contain no integrated implementation of the official 6×6 polynomial problem and no final assignment coefficients.

## Sources

- Tomasz Chwiej, official laboratory handout (12 March 2026), pp. 1–4: https://galaxy.agh.edu.pl/~chwiej/nmp/labs/nmp_sle_LU_shaping_polynomial.pdf
- Tomasz Chwiej, direct-methods lecture, especially pp. 9–13, 17–18, 22–25 and 32–35: https://galaxy.agh.edu.pl/~chwiej/nmp/lectures/nmp_sle_direct.pdf
- Official LAPACKE interface documentation: https://www.netlib.org/lapack/lapacke.html

