Name: iris
Version: 0.1.23
Release: 1%{?dist}
Epoch: 1

Summary: Iris multi-purpose TCP server
Group: Applications/Internet
URL: https://www.communico.pro/iris

License: GPLv3 with exceptions and LGPLv2 and BSD

Source0: http://download.communico.pro/pub/iris/iris-%{version}.tar.gz

Patch1: 01-centos-iris.conf.patch

BuildRequires: libuuid-devel, libev-devel, openssl-devel
BuildRequires: cmake, ragel
BuildRequires: systemd

Requires: %{name}-libs%{?_isa} = %{epoch}:%{version}-%{release}
Requires: %{name}-logger%{?_isa} = %{epoch}:%{version}-%{release}
Requires: %{name}-handler%{?_isa} = %{epoch}:%{version}-%{release}
Requires: fileutils, bash
Requires(post): %{_sbindir}/update-alternatives
Requires(postun): %{_sbindir}/update-alternatives

%{?systemd_requires: %systemd_requires}

Provides: iris = %{epoch}:%{version}-%{release}
Provides: iris%{?_isa} = %{epoch}:%{version}-%{release}
Obsoletes: iris < %{obsoleted_iris_evr}

# By default, patch(1) creates backup files when chunks apply with offsets.
# Turn that off to ensure such files don't get included in RPMs (cf bz#884755).
%global _default_patch_flags --no-backup-if-mismatch

%description
Iris multi-purpose TCP server
Iris is all-in-one solution for mass mail hosting written in C++.
The main goals of Iris are high speed, linear scalability up to
100 millions user accounts and ease of use.

%package libs

Summary: Iris common libraries
Group: Applications/Internet
Requires: /sbin/ldconfig
#Provides: iris-libs = %{epoch}:%{version}-%{release}
#Provides: iris-libs%{?_isa} = %{epoch}:%{version}-%{release}
Obsoletes: iris-libs < %{obsoleted_iris_evr}

%description libs
Iris common libraries
Iris is all-in-one solution for mass mail hosting written in C++.
The main goals of Iris are high speed, linear scalability up to
100 millions user accounts and ease of use.

%package devel

Summary: Iris header files and documentation
Group: Applications/Internet
Requires: %{name}-libs%{?_isa} = %{epoch}:%{version}-%{release}
Requires: openssl-devel%{?_isa} libuuid-devel%{?_isa} libev-devel%{?_isa}
Provides: iris-devel = %{epoch}:%{version}-%{release}
Provides: iris-devel%{?_isa} = %{epoch}:%{version}-%{release}
Obsoletes: iris-devel < %{obsoleted_iris_evr}

%description devel
Iris header files and documentation
Iris is all-in-one solution for mass mail hosting written in C++.
The main goals of Iris are high speed, linear scalability up to
100 millions user accounts and ease of use.

%package echo-handler
Summary: Example protocol module for Iris server
Group: Applications/Internet
Requires: %{name}-libs%{?_isa} = %{epoch}:%{version}-%{release}
Provides: iris-handler = %{epoch}:%{version}-%{release}
Provides: iris-handler%{?_isa} = %{epoch}:%{version}-%{release}
Obsoletes: iris-handler < %{obsoleted_iris_evr}

%description echo-handler
Example protocol module for Iris server
Iris is all-in-one solution for mass mail hosting written in C++.
The main goals of Iris are high speed, linear scalability up to
100 millions user accounts and ease of use.

%package file-logger
Summary: File logger for Iris server
Group: Applications/Internet
Requires: %{name}-libs%{?_isa} = %{epoch}:%{version}-%{release}
Provides: iris-logger = %{epoch}:%{version}-%{release}
Provides: iris-logger%{?_isa} = %{epoch}:%{version}-%{release}
Obsoletes: iris-logger < %{obsoleted_iris_evr}

%description file-logger
File logger for Iris server
Iris is all-in-one solution for mass mail hosting written in C++.
The main goals of Iris are high speed, linear scalability up to
100 millions user accounts and ease of use.

%package syslog-logger
Summary: Syslog logger for Iris server
Group: Applications/Internet
Requires: %{name}-libs%{?_isa} = %{epoch}:%{version}-%{release}
Provides: iris-logger = %{epoch}:%{version}-%{release}
Provides: iris-logger%{?_isa} = %{epoch}:%{version}-%{release}
Obsoletes: iris-logger < %{obsoleted_iris_evr}

%description syslog-logger
Syslog logger for Iris server
Iris is all-in-one solution for mass mail hosting written in C++.
The main goals of Iris are high speed, linear scalability up to
100 millions user accounts and ease of use.


%prep
%setup -q -n iris-%{version}

%patch1 -p1

%build

CFLAGS="%{optflags} -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE"
CXXFLAGS="$CFLAGS"
export CFLAGS CXXFLAGS

cmake . -DCMAKE_INSTALL_PREFIX="%{_prefix}" \
        -DINSTALL_LIBDIR:STRING="%{_libdir}"
make %{?_smp_mflags}

%check

%install
make DESTDIR=$RPM_BUILD_ROOT install

chmod 0755 $RPM_BUILD_ROOT%{_libdir}/lib*.so.*
find $RPM_BUILD_ROOT -print | sed "s|^$RPM_BUILD_ROOT||" | sort > ROOTFILES

install -m 0755 -d $RPM_BUILD_ROOT%{_localstatedir}/run/iris
install -m 0755 -d $RPM_BUILD_ROOT%{_localstatedir}/lib/iris
install -m 0755 -d $RPM_BUILD_ROOT%{_localstatedir}/log/iris
install -m 0755 -d $RPM_BUILD_ROOT%{_localstatedir}/cache/iris

install -m 0755 -d $RPM_BUILD_ROOT%{_sysconfdir}
install -m 0755 -d $RPM_BUILD_ROOT%{_sysconfdir}/iris
install -m 0755 -d $RPM_BUILD_ROOT%{_sysconfdir}/iris/conf.d
install -m 0755 -d $RPM_BUILD_ROOT%{_sysconfdir}/iris/handlers.d
install -m 0755 -d $RPM_BUILD_ROOT%{_sysconfdir}/iris/loggers.d
install -m 0755 -d $RPM_BUILD_ROOT%{_sysconfdir}/iris/services.d
install -m 0755 -d $RPM_BUILD_ROOT%{_sysconfdir}/iris/ssl
install -m 0755 -d $RPM_BUILD_ROOT%{_sysconfdir}/logrotate.d

install -p -m 755 -d ${RPM_BUILD_ROOT}%{_unitdir}
install -p -m 644 scripts/iris.service ${RPM_BUILD_ROOT}%{_unitdir}/%{name}.service

#install -p -m 755 -d $RPM_BUILD_ROOT%{_sysconfdir}/ld.so.conf.d
#echo "%{_libdir}/iris" > $RPM_BUILD_ROOT%{_sysconfdir}/ld.so.conf.d/%{name}-%{_arch}.conf

install -p -m 755 -d $RPM_BUILD_ROOT%{_sysconfdir}/logrotate.d
install -p -m 644 scripts/iris.logrotate ${RPM_BUILD_ROOT}%{_sysconfdir}/logrotate.d/%{name}

install -p -m 644 ${RPM_BUILD_ROOT}/usr/share/iris/conf/iris.conf \
    ${RPM_BUILD_ROOT}%{_sysconfdir}/iris/iris.conf

install -p -m 644 ${RPM_BUILD_ROOT}/usr/share/iris/conf/handlers.d/echo-handler.conf \
    ${RPM_BUILD_ROOT}%{_sysconfdir}/iris/handlers.d/echo-handler.conf
install -p -m 644 ${RPM_BUILD_ROOT}/usr/share/iris/conf/services.d/echo-service.conf \
    ${RPM_BUILD_ROOT}%{_sysconfdir}/iris/services.d/echo-service.conf
install -p -m 644 ${RPM_BUILD_ROOT}/usr/share/iris/conf/services.d/echo-ssl-service.conf \
    ${RPM_BUILD_ROOT}%{_sysconfdir}/iris/services.d/echo-ssl-service.conf
install -p -m 644 ${RPM_BUILD_ROOT}/usr/share/iris/conf/ssl/dh1024.pem \
    ${RPM_BUILD_ROOT}%{_sysconfdir}/iris/ssl/dh1024.pem
install -p -m 644 ${RPM_BUILD_ROOT}/usr/share/iris/conf/ssl/ssl-cert-snakeoil.key \
    ${RPM_BUILD_ROOT}%{_sysconfdir}/iris/ssl/ssl-cert-snakeoil.key
install -p -m 644 ${RPM_BUILD_ROOT}/usr/share/iris/conf/ssl/ssl-cert-snakeoil.pem \
    ${RPM_BUILD_ROOT}%{_sysconfdir}/iris/ssl/ssl-cert-snakeoil.pem

install -p -m 644 ${RPM_BUILD_ROOT}/usr/share/iris/conf/loggers.d/general-logger.conf \
    ${RPM_BUILD_ROOT}%{_sysconfdir}/iris/loggers.d/general-logger.conf

install -p -m 644 ${RPM_BUILD_ROOT}/usr/share/iris/conf/loggers.d/syslog-logger.conf \
    ${RPM_BUILD_ROOT}%{_sysconfdir}/iris/loggers.d/syslog-logger.conf

rm -f ${RPM_BUILD_ROOT}/usr/bin/iris-config-parser
rm -f ${RPM_BUILD_ROOT}/usr/share/iris/conf/handlers.d/echo-handler.conf
rm -f ${RPM_BUILD_ROOT}/usr/share/iris/conf/iris.conf
rm -f ${RPM_BUILD_ROOT}/usr/share/iris/conf/loggers.d/general-logger.conf
rm -f ${RPM_BUILD_ROOT}/usr/share/iris/conf/loggers.d/syslog-logger.conf
rm -f ${RPM_BUILD_ROOT}/usr/share/iris/conf/services.d/echo-service.conf
rm -f ${RPM_BUILD_ROOT}/usr/share/iris/conf/services.d/echo-ssl-service.conf
rm -f ${RPM_BUILD_ROOT}/usr/share/iris/conf/ssl/dh1024.pem
rm -f ${RPM_BUILD_ROOT}/usr/share/iris/conf/ssl/ssl-cert-snakeoil.key
rm -f ${RPM_BUILD_ROOT}/usr/share/iris/conf/ssl/ssl-cert-snakeoil.pem
rm -f ${RPM_BUILD_ROOT}/usr/share/man/man1/iris-config-parser.1.gz
rm -f ${RPM_BUILD_ROOT}/usr/share/man/man1/iris-config-test.1.gz
rm -f ${RPM_BUILD_ROOT}/usr/share/man/man8/iris-server-ctl.8.gz
rm -f ${RPM_BUILD_ROOT}/usr/share/man/man1/iris-config-parser.1*
rm -f ${RPM_BUILD_ROOT}/usr/share/man/man1/iris-config-test.1*
rm -f ${RPM_BUILD_ROOT}/usr/share/man/man8/iris-server-ctl.8*

%pre
/usr/sbin/groupadd -g 116 -o -r iris >/dev/null 2>&1 || :
/usr/sbin/useradd -M -N -g iri6 -o -r -d %{_localstatedir}/lib/iris -s /sbin/nologin \
	-c "Iris Server" -u 116 iris >/dev/null 2>&1 || :

%post devel
#%{_sbindir}/update-alternatives --install %{_bindir}/iris_config \
#	iris_config %{_libdir}/iris/iris_config %{__isa_bits}

%post libs -p /sbin/ldconfig

%post
%systemd_post iris-server.service
/bin/chmod 0755 %{_localstatedir}/lib/iris
/bin/touch %{_localstatedir}/log/iris/error.log
/bin/touch %{_localstatedir}/log/iris/access.log

%preun
%systemd_preun iris.service

%postun libs -p /sbin/ldconfig

%postun
%systemd_postun_with_restart iris.service

%files
%{_sbindir}/iris-server
%{_mandir}/man8/iris-server.8*
%{_unitdir}/iris.service
#%{_tmpfilesdir}/%{name}.conf
%dir %{_sysconfdir}/iris
%dir %{_sysconfdir}/iris/conf.d
%dir %{_sysconfdir}/iris/handlers.d
%dir %{_sysconfdir}/iris/services.d
%dir %{_sysconfdir}/iris/loggers.d
%{_sysconfdir}/iris/iris.conf
%attr(0755,iris,iris) %dir %{_localstatedir}/run/iris
%attr(0755,iris,iris) %dir %{_localstatedir}/lib/iris
%attr(0750,iris,iris) %dir %{_localstatedir}/log/iris
%config(noreplace) %{_sysconfdir}/logrotate.d/iris

%files libs
%dir %{_libdir}/iris
%{_libdir}/libiris-conf.so.*
%{_libdir}/libiris-core.so.*
%{_libdir}/libiris-ev.so.*
%{_libdir}/libiris-mt.so.*
%{_libdir}/libiris-net.so.*
%{_libdir}/libiris-net-ssl.so.*
%{_libdir}/libiris-util.so.*

%files echo-handler
%{_libdir}/iris/mod_iris_echo.so
%{_sysconfdir}/iris/handlers.d/echo-handler.conf
%{_sysconfdir}/iris/services.d/echo-service.conf
%{_sysconfdir}/iris/services.d/echo-ssl-service.conf
%{_sysconfdir}/iris/ssl/dh1024.pem
%{_sysconfdir}/iris/ssl/ssl-cert-snakeoil.key
%{_sysconfdir}/iris/ssl/ssl-cert-snakeoil.pem

%files file-logger
%{_libdir}/iris/mod_file_logger.so
%{_sysconfdir}/iris/loggers.d/general-logger.conf

%files syslog-logger
%{_libdir}/iris/mod_syslog_logger.so
%{_sysconfdir}/iris/loggers.d/syslog-logger.conf

%files devel
#%ghost %{_bindir}/iris-config
%{_bindir}/iris-config
%{_mandir}/man8/iris-config.8*
%{_includedir}/iris/*
%{_libdir}/libiris-conf.so
%{_libdir}/libiris-core.so
%{_libdir}/libiris-ev.so
%{_libdir}/libiris-mt.so
%{_libdir}/libiris-net.so
%{_libdir}/libiris-net-ssl.so
%{_libdir}/libiris-util.so

%changelog
* Fri Jun 26 2015 Andrey V. Shetukhin <stellar@communico.pro> 0.0.23
- Bug fixes & deep code refactoring

* Tue May 26 2015 Andrey V. Shetukhin <stellar@communico.pro> 0.0.22
- Execute directive in config files

* Wed Nov 19 2014 Andrey V. Shetukhin <stellar@communico.pro> 0.0.18
- Package for RHEL

