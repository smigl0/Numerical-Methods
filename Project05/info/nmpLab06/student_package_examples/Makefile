CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2
CORE := $(basename $(notdir $(wildcard examples/0[1-9]_*.cpp)))
OPT := 10_optional_equal_magnitude_failure
CORE_BINS := $(addprefix bin/,$(CORE))
.PHONY: help all standard optional check-standard check-optional check-all plot check-plot clean
all: standard
help:
	@printf 'make standard / optional / all\nmake check-standard / check-optional / check-all\nmake plot / check-plot\nmake clean (deletes only executables)\n'
standard: $(CORE_BINS)
optional: bin/$(OPT)
bin/%: examples/%.cpp
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) "$<" -o "$@"
check-standard: standard
	python3 tests/check_examples.py
check-optional: optional
	python3 tests/check_examples.py --optional
check-all: standard optional
	python3 tests/check_examples.py --all
plot: bin/05_power_method_2x2_history
	@if test -e data/power_history.csv; then echo 'Existing CSV preserved'; else ./bin/05_power_method_2x2_history --output data/power_history.csv; fi
	@if test -e plots/power_history.png; then echo 'Existing plot preserved'; else python3 plotting/plot_power_history.py --input data/power_history.csv --output plots/power_history.png; fi
check-plot: bin/05_power_method_2x2_history
	python3 tests/check_plot.py
clean:
	rm -f $(CORE_BINS) bin/$(OPT)
