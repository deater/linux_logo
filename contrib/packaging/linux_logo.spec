Summary:	Displays an ANSI or ASCII penguin and system information.
Name:		linux_logo
Version:	3.9b1
Release:	1
Copyright:	GPL
Group:		Applications/System
Source:		http://www.deater.net/weave/vmwprod/%{name}/%{name}-%{version}.tar.gz
URL:		http://www.deater.net/weave/vmwprod/%{name}/
Vendor:		VMW Software
BuildRoot:	%{_tmppath}/%{name}-%{version}-root
Prefix:		%{_prefix}
Packager:	Chris Ausbrooks <weed@bucket.pp.ualr.edu>

%description
Linux Logo is a small utility that displays an ANSI or ASCII logo of the
Linux penguin, along with some system information.

%prep
%setup -n %{name}-%{version}

%build
make
gzip -v9 %{name}.1

%install
rm -rf %{buildroot}
install -D -m 755 %{name} %{buildroot}/%{prefix}/bin/%{name}
install -D -m 644 %{name}.1.gz %{buildroot}/%{prefix}/man/man1/%{name}.1.gz

%clean
rm -rf %{buildroot}

%post
if [ -f /etc/rc.d/rc.local ]; then
	LOGOTMP=/tmp/rc.local.tmp
	if [ `which mktemp 2> /dev/null` ]; then
		LOGOTMP=`mktemp /tmp/rc.local.XXXXXX`
	fi
	grep -v LINUX_LOGO.RPM /etc/rc.d/rc.local > $LOGOTMP
	cat $LOGOTMP > /etc/rc.d/rc.local
	rm -f $LOGOTMP
fi
echo if [ \`which linux_logo 2\> /dev/null\` ]\; then"	"\
	\#LINUX_LOGO.RPM >> /etc/rc.d/rc.local
echo "	"if [ -f /etc/redhat-release ]\; then"	"\
	\#LINUX_LOGO.RPM >> /etc/rc.d/rc.local
echo "		"DASHT=\"-t\""			"\
	\#LINUX_LOGO.RPM >> /etc/rc.d/rc.local
echo "	"fi"					"\
	\#LINUX_LOGO.RPM >> /etc/rc.d/rc.local
echo "	"linux_logo -f \$DASHT \"\$R\" \> /etc/issue"	"\
	\#LINUX_LOGO.RPM >> /etc/rc.d/rc.local
echo "	"cp -f /etc/issue /etc/issue.net"		"\
	\#LINUX_LOGO.RPM >> /etc/rc.d/rc.local
echo fi"						"\
	\#LINUX_LOGO.RPM >> /etc/rc.d/rc.local
if [ `which linux_logo 2> /dev/null` ]; then
	if [ -f /etc/redhat-release ]; then
		DASHT="-t"
		R=`cat /etc/redhat-release`
	fi
	linux_logo -f $DASHT "$R" > /etc/issue
	cp -f /etc/issue /etc/issue.net
fi

%postun
if [ $1 = 0 ]; then
	if [ -f /etc/rc.d/rc.local ]; then
		LOGOTMP=/tmp/rc.local.tmp
		if [ `which mktemp 2> /dev/null` ]; then
			LOGOTMP=`mktemp /tmp/rc.local.XXXXXX`
		fi
		grep -v LINUX_LOGO.RPM /etc/rc.d/rc.local > $LOGOTMP
		cat $LOGOTMP > /etc/rc.d/rc.local
		rm -f $LOGOTMP
	fi
	if [ -f /etc/redhat-release ]; then
		R=$(cat /etc/redhat-release)
		arch=$(uname -m)
		a="a"
		case "_$arch" in
			_a*) a="an";;
			_i*) a="an";;
		esac
		NUMPROC=`egrep -c "^cpu[0-9]+" /proc/stat`
		if [ "$NUMPROC" -gt "1" ]; then
			SMP="$NUMPROC-processor "
			if [ "$NUMPROC" = "8" -o "$NUMPROC" = "11" ]; then
				a="an"
			else
				a="a"
			fi
		fi
		echo "" > /etc/issue
		echo "$R" >> /etc/issue
		echo "Kernel $(uname -r) on $a $SMP$(uname -m)" >> /etc/issue
		cp -f /etc/issue /etc/issue.net
		echo >> /etc/issue
	fi
fi

%files
%defattr(-, root, root)
%doc ANNOUNCE.logo BUGS CHANGES COPYING linux_logo-3.9b1.lsm LINUX_LOGO.FAQ
%doc README README.CUSTOM_LOGOS samples TODO
%{prefix}/bin/*
%{prefix}/man/*/*

%changelog
* Thu Mar 01 2001 Chris Ausbrooks <weed@bucket.pp.ualr.edu>
- pretty much completely rewrote specfile
- updated URLs
- added to docs
- included man page
- got rid of patch
- renamed spec file
- dirty hack to add to rc.local

* Mon Jul 24 2000 Prospector <prospector@redhat.com>
- rebuilt

* Wed Jul 12 2000 Than Ngo <than@redhat.de>
- use RPM macros

* Mon Jul 03 2000 Prospector <bugzilla@redhat.com>
- automatic rebuild

* Sat May 27 2000 Ngo Than <than@redhat.de>
- update to 3.05 for 7.0
- cleanup specfile
- use RPM_OPT_FLAGS

* Thu Nov 18 1999 Ngo Than <than@redhat.de>
- initial RPM
