# One independent main() per example. No wildcard link of all .cpp files.
CXX ?= g++
PYTHON ?= python3
CPPFLAGS ?=
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2
LDFLAGS ?=
LAPACKE_CPPFLAGS ?=
LAPACKE_LIBS ?= -llapacke -llapack -lblas -lm
STANDARD = 01_small_linear_system 02_row_major_storage 03_gaussian_elimination 04_triangular_substitution 05_partial_pivoting 09_toy_polynomial_output
LAPACKE = 06_lapacke_lu_factorization 07_lapacke_solve 08_determinant_and_residual
OPTIONAL = 10_conditioning_experiment
STANDARD_BINS = $(addprefix bin/,$(STANDARD))
LAPACKE_BINS = $(addprefix bin/,$(LAPACKE))
OPTIONAL_BINS = $(addprefix bin/,$(OPTIONAL))
.PHONY: all core standard lapacke optional check-core check-standard check-lapacke check-optional check-all plot clean help
all: core
core: standard lapacke
standard: $(STANDARD_BINS)
lapacke: $(LAPACKE_BINS)
optional: $(OPTIONAL_BINS)
bin:
	mkdir -p bin
$(STANDARD_BINS): bin/%: examples/%.cpp | bin
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< $(LDFLAGS) -o $@
$(LAPACKE_BINS) $(OPTIONAL_BINS): bin/%: examples/%.cpp | bin
	$(CXX) $(CPPFLAGS) $(LAPACKE_CPPFLAGS) $(CXXFLAGS) $< $(LDFLAGS) -o $@ $(LAPACKE_LIBS)
check-standard: standard
	$(PYTHON) tests/check_examples.py --group standard
check-lapacke: lapacke
	$(PYTHON) tests/check_examples.py --group lapacke
check-core: core
	$(PYTHON) tests/check_examples.py --group core
check-optional: optional
	$(PYTHON) tests/check_examples.py --group optional
check-all: core optional
	$(PYTHON) tests/check_examples.py --group all --plot
# User data is never created from hidden expected values or silently replaced.
plot:
	$(PYTHON) plotting/plot_toy_polynomial.py data/toy_run01.csv --output plots/toy_run01.png
clean:
	rm -f $(STANDARD_BINS) $(LAPACKE_BINS) $(OPTIONAL_BINS)
help:
	@printf '%s\n' 'make core: all NINE mandatory examples (requires LAPACKE)' 'make standard: E01-E05 and E09 only; no external numerical library' 'make lapacke: E06-E08; real LAPACKE, no mock fallback' 'make optional: E10; requires LAPACKE' 'make check-standard / check-lapacke / check-core / check-optional' 'make check-all: all ten plus CSV/plot failure-case tests' 'make plot: read data/toy_run01.csv after running E09' 'make clean: remove only named binaries, never data/plots' 'Override LAPACKE_CPPFLAGS, LDFLAGS and LAPACKE_LIBS for a verified installation; clean first if flags change.'
