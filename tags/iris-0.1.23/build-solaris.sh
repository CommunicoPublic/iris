#!/bin/sh
PWD=`pwd`
VERSION="0.1.23"
ARCH=SPARC

rm -rf build
/bin/sh ClearPackage.sh
cmake -DSKIP_RELINK_RPATH=ON . -DCMAKE_INSTALL_PREFIX=/opt/csw -DCFLAGS="${CFLAGS}" -DCXXFLAGS="${CXXFLAGS}"
make
mkdir build
make DESTDIR=build install
# Libiris
cd $PWD/Solaris/CMNClibiris-core01
pkgmk -o -r $PWD/build/opt/csw -d $PWD/build -f Prototype
# Iris
cd $PWD/Solaris/CMNCiris-server01
pkgmk -o -r $PWD/build/opt/csw -d $PWD/build -f Prototype
# Iris configuration
cd $PWD/Solaris/CMNCiris-conf01
pkgmk -o -r $PWD/build/opt/csw -d $PWD/build -f Prototype
# Iris CLI
cd $PWD/Solaris/CMNCiris-cli01
pkgmk -o -r $PWD/build/opt/csw -d $PWD/build -f Prototype
# Iris Dev
cd $PWD/Solaris/CMNCiris-dev
pkgmk -o -r $PWD/build/opt/csw -d $PWD/build -f Prototype
# Iris file logger
cd $PWD/Solaris/CMNCiris-file-logger01
pkgmk -o -r $PWD/build/opt/csw -d $PWD/build -f Prototype
# Iris syslog
cd $PWD/Solaris/CMNCiris-syslog-logger01
pkgmk -o -r $PWD/build/opt/csw -d $PWD/build -f Prototype
# Iris Echo handler
cd $PWD/Solaris/CMNCiris-echo-handler01
pkgmk -o -r $PWD/build/opt/csw -d $PWD/build -f Prototype

cd $PWD/build
tar -cf - CMNClibiris-core01 | gzip -9 -c > CMNClibiris-core01.$VERSION.$ARCH.pkg.tar.gz
tar -cf - CMNCiris-server01 | gzip -9 -c > CMNCiris-server01.$VERSION.$ARCH.pkg.tar.gz
tar -cf - CMNCiris-conf01 | gzip -9 -c > CMNCiris-conf01.$VERSION.$ARCH.pkg.tar.gz
tar -cf - CMNCiris-cli01 | gzip -9 -c > CMNCiris-cli01.$VERSION.$ARCH.pkg.tar.gz
tar -cf - CMNCiris-dev | gzip -9 -c > CMNCiris-dev.$VERSION.$ARCH.pkg.tar.gz
tar -cf - CMNCiris-file-logger01 | gzip -9 -c > CMNCiris-file-logger01.$VERSION.$ARCH.pkg.tar.gz
tar -cf - CMNCiris-syslog-logger01 | gzip -9 -c > CMNCiris-syslog-logger01.$VERSION.$ARCH.pkg.tar.gz
tar -cf - CMNCiris-echo-handler01 | gzip -9 -c > CMNCiris-echo-handler01.$VERSION.$ARCH.pkg.tar.gz
rm -fr opt
cd $PWD
