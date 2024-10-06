### Snookered!

A game of pool with some janky physics! :D 

To build it you need cmake and some sort of build tool e.g. Make, Ninja, Visual Studio etc. There are dependencies on Raylib and Chipmunk (physics engine) but CMake will just pull those in for you. Note that raylib and glfw must both be built as shared libraries using the cmake flags below for the hot-reloading functionality to work, otherwise it will crash and segfault horribly.

```
mkdir build && cd build
cmake -DSHARED=ON -DBUILD_SHARED_LIBS=ON -DBUILD_SHARED=ON ..
ninja -j8 # or whatever build system you generated with...
```

Alternatively use the presets file, only Ninja is currently supported so that will need to be installed too.
```
cmake --preset debug
cmake --build --preset debug -- -j8
```