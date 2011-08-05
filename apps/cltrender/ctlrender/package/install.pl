#! /usr/bin/perl

use Term::ReadLine;
use File::Glob;

use strict;

my($term)=new Term::ReadLine('install.pl');
my($version)="ctl-1.4.2";
my($default)="/usr/local/$version";
my($tarfile)="$version";
my($location);
my($linein);

my($uname_arch)=`uname -m 2>&1`;
chomp($uname_arch);
my($uname_system)=`uname -s 2>&1`;
chomp($uname_system);

if($uname_system eq 'Darwin') {
	if($uname_arch=~/^i\d86/) {
		$tarfile="$version-osx.tar.gz";
		$uname_system="OS X";
	} else {
		die("unknown Darwin (OS X) architecture '$uname_arch'.");
	}
} elsif($uname_system eq 'Linux') {
	$uname_system="linux";
	if($uname_arch=~/^i\d86/) {
		$tarfile="$version-i586.tar.gz";
		$uname_arch='i586';
	} elsif($uname_arch=~/^x86_64/) {
		$tarfile="$version-amd64.tar.gz";
		$uname_arch='amd64';
	} else {
		die("unknown linux architecture '$uname_arch'.");
	}
} else {
	die("unknown operating system '$uname_system'.");
}

print<<"__EOF__";

CTL installer for v1.4.2 ($uname_system/$uname_arch)

By default this will install the files in $default. Installing
in /usr/local is fine, but will overwrite any pre-existing OpenEXR
installation.

If installed in a location other than /usr/local it will optionally
install symbolic links into /usr/local/bin

__EOF__

if($>!=0) {
	$linein=$term->readline("Not getting run as root. Sure you want to ".
	                        "continue [NO/yes]: ");
	if(!($linein=~/^[yY]/)) {
		exit(1);
	}
}

while(1) {
	$location=$term->readline("installation location [$default]: ");
	if($location eq '') {
		$location=$default;
	}

	if(!(-d $location)) {
		$linein=$term->readline("$location does not exist. Make directory ".
		                       "[YES/no]: ");
		if($linein ne '' && !($linein=~/^[yY]/)) {
			next;
		}
		if(system("mkdir -p $location")) {
			die("unable to make directory $location.");	
		}
	}
	last;
}

$|=1;
print("Expanding archive...");
$linein=`tar xfz $tarfile -C $location 2>&1`;
if($?) {
	print("FAILED\n$linein");
	exit(1);
} else {
	print("OK\n\n");
}

my($warnings);
if($location ne $default && $location ne '/usr/local') {
	if($uname_system ne "OS X") {
		print("WARNING: you have installed into a non-standard location. ".
		      "Make sure that\nLD_LIBRARY_PATH (or /etc/ld.conf) is set to ".
		      "include $location/lib\n\n");
	} else {
		print("WARNING: you have installed into a non-standard location. ".
		      "Make sure that\nDYLD_LIBRARY_PATH is set to ".
		      "include $location/lib\n\n");
	}
	$warnings++;
}

if($uname_system ne "OS X" && !(-e "/usr/lib/libtiff.so.3")) {
	print("Don't have libtiff v3.0 in /usr/lib... ");
	if(-e "$location/lib/libtiff.so" || -e "$location/lib/libtiff.so.3") {
		print("but it's in $location/lib, so that's OK.\n");	
	} elsif(-e "/usr/lib/libtiff.so.4") {
		print("but we'll use v4.0.\n");
		if(!symlink("/usr/lib/libtiff.so.4", "$location/lib/libtiff.so.3")) {
			print("unable to symlink /usr/lib/libtiff.so.4 to\n".
			      "$location/lib/libtiff.so\n");
		}
	} else {
		print("and can't find a suitable\nreplacement. install ".
		      "libtiff via the appropriate package manager.\n");	
		exit(1);
	}
}

if($location eq '/usr/local') {
	exit(0);
}

$linein=$term->readline("Make symbolic links from $location/bin to ".
                       "/usr/local/bin [YES/no]: ");

if($linein ne '' && !($linein=~/^[yY]/)) {
	exit(1);
}

my(@bins)=File::Glob::bsd_glob("$location/bin/ctl*", File::Glob::GLOB_ERR());
my($bin, $bin_name, $link);

if(File::Glob::GLOB_ERROR) {
	print("Unable to get a list of files in $location/bin. Symlinks\n".
	      "not made.\n");
}

print("\n");
foreach $bin (@bins) {
	if(!($bin=~m@([^/]+)$@)) {
		die("Uh Oh. Problem in installer ($bin has no file).");
	}
	$bin_name=$1;
	if(!(-e "/usr/local/bin/$bin_name")) {
	} elsif(-l "/usr/local/bin/$bin_name") {
		$link=readlink("/usr/local/bin/$bin_name");
		if(!defined($link)) {
			print("unable to figure out where /usr/local/bin/$bin_name links ".
			      "to,\nignoring it.\n");
			$warnings++;
			next;
		} elsif($link=~m@/ctl-\d+\.\d+\.\d+/bin@) {	
			print("updating /usr/local/bin/$bin_name\n");
			unlink("/usr/local/bin/$bin_name");
		} else {
			print("/usr/local/bin/$bin_name linked someplace unexpected, ".
			      "ignoring it.\n");
			$warnings++;
			next;
		}
	} else {
		print("/usr/local/bin/$bin_name is not a symbolic link, ignoring ".
		      "it.\n");
		$warnings++;
		next;
	}
	print("making symlink from $bin to /usr/local/bin/$bin_name\n");
	if(!symlink($bin, "/usr/local/bin/$bin_name")) {
		print("unable to make /usr/local/bin/$bin_name link ($!).\n");
		$warnings++;
	}
}

print("\n");
if($warnings) {
	print("installation complete with minor issues.\n");
	exit(1);
} else {
	print("installation complete.\n");
	exit(0);
}

# vim: ts=4
