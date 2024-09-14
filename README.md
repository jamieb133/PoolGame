### Snookered!

A game of pool with some janky physics! :D 

To build it you need cmake and some sort of build tool e.g. Make, Ninja, Visual Studio etc. There is a dependency on Raylib but CMake will just pull that in for you. Note that raylib and glfw must both be built as shared libraries using the cmake flags below for the hot-reloading functionality to work, otherwise it will crash and segfault horribly.

```
mkdir build && cd build
cmake -DSHARED=ON -DBUILD_SHARED_LIBS=ON ..
make -j8 # or whatever build system you generated with...
```