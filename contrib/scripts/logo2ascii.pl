#!/usr/bin/perl -p
# logo2ascii
# Takes a logo and converts it to use only ASCII characters
# (used to convert to ascii some of the logos in the distribution
# so they can be introduced into parse_logo)
# (c) Javier Fernandez-Sanguino Pen~a <jfs@computer.org>
#
# Distributed under the terms of the GNU GPL license
# see http://www.gnu.org

s/\[\d+;\d+m//g;
s/\[\d+m//g;
s/\[[\d;]+m//g;
# special case: spaces
while ( /\[(\d)C/ ) {
	$spaces = " " x $1;
	s/\[$1C/$spaces/g;
}
