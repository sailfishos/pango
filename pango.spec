# 
# Do NOT Edit the Auto-generated Part!
# Generated by: spectacle version 0.23
# 
# >> macros
%define freetype_version 2.1.3-3
%define fontconfig_version 2.6
%define cairo_version 1.7.6
%define libthai_version 0.1.9
# << macros

Name:       pango
Summary:    System for layout and rendering of internationalized text
Version:    1.29.4
Release:    1
Group:      System/GUI/GNOME
License:    LGPLv2+
URL:        http://www.pango.org
Source0:    http://download.gnome.org/sources/pango/1.28/pango-%{version}.tar.xz
Source100:  pango.yaml
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(freetype2) >= %{freetype_version}
BuildRequires:  pkgconfig(fontconfig) >= %{fontconfig_version}
BuildRequires:  pkgconfig(xt)
BuildRequires:  pkgconfig(xrender)
BuildRequires:  pkgconfig(x11)
BuildRequires:  pkgconfig(xext)
BuildRequires:  pkgconfig(xft)
BuildRequires:  pkgconfig(cairo) >= %{cairo_version}


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
%setup -q -n %{name}-%{version}

# >> setup
# << setup

%build
# >> build pre
# << build pre

%configure --disable-static \
    --disable-gtk-doc \
    --disable-doc-cross-references \
    --with-included-modules=basic-fc \
    --disable-introspection

make %{?jobs:-j%jobs}

# >> build post
# << build post
%install
rm -rf %{buildroot}
# >> install pre
# << install pre
%make_install

# >> install post
# the ghost modules file
touch $RPM_BUILD_ROOT%{_sysconfdir}/pango/pango.modules
# << install post



%post
/sbin/ldconfig
# >> post
%{_bindir}/pango-querymodules > %{_sysconfdir}/pango/pango.modules
# << post

%postun -p /sbin/ldconfig





%files
%defattr(-,root,root,-)
# >> files
%doc AUTHORS COPYING
%{_libdir}/libpango*-*.so.*
%{_bindir}/pango-querymodules
%{_libdir}/pango
%dir %{_sysconfdir}/pango
%config %{_sysconfdir}/pango/pangox.aliases
%ghost %{_sysconfdir}/pango/pango.modules
# << files


%files devel
%defattr(-,root,root,-)
# >> files devel
%doc %{_mandir}/man1/*
%{_datadir}/man/man1/pango-view.1.gz
%doc pango-view/HELLO.txt
%{_bindir}/pango-view
%{_libdir}/libpango*.so
%{_includedir}/*
%{_libdir}/pkgconfig/*
%doc %{_datadir}/gtk-doc/html/pango
# << files devel
