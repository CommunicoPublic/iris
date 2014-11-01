#!/bin/sh

if [ -f Makefile ]; then
    make clean
fi

rm -fr CMakeCache.txt CMakeFiles DartTestfile.txt Makefile Testing cmake_install.cmake progress.make install_manifest.txt CTestTestfile.cmake doc/html
rm -fr core *.out *.so *.info *.core iris-config include/IrisConfig.h test.txt
rm -fr debian/*.log debian/files debian/docs debian/*.debhelper debian/*.symbols debian/*.substvars
rm -fr configure-stamp build-stamp
rm -fr debian/iris-server0.1-dbg debian/iris-cli0.1 debian/iris-server0.1 debian/iris-configs0.1 debian/libiris-core0.1 debian/iris-dev debian/tmp debian/iris-file-logger0.1 debian/iris-echo-handler0.1 debian/*.init debian/*.logrotate
rm -fr src/util/ConfigFileParser.cpp
rm -fr include/util/JSONParser.tcc
rm -fr iris.conf.default
