Name:       pango
Summary:    System for layout and rendering of internationalized text
Version:    1.48.10
Release:    1
License:    LGPLv2+
URL:        http://www.pango.org
Source0:    %{name}-%{version}.tar.xz
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
Requires:   %{name} = %{version}-%{release}

%description devel
The pango-devel package includes the header files and developer documentation
for the pango package.


%prep
%autosetup -p1 -n %{name}-%{version}/upstream

%build
%meson -Dintrospection=disabled -Dgtk_doc=false -Dlibthai=disabled -Dxft=disabled
%meson_build

%install
%meson_install

%post
/sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%license COPYING
%{_libdir}/libpango*-*.so.*

%files devel
%defattr(-,root,root,-)
%{_bindir}/pango-view
%{_bindir}/pango-list
%{_bindir}/pango-segmentation
%{_libdir}/libpango*.so
%{_includedir}/*
%{_libdir}/pkgconfig/*

