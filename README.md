# ELFY

A simple ELF symbol name dumper

Steps to build the project:

```
$ git clone --recursive https://github.com/miguelsaldivar/ELFY.git
$ mkdir build
$ cd build
$ CXX=clang++ CC=clang cmake  -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
$ make
```

A rust compiler is assumed to be installed, and should be. The main premise
of this project is to see how an ffi method for `cpp_demangle` could look like.
