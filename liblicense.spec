Name: liblicense
Version: 0.6.2
Release: 1
License: LGPL v2.1
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
BuildRequires: python-devel
BuildRequires: gettext
Url: http://www.creativecommons.org/project/Liblicense

BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

%{!?python_sitelib: %define python_sitelib %(%{__python} -c "from distutils.sysconfig import get_python_lib; print get_python_lib()")}
%{!?python_sitearch: %define python_sitearch %(%{__python} -c "from distutils.sysconfig import get_python_lib; print get_python_lib(1)")}

%description
The liblicense package contains the library, bindings, CLI utilities and license files.

%package modules
Summary: Input/output modules for accessing license metadata in various file foramts.
Group: Development/Libraries

%description modules
Input/output modules for accessing license metadata in various file foramts.

%package python
Summary: Python bindings for liblicense, a library to handle media license metadata
Group: Development/Libraries
Provides: python-liblicense

%description python
Python bindings for liblicense, a library to handle media license metadata.

%package cli
Summary: Simple command-line utility for examining the license in a file or setting a user preference for a default license
Group: Applications/File

%description cli
Simple command-line utility for examining the license in a file or setting a user preference for a default license

%package devel
Summary: Development files for %{name}
Group: Development/Libraries
Requires: %{name} = %{version}-%{release}

%description devel
The %{name}-devel package contains libraries and header files for
developing applications that use %{name}.

%prep
%setup -q

%build
%configure --disable-static
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT
find $RPM_BUILD_ROOT -name '*.la' -exec rm -f {} ';'
%find_lang %{name}

%clean
rm -rf $RPM_BUILD_ROOT

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files -f %{name}.lang
%defattr(-,root,root)
%doc COPYING
%{_libdir}/liblicense.so*
%{_libdir}/liblicense/%{version}
%{_libdir}/liblicense/%{version}/config/
%{_datadir}/liblicense/icons
%{_datadir}/liblicense/licenses
%{_datadir}/pixmaps/scales.svg

%files modules
%{_libdir}/liblicense/%{version}/io/

%files cli
%{_bindir}

%files devel
%{_includedir}/liblicense-2.0
%{_libdir}/pkgconfig/liblicense.pc

%files python
%dir %{python_sitelib}/liblicense
%{python_sitearch}/liblicense/liblicense.so
%{python_sitelib}/liblicense/__init__.py
%{python_sitelib}/liblicense/__init__.pyc
%{python_sitelib}/liblicense/__init__.pyo

%changelog
* Tue May 13 2008 Asheesh Laroia <asheesh@creativecommons.org> - 0.6.2-1
- liblicense 0.6.2
- Create separate package for plugins
* Wed Aug 22 2007 Scott Shawcroft <scott.shawcroft@gmail.com> - 0.4-1
- liblicense 0.4
* Mon Jul 30 2007 Scott Shawcroft <scott.shawcroft@gmail.com> - 0.3-1
- initial liblicense rpm
