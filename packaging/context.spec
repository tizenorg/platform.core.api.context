Name:       context
Summary:    Tizen Context Framework Native API
Version:    0.4.2
Release:    1
Group:      System/API
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz

BuildRequires: cmake
BuildRequires: pkgconfig(aul)
BuildRequires: pkgconfig(bundle)
BuildRequires: pkgconfig(capi-appfw-app-control)
BuildRequires: pkgconfig(pkgmgr-info)
BuildRequires: pkgconfig(libsmack)
BuildRequires: pkgconfig(context-common)

%ifarch %{arm}
%define ARCH arm
%else
%define ARCH i586
%endif

%description
Tizen Context Framework Native API

%prep
%setup -q

%build
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`

export   CFLAGS+=" -Wextra -Wcast-align -Wcast-qual -Wshadow -Wwrite-strings -Wswitch-default"
export CXXFLAGS+=" -Wextra -Wcast-align -Wcast-qual -Wshadow -Wwrite-strings -Wswitch-default -Wnon-virtual-dtor -Wno-c++0x-compat"

export   CFLAGS+=" -Wno-unused-parameter -Wno-empty-body"
export CXXFLAGS+=" -Wno-unused-parameter -Wno-empty-body"

export   CFLAGS+=" -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-strict-aliasing -fno-unroll-loops -fsigned-char -fstrict-overflow -fno-common"
export CXXFLAGS+=" -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-strict-aliasing -fno-unroll-loops -fsigned-char -fstrict-overflow"

%define BINTYPE engineer
export   CFLAGS+=" -DTIZEN_ENGINEER_MODE"
export CXXFLAGS+=" -DTIZEN_ENGINEER_MODE"
export   FFLAGS+=" -DTIZEN_ENGINEER_MODE"

cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix} -DARCH=%{ARCH} -DMAJORVER=${MAJORVER} -DFULLVER=%{version} -DPROFILE=%{?tizen_profile_name} -DBINTYPE=%{BINTYPE}
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

mkdir -p %{buildroot}/usr/share/license
cp LICENSE %{buildroot}/usr/share/license/%{name}

%post
/sbin/ldconfig

%postun
/sbin/ldconfig

%files
%manifest packaging/%{name}.manifest
%defattr(-,root,root,-)
%{_libdir}/*.so*
/usr/share/license/%{name}
#%if %{BINTYPE} == "engineer"
#/usr/bin/*
#%endif

%package devel
Summary:    Tizen Context Framework Native API (Development)
Group:      System/API
Requires:	%{name} = %{version}-%{release}

%description devel
Tizen Context Framework Native API (Development)

%files devel
%defattr(-,root,root,-)
%{_includedir}/context-service/*.h
%{_libdir}/pkgconfig/%{name}.pc
