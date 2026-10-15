### HOW TO RUN MY CODE
My code is written to be compiled on Ubuntu:26.04, nut other apt based distributions shouldn't have no issue. Dockerfile building should be only considered if drastic issues should occur.
```
cd build
cmake ..
make
```
If linux distibution is an issue, run build:
```
podman build . -t "build-proj:v1"
podman run -it --userns="keep-id" -v .:/proj:z localhost/build-proj:v1
cd /proj/build
cmake ..
make
```

# General Project structure
```
Project###
.
├── build           - build directory
├── data            - data directory
├── include         - include directory
├── info            - files related to tasks and presentations
├── plot            - plots & scripts for plots
├── report          - report latex file and pdf file
├── src             - source code
├── CMakeLists.txt
├── Dockerfile
└── README.md
```
