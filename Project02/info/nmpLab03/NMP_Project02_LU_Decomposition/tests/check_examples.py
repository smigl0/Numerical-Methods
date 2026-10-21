#!/usr/bin/env python3
"""Execute and independently check tiny fixtures."""
from __future__ import annotations

import argparse
import csv
import json
import math
from pathlib import Path
import re
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[1]
NAMES = [
    "01_small_linear_system", "02_row_major_storage", "03_gaussian_elimination",
    "04_triangular_substitution", "05_partial_pivoting", "06_lapacke_lu_factorization",
    "07_lapacke_solve", "08_determinant_and_residual", "09_toy_polynomial_output",
    "10_conditioning_experiment",
]
GROUPS = {"standard": [1,2,3,4,5,9], "lapacke": [6,7,8], "core": list(range(1,10)),
          "optional": [10], "all": list(range(1,11))}
EVIDENCE: list[dict] = []


def require(condition: bool, message: str) -> None:
    if not condition:
        raise RuntimeError(message)


def execute(command: list[str], expected_success: bool = True) -> str:
    run = subprocess.run(command, text=True, capture_output=True, timeout=30)
    EVIDENCE.append({"command": command, "returncode": run.returncode,
                     "stdout": run.stdout, "stderr": run.stderr,
                     "expected_success": expected_success})
    require((run.returncode == 0) == expected_success,
            f"Unexpected exit {run.returncode}: {' '.join(command)}\n{run.stdout}\n{run.stderr}")
    return run.stdout


def binary(number: int) -> str:
    path = ROOT / "bin" / NAMES[number-1]
    require(path.is_file(), f"Missing {path}; compile this example first")
    return str(path)


def values(output: str, label: str) -> list[float]:
    match = re.search(r"^" + re.escape(label) + r" = (.+)$", output, re.M)
    require(match is not None, f"Missing output label {label!r}")
    result = [float(v) for v in match.group(1).split()]
    require(all(math.isfinite(v) for v in result), f"Nonfinite output: {label}")
    return result


def close_vector(got: list[float], expected: list[float], tol: float = 1e-12) -> None:
    require(len(got) == len(expected), "Incorrect result length")
    require(all(math.isclose(g,e,rel_tol=tol,abs_tol=tol) for g,e in zip(got,expected)),
            f"Expected {expected}, got {got}")


def scalar(output: str, label: str) -> float:
    result = values(output, label)
    require(len(result) == 1, f"Expected scalar {label}")
    return result[0]


def small_example(number: int) -> None:
    output = execute([binary(number)])
    require(f"PASS E{number:02d}" in output, f"E{number:02d} missing self-check completion")
    if number in [1,5,7,8]:
        close_vector(values(output,"x"),[1,2])
        require(scalar(output,"residual_inf") <= 1e-12, "Residual too large")
    if number == 2:
        close_vector(values(output,"row_major"),[1,2,3,4,5,6])
        close_vector(values(output,"padded"),[1,2,3,-99,-99,4,5,6,-99,-99])
        entries = re.findall(r"A\((\d),(\d)\)=(\d) offset=(\d)",output)
        require(len(entries)==6,"Expected six address checks")
        for i,j,v,k in entries:
            i,j,v,k=map(int,(i,j,v,k))
            require(k==3*i+j and v==k+1,"Incorrect row-major address")
    if number == 3:
        close_vector(values(output,"U"),[2,1,1,0,1,1,0,0,2])
        close_vector(values(output,"rhs"),[4,2,2])
        close_vector(values(output,"x"),[1,1,1])
        require(all(t in output for t in ["m(1,0)=2","m(2,0)=4","m(2,1)=3"]),"Wrong multipliers")
        require(scalar(output,"residual_inf")<=1e-12,"Original-equation residual failed")
    if number == 4:
        close_vector(values(output,"y"),[4,2,2])
        close_vector(values(output,"x"),[1,1,1])
        require(scalar(output,"lower_residual")<=1e-12 and scalar(output,"upper_residual")<=1e-12,
                "Triangular residual failed")
    if number in [5,8]:
        close_vector(values(output,"det"),[-2])
        require(scalar(output,"swaps")==1,"Expected one interchange")
    if number == 6:
        require(scalar(output,"info")==0,"Factorization info failed")
        close_vector(values(output,"ipiv"),[2,2])
        close_vector(values(output,"lu"),[3,4,1/3,2/3])
        L,U,PA=[values(output,k) for k in ["L","U","PA"]]
        close_vector(L,[1,0,1/3,1]);close_vector(U,[3,4,0,2/3]);close_vector(PA,[3,4,1,2])
        product=[sum(L[2*i+k]*U[2*k+j] for k in range(2)) for i in range(2) for j in range(2)]
        close_vector(product,PA)
        require(scalar(output,"factor_residual")<=1e-12,"PA-LU check failed")
    if number == 7:
        require(scalar(output,"factor_info")==0 and scalar(output,"solve_info")==0,"Library info failed")
    if number == 8:
        close_vector(values(output,"diagonal_product"),[2])
        require(scalar(output,"scaled_residual")<=1e-12,"Scaled residual failed")
    if number == 10:
        delta=scalar(output,"delta")
        cases=[dict((k,float(v)) for k,v in re.findall(r"(\w+)=([^ ]+)",line))
               for line in output.splitlines() if line.startswith("CASE ")]
        require(len(cases)>0,"No nonsingular cases")
        shifts=[]
        for row in cases:
            require(all(math.isfinite(v) for v in row.values()),"Nonfinite conditioning output")
            eps=row["stored"]
            require(eps>0,"Invalid stored epsilon")
            close_vector([row["x0"],row["x1"]],[1-delta/eps,delta/eps],tol=1e-10)
            require(math.isclose(row["det"],eps,rel_tol=1e-10,abs_tol=0),"Wrong tiny determinant")
            require(math.isclose(row["shift"],delta/eps,rel_tol=1e-9,abs_tol=1e-15),"Wrong response to perturbation")
            require(row["residual"]<=1e-10*(1+max(abs(row["x0"]),abs(row["x1"]))),"Residual failed")
            shifts.append(row["shift"])
        require(all(a<b for a,b in zip(shifts,shifts[1:])),"Sensitivity did not increase")
        require("SINGULAR requested=0 stored=0 info=2 solve_skipped" in output,"Missing singular limit")
        require(scalar(output,"solved")==len(cases),"Case count mismatch")
    print(f"PASS E{number:02d}: independently checked printed results")


def csv_example(with_plot: bool) -> None:
    with tempfile.TemporaryDirectory(prefix="nmp02_toy_") as tmp:
        base=Path(tmp);path=base/"toy.csv"
        execute([binary(9),str(path)])
        with path.open(newline="") as stream:
            reader=csv.DictReader(stream)
            require(reader.fieldnames==["x","y"],"Incorrect E09 CSV schema")
            rows=list(reader)
        require(len(rows)==5,"Incorrect E09 row count")
        close_vector([float(r["x"]) for r in rows],[0,.5,1,1.5,2])
        close_vector([float(r["y"]) for r in rows],[1,1.25,1,.25,-1])
        original=path.read_bytes()
        execute([binary(9),str(path)],False)
        require(path.read_bytes()==original,"Overwrite protection changed data")
        execute([binary(9)],False)
        execute([binary(9),str(base/"absent"/"toy.csv")],False)
        print("PASS E09: actual CSV, numerical values, no-overwrite and path/usage errors")
        if with_plot:
            plotter=str(ROOT/"plotting/plot_toy_polynomial.py")
            dest=base/"toy.png"
            execute([sys.executable,plotter,str(path),"--output",str(dest)])
            require(dest.read_bytes().startswith(b"\x89PNG\r\n\x1a\n") and dest.stat().st_size>1000,"Invalid PNG")
            before=dest.read_bytes()
            execute([sys.executable,plotter,str(path),"--output",str(dest)],False)
            require(dest.read_bytes()==before,"Plot overwrite protection failed")
            execute([sys.executable,plotter,str(path),"--output",str(dest),"--overwrite"])
            invalid=["wrong,header\n0,1\n", "x,y\n0,nan\n.5,1\n1,1\n1.5,1\n2,1\n",
                     "x,y\n0,1\n", "x,y\n0,1\n.5,1\n.5,1\n1.5,1\n2,1\n",
                     "x,y\n0,1,extra\n.5,1\n1,1\n1.5,1\n2,1\n"]
            for i,text in enumerate(invalid):
                bad=base/f"bad{i}.csv";bad.write_text(text)
                bad_out=base/f"bad{i}.png"
                execute([sys.executable,plotter,str(bad),"--output",str(bad_out)],False)
                require(not bad_out.exists(),"Invalid CSV produced a plot")
            execute([sys.executable,plotter,str(base/"missing.csv"),"--output",str(base/"missing.png")],False)
            execute([sys.executable,plotter,str(path),"--output",str(base/"absent"/"toy.png")],False)
            print("PASS plotter: real E09 input, headless PNG and eight protected/error cases")


def main() -> int:
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--group",choices=GROUPS,default="standard")
    parser.add_argument("--plot",action="store_true",help="Test Matplotlib and invalid inputs; requires E09 binary")
    parser.add_argument("--log",type=Path,help="Write actual commands/stdout/stderr/exit codes as JSON")
    args=parser.parse_args();status=0
    try:
        for n in GROUPS[args.group]:
            if n!=9:small_example(n)
        if 9 in GROUPS[args.group] or args.plot:csv_example(args.plot)
        print(f"PASS requested group: {args.group}")
    except (OSError,ValueError,RuntimeError,subprocess.TimeoutExpired) as exc:
        print(f"FAIL: {exc}",file=sys.stderr);status=1
    finally:
        if args.log:
            args.log.parent.mkdir(parents=True,exist_ok=True)
            args.log.write_text(json.dumps({"status":status,"group":args.group,"runs":EVIDENCE},indent=2))
    return status


if __name__=="__main__":
    raise SystemExit(main())
