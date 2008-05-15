Name: liblicense
Version: 0.6.2
Release: 1
License: LGPL
Summary: Content License Library
Group: Development/Libraries
Source: %{name}-%{version}.tar.gz
BuildRequires: exempi-devel >= 1.99.9
BuildRequires: raptor-devel >= 1.4.14
BuildRequires: gnome-common
BuildRequires: sqlite-devel
BuildRequires: taglib-devel
BuildRequires: libgsf-devel
BuildRequires: id3lib-devel
BuildRequires: libvorbis-devel
Buildrequires: flac-devel
BuildRequires: ruby-devel
BuildRequires: gettext
Url: http://www.creativecommons.org/project/Liblicense

BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

%description
The liblicense package contains the library, bindings, CLI utilities and license files.

%package modules
Summary: Input/output modules for accessing license metadata in various file foramts.
Group: Yourmom

%description modules
Input/output modules for accessing license metadata in various file foramts.

%package cli
Summary: Simple command-line utility for examining the license in a file or setting a user preference for a default license
Group: Yourmom

%description cli
Simple command-line utility for examining the license in a file or setting a user preference for a default license

%package devel
Summary: Required headers and libraries for building software that uses liblicense
Group: Yourmom

%description devel
Required headers and libraries for building software that uses liblicense

%package debug
Summary: Debug files for attaching to liblicense with gdb
Group: Yourmom

%description debug
Debug files for attaching to liblicense with gdb

%prep
%setup -n liblicense-0.6.2
./configure --prefix=/usr

%build
make RPM_OPT_FLAGS="$RPM_OPT_FLAGS"

%install
rm -rf $RPM_BUILD_ROOT
make DESTDIR=$RPM_BUILD_ROOT install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc COPYING
/usr/lib/liblicense.so*
/usr/lib/liblicense/*/config/flat_file.so
/usr/share/liblicense/icons
/usr/share/liblicense/licenses
/usr/share/locale/*/LC_MESSAGES/liblicense.mo
/usr/share/pixmaps/scales.svg

%files modules
/usr/lib/liblicense/*/io/*.so

%files cli
/usr/bin

%files devel
/usr/include/liblicense-2.0
/usr/lib/liblicense*.*a
/usr/lib/pkgconfig/liblicense.pc

%files debug
/usr/lib/debug

%changelog
* Tue May 13 2008 Asheesh Laroia <asheesh@creativecommons.org> - 0.6.2-1
- liblicense 0.6.2
- Create separate package for plugins
* Wed Aug 22 2007 Scott Shawcroft <scott.shawcroft@gmail.com> - 0.4-1
- liblicense 0.4
* Mon Jul 30 2007 Scott Shawcroft <scott.shawcroft@gmail.com> - 0.3-1
- initial liblicense rpm
