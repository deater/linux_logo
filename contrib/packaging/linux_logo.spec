Summary:	Displays an ANSI or ASCII penguin and system information.
Name:		linux_logo
Version:	4.09
Release:	1cma
Copyright:	GPL
Group:		Applications/System
Source0:	http://www.deater.net/weave/vmwprod/%{name}/%{name}-%{version}.tar.gz
URL:		http://www.deater.net/weave/vmwprod/%{name}/
Vendor:		VMW Software
BuildRoot:	%{_tmppath}/%{name}-%{version}-root
Prefix:		%{_prefix}
Packager:	Chris Ausbrooks <weed@thebucket.org>

%description
Linux Logo is a small utility that displays an ANSI or ASCII logo of the
Linux penguin, along with some system information.

%prep
%setup -n %{name}-%{version}

%build
ls ./logos/*/* >> logo_config
make
(	echo '#!/bin/bash'
	echo '#'
	echo '# linux_logo	Hook to allow linux_logo to update issue and issue.net'
	echo '#'
	echo '# chkconfig: 2345 99 00'
	echo '# description: Linux_logo puts up a greeting screen.'
	echo ''
	echo '# Source the function library.'
	echo '. /etc/init.d/functions'
	echo ''
	echo 'RETVAL=0'
	echo ''
	echo '# See how we were called.'
	echo 'case "${1}" in'
	echo '	start|restart|reload)'
	echo '		if [ -x /'%{prefix}'/bin/linux_logo -a -f /etc/issue ] ; then'
	echo '			if [ -f /etc/redhat-release ]; then'
	echo '				DASHT="-t"'
	echo '				RRELEASE=$(cat /etc/redhat-release)'
	echo '			fi'
	echo '			action "Updating issue: "echo $(/'%{prefix}'/bin/linux_logo -f ${DASHT} "${RRELEASE}" > /etc/issue)'
	echo '			if [ -f /etc/issue.net ]; then'
	echo '				action "Updationg issue.net: " echo $(cat /etc/issue > /etc/issue.net)'
	echo '			fi'
	echo '			RETVAL=${?}'
	echo '			[ "${RETVAL}" = 0 ] && touch /var/lock/subsys/linux_logo'
	echo '		fi'
	echo '		;;'
	echo '	stop)'
	echo '		# ignore'
	echo '		rm -f /var/lock/subsys/linux_logo'
	echo '		;;'
	echo '	*)'
	echo '		echo "Usage: ${0} {start|stop|restart|reload}"'
	echo '		RETVAL=1'
	echo 'esac'
	echo ''
	echo 'exit ${RETVAL}'
) > linux_logo.init
touch linux_logo.conf

%install
rm -rf %{buildroot}
install -D -m 755 %{name} %{buildroot}/%{prefix}/bin/%{name}
install -D -m 644 %{name}.1.gz %{buildroot}/%{prefix}/man/man1/%{name}.1.gz
install -D -m 755 %{name}.init %{buildroot}/etc/rc.d/init.d/%{name}
install -D -m 644 %{name}.conf %{buildroot}/etc/%{name}.conf

%clean
rm -rf %{buildroot}

%post
if [ -f /etc/rc.d/rc.local ]; then
	LOGOTMP=/tmp/rc.local.tmp
	if [ $(which mktemp 2> /dev/null) ]; then
		LOGOTMP=$(mktemp /tmp/rc.local.XXXXXX)
	fi
	grep -v LINUX_LOGO.RPM /etc/rc.d/rc.local > ${LOGOTMP}
	cat ${LOGOTMP} > /etc/rc.d/rc.local
	rm -f ${LOGOTMP}
fi
/sbin/chkconfig --add linux_logo
/sbin/service linux_logo restart > /dev/null 2>&1 || :
if [ -f /etc/linux_logo.conf -a ! -s /etc/linux_logo.conf ] ; then
	if [ -f /etc/redhat-release ]; then
		DASHT="-t"
		RRELEASE=$(cat /etc/redhat-release)
	fi
	echo "-f ${DASHT} \"${RRELEASE}\"" >> /etc/linux_logo.conf
fi

%preun
if [ "${1}" = 0 ]; then
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
	/sbin/service linux_logo stop > /dev/null 2>&1 || :
	/sbin/chkconfig --del linux_logo
fi

%postun
/sbin/service linux_logo restart > /dev/null 2>&1 || :

%files
%defattr(-, root, root)
%doc ANNOUNCE.logo BUGS CHANGES COPYING %{name}-%{version}.lsm LINUX_LOGO.FAQ
%doc README README.CUSTOM_LOGOS TODO USAGE
%config(noreplace) /etc/linux_logo.conf
/etc/rc.d/*/*
%{prefix}/bin/*
%{prefix}/man/*/*

%changelog
* Tue Mar 05 2002 Chris Ausbrooks <weed@thebucket.org>
- updated to 4.01
- got rid of extra file
- added conf manipulation

* Thu Dec 06 2001 Chris Ausbrooks <weed@thebucket.org>
- updated to b5
- fixed email address

* Sun Oct 14 2001 Chris Ausbrooks <weed@bucket.pp.ualr.edu>
- added rc.d file, removed rc.local mods

* Mon Mar 05 2001 Chris Ausbrooks <weed@bucket.pp.ualr.edu>
- iterated version number
- added more logos to logo_config now that they all work

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
