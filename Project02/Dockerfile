FROM docker.io/library/fedora:latest
RUN dnf upgrade -y && \
    dnf install -y \
        cmake \
        gcc-c++ \
        make \
        blas-devel \
        openblas \
        openblas-devel \
        lapack \
        lapack-devel && \
    dnf clean all
