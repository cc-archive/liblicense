Name: liblicense
Version: 0.8.1
Release: 1
License: LGPLv2
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
The liblicense package contains the library, bindings, CLI utilities and
license files.

%package modules
Summary: Input/output modules for accessing license metadata in various file formats
Group: Development/Libraries

%description modules
Input/output modules for accessing license metadata in various file foramts.

%package python
Summary: Python bindings for liblicense, a library to handle license metadata
Group: Development/Libraries

%description python
Python bindings for liblicense, a library to handle media license metadata.

%package cli
Summary: CLI tool for choosing a user default license or the license of a file
Group: Applications/File

%description cli
Simple command-line utility for examining the license in a file or setting a
user preference for a default license

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
%configure --disable-static --disable-rpath
sed -i 's|^hardcode_libdir_flag_spec=.*|hardcode_libdir_flag_spec=""|g' libtool
sed -i 's|^runpath_var=LD_RUN_PATH|runpath_var=DIE_RPATH_DIE|g' libtool
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
%{_libdir}/liblicense.so.*
%dir %{_libdir}/liblicense
%dir %{_libdir}/liblicense/%{version}
%{_libdir}/liblicense/%{version}/config/
%dir %{_datadir}/liblicense
%{_datadir}/liblicense/icons
%{_datadir}/liblicense/licenses
%{_datadir}/pixmaps/scales.svg

%files modules
%defattr(-,root,root)
%{_libdir}/liblicense/%{version}/io/

%files cli
%defattr(-,root,root)
%{_bindir}/license

%files devel
%defattr(-,root,root)
%{_includedir}/liblicense-3.0
%{_libdir}/pkgconfig/liblicense.pc
%{_libdir}/liblicense.so

%files python
%defattr(-,root,root)
%dir %{python_sitelib}/liblicense
%{python_sitearch}/liblicense/liblicense.so
%{python_sitelib}/liblicense/__init__.py
%{python_sitelib}/liblicense/__init__.pyc
%{python_sitelib}/liblicense/__init__.pyo

%changelog
* Tue Dec 23 2008 Asheesh Laroia <asheesh@creativecommons.org> - 0.8.1-1
- liblicense 0.8.1 upstream

* Sat Nov 29 2008 Ignacio Vazquez-Abrams <ivazqueznet+rpm@gmail.com> - 0.8-3
- Rebuild for Python 2.6

* Fri Aug 29 2008 Michael Schwendt <mschwendt@fedoraproject.org> - 0.8-2
- Include unowned directories
- Add missing defattr to sub-packages

* Mon Jul 28 2008 Asheesh Laroia <asheesh@creativecommons.org> - 0.8-1
- liblicense 0.8 upstream

* Mon Jun 16 2008 Asheesh Laroia <asheesh@creativecommons.org> - 0.7.0-3
- Pass --disable-rpath to ./configure.
- Modify bundled libtool with sed expressions from Fedora Packaging Guidelines

* Thu May 29 2008 Asheesh Laroia <asheesh@creativecommons.org> - 0.7.0-2
- Put liblicense.so in -devel package
- Fix really long (>80 chars) descriptions
- Stop trying to own /usr/bin
- Remove trailing dot from package summary; fixed typo (misspelled "formats")
- LGPLv2 is license

* Tue May 13 2008 Asheesh Laroia <asheesh@creativecommons.org> - 0.7.0-1
- liblicense 0.7.0
- Create separate packages for plugins, Python modules

* Wed Aug 22 2007 Scott Shawcroft <scott.shawcroft@gmail.com> - 0.4-1
- liblicense 0.4

* Mon Jul 30 2007 Scott Shawcroft <scott.shawcroft@gmail.com> - 0.3-1
- initial liblicense rpm
