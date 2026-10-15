### HOW TO RUN MY CODE
My code is written to be compiled on Ubuntu:26.04

I personally run my code environment with
```
podman build . -t "build-proj:v1"
podman run -it --userns="keep-id" -v .:/proj:z localhost/build-proj:v1
cd /proj/build
cmake ..
make -j32
```

# General Project structure
```
Project###
├── build           - build directory
├── data            - output data
├── include         - header files
├── info            - lab presentations
├── src             - source code (*.c,*.cpp)
├── CMakeLists.txt
├── Dockerfile
└── README.md
```
