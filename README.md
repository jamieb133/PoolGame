### Snookered!

A game of pool with some janky physics! :D 

To build it you need cmake and some sort of build tool e.g. Make, Ninja, Visual Studio etc. There is a dependency on Raylib but CMake will just pull that in for you.

```
mkdir build && cd build
cmake ..
make -j8 # or whatever build system you generated with...
```