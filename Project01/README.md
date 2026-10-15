### HOW TO RUN MY CODE
My code is written to compiled on Debian 13 (Trixie), attached is also a Dockerfile with bare build necessities to properly build on Debian 13.

I personally run my code environment with
```
podman run -it --userns="keep-id" -v .:/proj build
cd /src/build
cmake ..
make -j32
```

