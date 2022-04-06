# libbesic
Shared library to coordinate BESI-C device configuration


## Install
To install from source

	sudo make install


## Build Packages
Install build dependencies

	sudo apt-get -y install devscripts debhelper gcc

Then build packages

	debuild -uc -us


## Useful Links
- [Debian Packaging Manual](https://www.debian.org/doc/manuals/maint-guide/start.en.html)
- [Debian Library Packaging Guide](https://www.netfort.gr.jp/~dancer/column/libpkg-guide/libpkg-guide.html#staticonlylibs)
