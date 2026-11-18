# Numerical Methods in Physics — Matrix Diagonalization Using the Power Method and Gram–Schmidt

**AGH University of Krakow · Faculty of Physics and Applied Computer Science**  
**Instructor:** Hamzeh Khanpour  

> **These examples teach individual numerical ingredients. We must implement the complete assessed laboratory project independently.**

The official documents take priority for the assessed work: [Power method with Gram–Schmidt (PDF)](https://galaxy.agh.edu.pl/~chwiej/nmp/labs/nmp_diag_power_method.pdf) and [Iterative diagonalization lecture (PDF)](https://galaxy.agh.edu.pl/~chwiej/nmp/lectures/nmp_diag_iterative.pdf), both authored by Tomasz Chwiej.

## 1. Study order and self-checkable examples

Read the indicated slides, calculate the tiny problem by hand, compile/run the corresponding example, and compare numerical values. All source files use only standard C++17 (no BLAS, LAPACK, CERN ROOT or Eigen).

| ID | Source | Slide pages | We learn | Expected checks |
|---|---|---|---|---|
| E01 | `01_eigenpairs_2x2.cpp` | S11–S16 | Exact eigenpairs of B | λ=3 and 1; both residuals ≈ 0; orthogonality ≈ 0 |
| E02 | `02_tridiagonal_3x3.cpp` | S07–S10, S17–S18 | Three-point stencil matrix | T3(1,2,3)=(0,0,-4) |
| E03 | `03_vector_matrix_operations.cpp` | S19–S22 | Dot product, norm, dense multiplication | u·w=8, ||u||=√14, T3u=(0,0,-4) |
| E04 | `04_one_power_step.cpp` | S27–S28 | One normalized power step | y=(2,1); ||y||=√5; x1=y/√5 |
| E05 | `05_power_method_2x2_history.cpp` | S23–S30, S35–S36 | Complete 2×2 toy iteration and CSV | R0=2, R1=2.8, R2≈2.97561; approach 3 |
| E06 | `06_rayleigh_residual.cpp` | S31–S34 | Quotient plus eigenpair residual | R(2,1)=2.8, normalized residual=0.6 |
| E07 | `07_gram_schmidt_2d.cpp` | S37–S42 | 2D projection and normalization | y⊥=(1,-1), q1·q2≈0 |
| E08 | `08_projected_power_toy.cpp` | S40–S46 | Projected power iteration, separate 3×3 toy | After removing λ=4 direction, approach λ=2 |
| E09 | `09_spectral_reconstruction_2x2.cpp` | S47–S51 | Rank-one spectral reconstruction and inverse | B=[[2,1],[1,2]], B⁻¹=[[2,-1],[-1,2]]/3 |
| E10 (optional) | `10_optional_equal_magnitude_failure.cpp` | S26, S35, S56 | OPTIONAL — equal-magnitude failure | D=diag(1,-1); R=0 yet residual=1 |


## 2. Environment and build instructions

Requirements: a terminal, `make`, `g++` supporting C++17; Python 3 for tests, and Python 3 plus Matplotlib for optional figures. This does **not** claim preinstalled support on AGH classroom PCs or Taurus. Check first:

```sh
g++ --version
make --version
python3 --version
python3 -c 'import matplotlib; print(matplotlib.__version__)'
```

From the extracted `student_package` directory, use:

```sh
make help
make standard             # builds E01–E09
make check-standard       # independent numeric checks
make optional             # builds E10
make check-optional
make check-all
make plot                 # generates E05 toy CSV and PNG if absent
make check-plot
make clean                # removes only generated binaries
```

Build/run any program independently. Do not combine files, since each example has its own `main()`:

```sh
mkdir -p bin
g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 examples/01_eigenpairs_2x2.cpp -o bin/01_eigenpairs_2x2
./bin/01_eigenpairs_2x2
```

Or build and run every source as follows:

```sh
make bin/01_eigenpairs_2x2 && ./bin/01_eigenpairs_2x2
make bin/02_tridiagonal_3x3 && ./bin/02_tridiagonal_3x3
make bin/03_vector_matrix_operations && ./bin/03_vector_matrix_operations
make bin/04_one_power_step && ./bin/04_one_power_step
make bin/05_power_method_2x2_history && ./bin/05_power_method_2x2_history --output data/my_power_trial.csv
make bin/06_rayleigh_residual && ./bin/06_rayleigh_residual
make bin/07_gram_schmidt_2d && ./bin/07_gram_schmidt_2d
make bin/08_projected_power_toy && ./bin/08_projected_power_toy
make bin/09_spectral_reconstruction_2x2 && ./bin/09_spectral_reconstruction_2x2
make bin/10_optional_equal_magnitude_failure && ./bin/10_optional_equal_magnitude_failure  # optional
```

For E05, the default output is `data/power_history.csv`. If that file already exists, the program refuses to overwrite it: choose another filename with `--output data/my_trial.csv`, or explicitly add `--overwrite`. Do not rerun E05 without changing the filename after `make plot` has already generated it.

## 3. Power-iteration plotting

The complete tiny E05 example takes the independent matrix

\[
B=\begin{pmatrix}2&1\\1&2\end{pmatrix},\quad x^{(0)}=(1,0)^T.
\]

Its CSV columns are `iteration,rayleigh,residual_norm`. Iteration 0 describes the *starting* vector; iteration 1 is the first normalized multiply. The Rayleigh quotient approaches the exact dominant eigenvalue 3, and the eigenpair residual tends to zero.

```sh
make plot
# data/power_history.csv
# plots/power_history.png
make check-plot
```

Or plot your own E05 CSV under a distinct filename:

```sh
python3 plotting/plot_power_history.py --input data/my_trial.csv --output plots/my_trial.png
```

The Python program validates the CSV schema, finite numbers, and index sequence; it does not modify existing output unless `--overwrite` is requested. The E05 plot is not a plot of the assessed 6×6 matrix. On a machine without Matplotlib, all numerical C++ programs still compile and run; Python plotting is a separate optional dependency.

## 4. Our official laboratory work (not implemented for us)

The assigned finite-difference matrix is **real symmetric**, of dimension **n=6**, with diagonal **−2**, first off-diagonals **+1**, all other entries 0, and **Δ=1**. The handout prescribes a **two-dimensional array** for the matrix, not CSR. We use **itmax=150** and the prescribed relative eigenvalue tolerance **ε=1e−8**, with suitable numerical safeguards.

1. Build and display the entire assigned 6×6 matrix; check symmetry and both endpoint rows.
2. Independently integrate the scalar product, matrix–vector product, normalization, power iteration and Gram–Schmidt projection into our own eigenpair finder. Save successive eigenvalue estimates to separate files for each eigenpair.
3. Reconstruct and save the original matrix from all computed eigenpairs: `A_num = Σ λ_k q_k q_kᵀ`.
4. Compute and save the spectral inverse: `A_num⁻¹ = Σ (1/λ_k) q_k q_kᵀ`, provided eigenvectors are orthonormal and eigenvalues are nonzero.
5. At home: combine eigenvalue histories on one plot; study convergence versus eigenvalue index; try ten largest-absolute-magnitude eigenvalues for n=200; repeat reconstruction with ε=1e−4; explain inverse symmetry; turn off Gram–Schmidt and discuss repeated convergence to dominant modes.

**Numerical caution:** The basic power method targets the eigenvalue with the largest **absolute magnitude**, not necessarily the most positive eigenvalue. For this assignment the exact six eigenvalues are negative. An eigenvalue estimate that barely changes does not, by itself, establish a small eigenpair residual. Always report when `itmax` stops a run without meeting the requested tolerance. For n=200 the spectral gap is much smaller, so 150 steps may be inadequate.

## 5. If something goes wrong

- **Incorrect tridiagonal array:** compare every entry of `T3` with E02; ensure no invalid neighbor index on the first/last rows.
- **Unstable division:** check that the norm of each projected vector is finite and nonzero before normalization.
- **Wrong eigenvalue ordering:** compare magnitudes `|λ|`, not just algebraic values.
- **Repeated eigenvector:** ensure Gram–Schmidt projection is applied during **every** inner power iteration.
- **Misleading convergence:** inspect `||Aq−λq||₂` in addition to consecutive eigenvalue changes.
- **Wrong spectral inverse:** invert eigenvalues, not individual entries of `A`; only do so for nonzero eigenvalues and normalized eigenvectors.
- **Plot fails:** verify CSV column headers and sequential iteration indices; check for a missing Matplotlib installation. Exact zero residuals should not be replaced by artificial positive data just to enable log plotting.
- **Unexpected output-file conflict:** create a new filename or deliberately pass `--overwrite`.

The tests check numerical values, not merely `PASS` strings. Numerical values may vary at the last floating-point digits across computers.

## 6. Reference and responsibility

Tomasz Chwiej, *Diagonalization of matrix with power method and Gram–Schmidt orthogonalization* (8 April 2025), Sections 1–4, and *Diagonalization with iterative methods*, pp. 1–6. The pedagogical architecture and visual presentation style follow the instructor-supplied earlier SOR package. Full independent assessed work remains our responsibility.
