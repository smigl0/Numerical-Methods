### HOW TO RUN MY CODE
My code was written and compiled on Fedora:44. To properly build, run the following:
```
cd build
cmake ..
make
```
To build on non-fedora system, ensure `docker` and `docker-compose` are installed locally and run
`
docker compose up
`
# General Project structure
```
Project###
.
├── build           - build directory
├── data            - data directory
├── include         - include directory
├── info            - files related to tasks and presentations
├── plot            - plots scripts
├── img             - saved plot images from plot scripts
├── report          - report latex file and pdf file
├── src             - source code
├── CMakeLists.txt
├── Dockerfile
├── docker-compose.yml
└── README.md
```
