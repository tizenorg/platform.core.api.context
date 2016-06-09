Name:       context
Summary:    Tizen Context Framework Native API
Version:    0.9.1
Release:    1
Group:      Service/Context
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz

%define BUILD_PROFILE %{?profile}%{!?profile:%{?tizen_profile_name}}

%define SYSTEM_SERVICE	0
%define LEGACY_APPFW	0

%if "%{?BUILD_PROFILE}" == "tv"
ExcludeArch: %{arm} aarch64 %ix86 x86_64
%endif

BuildRequires: cmake
BuildRequires: pkgconfig(gio-2.0)
BuildRequires: pkgconfig(context-common)
BuildRequires: pkgconfig(aul)
BuildRequires: pkgconfig(bundle)
BuildRequires: pkgconfig(capi-appfw-app-control)
BuildRequires: pkgconfig(pkgmgr-info)

%description
Tizen Context Framework Native API

%prep
%setup -q

%build
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`

export   CFLAGS+=" -Wextra -Wcast-align -Wshadow -Wwrite-strings -Wswitch-default -Wno-unused-parameter"
export CXXFLAGS+=" -Wextra -Wcast-align -Wshadow -Wwrite-strings -Wswitch-default -Wno-unused-parameter"

export   CFLAGS+=" -Wno-empty-body -fomit-frame-pointer -fno-optimize-sibling-calls"
export CXXFLAGS+=" -Wno-empty-body -fomit-frame-pointer -fno-optimize-sibling-calls"

export   CFLAGS+=" -fno-strict-aliasing -fno-unroll-loops -fsigned-char -fstrict-overflow"
export CXXFLAGS+=" -fno-strict-aliasing -fno-unroll-loops -fsigned-char -fstrict-overflow"

export   CFLAGS+=" -fno-common"
export CXXFLAGS+=" -Wnon-virtual-dtor"
export CXXFLAGS+=" -std=c++0x"

#export   CFLAGS+=" -Wcast-qual"
#export CXXFLAGS+=" -Wcast-qual"

#export   CFLAGS+=" -DTIZEN_ENGINEER_MODE"
#export CXXFLAGS+=" -DTIZEN_ENGINEER_MODE"
#export   FFLAGS+=" -DTIZEN_ENGINEER_MODE"

%if "%{?BUILD_PROFILE}" == "tv"
export   CFLAGS+=" -D_ALLOW_SERVICE_APP_TRIGGER_"
export CXXFLAGS+=" -D_ALLOW_SERVICE_APP_TRIGGER_"
%endif

cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix} -DMAJORVER=${MAJORVER} -DFULLVER=%{version} \
							   -DPROFILE=%{?BUILD_PROFILE} \
							   -DSYSTEM_SERVICE=%{SYSTEM_SERVICE} \
							   -DLEGACY_APPFW=%{LEGACY_APPFW}
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

%package test
Summary:    Tizen Context Framework Testsuite
Group:      Service/Context

%description test
Tizen Context Framework Testsuite

%files test
%manifest testsuite/context-test.manifest
%{_bindir}/context-test
