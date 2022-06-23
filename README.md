# libbesic
Shared library to preform API calls and coordinate BESI-C device configuration


## Install
To install from source

	sudo make install

To test install install into a temporary directory

	make DESTDIR=tmp prefix=/usr install


## Build Packages
Install build dependencies

	sudo apt-get -y install devscripts debhelper gcc libcurl4-openssl-dev cython3

Then build packages

	debuild -uc -us


## Useful Links
- [Debian Packaging Manual](https://www.debian.org/doc/manuals/maint-guide/start.en.html)
- [Debian Library Packaging Guide](https://www.netfort.gr.jp/~dancer/column/libpkg-guide/libpkg-guide.html#staticonlylibs)
