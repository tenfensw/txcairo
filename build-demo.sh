#!/bin/sh -e
rm -rfv *.dSYM *.o demo
test "$1" != "clean" && c++ -o demo -g -Wall -Wextra -std=c++0x demo.cxx -I. `txlin-macflags --cflags` `txlin-macflags --libs` `pkg-config --cflags cairo` `pkg-config --libs cairo`
exit $?
