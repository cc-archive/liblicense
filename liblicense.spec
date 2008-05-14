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
Buildrequires: flac-devel
BuildRequires: ruby-devel
BuildRequires: gettext
Url: http://www.creativecommons.org/project/Liblicense

BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

%description
The liblicense package contains the library, bindings, CLI utilities and license files.

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
/usr/bin
/usr/share
/usr/lib
/usr/include

%changelog
* Tue May 13 2008 Asheesh Laroia <asheesh@creativecommons.org> - 0.6.2-1
- liblicense 0.6.2
- Create separate package for plugins
* Wed Aug 22 2007 Scott Shawcroft <scott.shawcroft@gmail.com> - 0.4-1
- liblicense 0.4
* Mon Jul 30 2007 Scott Shawcroft <scott.shawcroft@gmail.com> - 0.3-1
- initial liblicense rpm
