# Conditional building of X11 related things
%bcond_with X11

Name:       pango

Summary:    System for layout and rendering of internationalized text
Version:    1.40.1
Release:    1
Group:      System/GUI/GNOME
License:    LGPLv2+
URL:        http://www.pango.org
Source0:    http://download.gnome.org/sources/pango/1.30/pango-%{version}.tar.xz
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
%if %{with X11}
BuildRequires:  pkgconfig(xft) >= 2.0.0
BuildRequires:  pkgconfig(xrender)
%endif
BuildRequires:  pkgconfig(glib-2.0) >= 2.31.0
BuildRequires:  pkgconfig(gobject-2.0)
BuildRequires:  pkgconfig(gmodule-no-export-2.0)
BuildRequires:  pkgconfig(cairo) >= 1.7.6
BuildRequires:  pkgconfig(freetype2)
BuildRequires:  pkgconfig(fontconfig) >= 2.5.0
BuildRequires:  pkgconfig(harfbuzz)

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



%prep
%setup -q -n %{name}-%{version}/libpango

%build

echo "EXTRA_DIST = missing-gtk-doc" > gtk-doc.make

%autogen --disable-static \
    --disable-gtk-doc \
    --disable-doc-cross-references \
    --with-included-modules=basic-fc \
    --disable-introspection

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

%post
/sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%doc AUTHORS COPYING
%{_libdir}/libpango*-*.so.*

%files devel
%defattr(-,root,root,-)
%doc %{_mandir}/man1/*
%{_datadir}/man/man1/pango-view.1.gz
%doc pango-view/HELLO.txt
%{_bindir}/pango-view
%{_libdir}/libpango*.so
%{_includedir}/*
%{_libdir}/pkgconfig/*
#%doc %{_datadir}/gtk-doc/html/pango
