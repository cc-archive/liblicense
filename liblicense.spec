Name: liblicense
Version: 0.6.2
Release: 1
License: LGPL
Summary: Content License Library
Group: Development/Libraries
Source0: http://downloads.sourceforge.net/%{name}/%{name}-%{version}.tar.gz
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
Group: Development/Libraries

%description modules
Input/output modules for accessing license metadata in various file foramts.

%package cli
Summary: Simple command-line utility for examining the license in a file or setting a user preference for a default license
Group: Applications/File

%description cli
Simple command-line utility for examining the license in a file or setting a user preference for a default license

%package devel
Summary: Development files for ${name}
Group: Development/Libraries
Requires: %{name} = %{version}-%{release}

%description devel
The %{name}-devel package contains libraries and header files for
developing applications that use %{name}.

%package debug
Summary: Debug files for attaching to liblicense with gdb
Group: Development/Libraries

%description debug
Debug files for attaching to liblicense with gdb

%prep
%setup -q

%build
%configure --disable-static
make RPM_OPT_FLAGS="$RPM_OPT_FLAGS" %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT
find $RPM_BUILD_ROOT -name '*.la' -exec rm -f {} ';'

%clean
rm -rf $RPM_BUILD_ROOT

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

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
