Name:       pango
Summary:    System for layout and rendering of internationalized text
Version:    1.42.3
Release:    1
Group:      System/GUI/GNOME
License:    LGPLv2+
URL:        http://www.pango.org
Source0:    http://download.gnome.org/sources/pango/1.42/pango-%{version}.tar.xz
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
BuildRequires:  pkgconfig(glib-2.0) >= 2.31.0
BuildRequires:  pkgconfig(gobject-2.0)
BuildRequires:  pkgconfig(gmodule-no-export-2.0)
BuildRequires:  pkgconfig(cairo) >= 1.7.6
BuildRequires:  pkgconfig(freetype2)
BuildRequires:  pkgconfig(fontconfig) >= 2.5.0
BuildRequires:  pkgconfig(harfbuzz) >= 1.4.2
BuildRequires:  pkgconfig(fribidi)
BuildRequires:  meson

%description
Pango is a library for laying out and rendering of text, with an emphasis
on internationalization. Pango can be used anywhere that text layout is needed,
though most of the work on Pango so far has been done in the context of the
GTK+ widget toolkit. Pango forms the core of text and font handling for GTK+.

Pango is designed to be modular; the core Pango layout engine can be used
with different font backends.

The integration of Pango with Cairo provides a complete solution with high
quality text handling and graphics rendering.


%package devel
Summary:    Development files for pango
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}

%description devel
The pango-devel package includes the header files and developer documentation
for the pango package.

%package tests
Summary: Tests for the %{name} package
Requires: %{name} = %{version}-%{release}

%description tests
The %{name}-tests package contains tests that can be used to verify
the functionality of the installed %{name} package.


%prep
%setup -q -n %{name}-%{version}/upstream

%build
%meson -Dgir=false -Denable_docs=false
%meson_build

%install
%meson_install

%post
/sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%doc COPYING
%{_libdir}/libpango*-*.so.*

%files devel
%doc AUTHORS
%defattr(-,root,root,-)
%{_bindir}/pango-view
%{_bindir}/pango-list
%{_libdir}/libpango*.so
%{_includedir}/*
%{_libdir}/pkgconfig/*
#%doc %{_datadir}/gtk-doc/html/pango

%files tests
%{_libexecdir}/installed-tests/%{name}
%{_datadir}/installed-tests

