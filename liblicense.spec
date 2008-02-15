Name: liblicense
Version: 0.6
Release: 1
License: LGPL
Summary: Content License Library
Group: Development/Libraries
Source: %{name}-%{version}.tar.bz2
Patch0: %{name}-%{version}-gmsgfmt.patch
BuildRequires: exempi >= 1.99.3
BuildRequires: raptor-devel >= 1.4.14
BuildRequires: gnome-common
BuildRequires: sqlite-devel
BuildRequires: taglib-devel
BuildRequires: libgsf-devel
BuildRequires: id3lib-devel
Buildrequires: flac-devel
BuildRequires: ruby-devel
BuildRequires: gettext
Requires: ruby
Requires: python
Requires: exempi
Requires: raptor
Requires: sqlite
Requires: taglib
Requires: libgsf
Requires: id3lib
Requires: flac
Url: http://www.creativecommons.org/project/Liblicense

BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

%description
The liblicense package contains the library, bindings, CLI utilities and license files.

%prep
%setup -n liblicense-0.6
%patch -p1
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
* Wed Aug 22 2007 Scott Shawcroft <scott.shawcroft@gmail.com> - 0.4-1
- liblicense 0.4
* Mon Jul 30 2007 Scott Shawcroft <scott.shawcroft@gmail.com> - 0.3-1
- initial liblicense rpm
