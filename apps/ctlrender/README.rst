Installation
============

Repository Organization
***********************

This directory and subdirectories contain the source code for the ctlrender application.

prereqs - contains the expanded source trees for the prerequisites required to build ctlrender.
ctlrender - contains the expanded source tree for the ctlrender package which includes a version of CTL.


Instructions for building ctlrender on Mac OS X
***********************************************

Mac OS X builds require Apple's Xcode developer tools to be installed:

For Mac OS X 10.6.8, make sure you have Xcode 4.0.2.
For Mac OS X 10.5.8, make sure you have Xcode 3.1.4.
The build has not been tested on the new Mac OS X Lion (10.7), but it should
work on that version as well. Lion requires Xcode version 4.1

Xcode can be downloaded from the Apple Developer web site at  http://developer.apple.com

To build ctlrender, first download the ACES Developer Resources from
git@github.com:ampas/aces-dev.git.  For the purpose of these instructions, it
is assumed the downloaded repository directory named aces-dev is in the home
directory (i.e. ~/aces-dev). If you choose another directory name, substitute
that name in the instructions below.

Open a Terminal command-line window.

Change directories to the ACES subdirectory containing the ctlrender source::

	cd ~/aces-dev/apps/ctlrender

Create the following soft link::

	ln -s /usr/bin/true /bin/true

Build ilmbase-1.0.2::

	cd ~/aces-dev/apps/ctlrender/prereqs/ilmbase-1.0.2
	./configure
	make
	sudo make install

Build openexr-1.7.0::

	cd ~/aces-dev/apps/ctlrender/prereqs/openexr-1.7.0
	./configure
	make
	sudo make install

Build jpeg-6b::

	cd ~/aces-dev/apps/ctlrender/prereqs/jpeg-6b
	./configure
	make
	sudo make install

Build tiff-3.8.2::

	cd ~/aces-dev/apps/ctlrender/prereqs/tiff-3.8.2
	./configure
	make
	sudo make install

Build ctlrender::

	cd ~/aces-dev/apps/ctlrender/src
	./configure
	make
	sudo make install

A newly built ctlrender program should exist in /usr/local/bin

Instructions for building ctlrender on Ubuntu 10.04.2 (64-bit)
**************************************************************

Open a Terminal command-line window.

Install the developer tools::

	sudo apt-get install build-essential
	sudo apt-get install autoconf
	sudo apt-get install subversion

Install the zlib library (required by openexr-1.7.0)::

	sudo apt-get install zlib1g-dev

To build ctlrender, first download the ACES Developer Resources from
git@github.com:ampas/aces-dev.git.  For the purpose of these instructions, it
is assumed the downloaded repository directory named aces-dev is in the home
directory (i.e. ~/aces-dev). If you choose another directory name, substitute
that name in the instructions below.

Enter the following command::

	export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
	Note: To make the LD_LIBRARY_PATH setting permanent, add the above line to ~/.bash_profile

Change directories to the ACES subdirectory containing the ctlrender source::

	cd ~/aces-dev/apps/ctlrender

Build ilmbase-1.0.2::

	cd ~/aces-dev/apps/ctlrender/prereqs/ilmbase-1.0.2
	./configure
	make
	sudo make install

Build openexr-1.7.0::

	cd ~/aces-dev/apps/ctlrender/prereqs/openexr-1.7.0
	./configure
	make
	sudo make install

Build jpeg-6b::

	cd ~/aces-dev/apps/ctlrender/prereqs/jpeg-6b
	./configure
	make
	sudo make install

Build tiff-3.8.2::

	cd ~/aces-dev/apps/ctlrender/prereqs/tiff-3.8.2
	./configure
	make
	sudo make install

Build ctlrender::

	cd ~/aces-dev/apps/ctlrender/src
	./configure
	make
	sudo make install

A newly built ctlrender program should exist in /usr/local/bin
