#!/usr/bin/env python3
"""Plot the five rows actually written by E09."""
from __future__ import annotations

import argparse
import csv
import math
from pathlib import Path
import sys


def read_toy_csv(path: Path) -> tuple[list[float], list[float]]:
    """Require E09's x,y schema, five finite rows, and the documented grid."""
    with path.open(newline="", encoding="utf-8") as stream:
        reader = csv.DictReader(stream)
        if reader.fieldnames != ["x", "y"]:
            raise ValueError("Expected exactly the CSV header x,y")
        rows = list(reader)
    if len(rows) != 5:
        raise ValueError("Expected exactly five toy rows; adapt a separate reader for your project")
    xs, ys = [], []
    for number, row in enumerate(rows, start=2):
        if None in row or row.get("x") is None or row.get("y") is None:
            raise ValueError(f"Malformed CSV row {number}")
        x, y = float(row["x"]), float(row["y"])
        if not (math.isfinite(x) and math.isfinite(y)):
            raise ValueError(f"Nonfinite value on row {number}")
        xs.append(x)
        ys.append(y)
    if xs != [0.0, 0.5, 1.0, 1.5, 2.0]:
        raise ValueError("Expected E09's increasing grid 0,0.5,1,1.5,2")
    return xs, ys


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input", type=Path, help="Existing E09 CSV file")
    parser.add_argument("--output", required=True, type=Path, help="Destination .png or .pdf")
    parser.add_argument("--overwrite", action="store_true", help="Explicitly allow replacing the plot")
    args = parser.parse_args()
    try:
        if args.output.suffix.lower() not in {".png", ".pdf"}:
            raise ValueError("Output must end in .png or .pdf")
        if args.input.resolve() == args.output.resolve():
            raise ValueError("Input and output must be different files")
        if args.output.exists() and not args.overwrite:
            raise ValueError("Plot exists; choose a new name or explicitly pass --overwrite")
        if not args.output.parent.is_dir():
            raise ValueError("Output parent does not exist; create plots/ first")
        xs, ys = read_toy_csv(args.input)
        try:
            import matplotlib
            matplotlib.use("Agg")  # Headless Linux / SSH: no display server needed.
            import matplotlib.pyplot as plt
        except ImportError as exc:
            raise RuntimeError("Matplotlib is required; ask the lab administrator or use a configured Python environment") from exc
        fig, ax = plt.subplots(figsize=(7, 4.2))
        ax.plot(xs, ys, marker="o", label="E09 CSV samples (straight-line joins)")
        ax.set(xlabel="x", ylabel="q(x)", title="Toy polynomial: q(x) = 1 + x - x²")
        ax.grid(True, alpha=0.3)
        ax.legend(loc="best")
        fig.tight_layout()
        fig.savefig(args.output, dpi=180)
        plt.close(fig)
        if args.output.stat().st_size == 0:
            raise RuntimeError("The output plot is empty")
        print(f"Read {len(xs)} actual rows from {args.input}; saved {args.output}")
        return 0
    except (OSError, ValueError, RuntimeError, csv.Error) as exc:
        print(f"Plot error: {exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
