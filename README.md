# dslab
DSLAB is a cross-platform environment for portable and fast computer science applications in C++.

DSLAB provides an interface (the class DataEngine) from which you subclass to get your functionality. DSLAB is based on a simple interface allowing for cross-platform, OpenGL-ready, animatable and fast applications to showcase your research.

#Features
* Cross-Platform GUI (including menu bars, dialog boxes, controls, file open and close, and more)
* Linux and Windows from the same source code
* OpenGL, usually hardware-accelerated, with timed animation support
* MXE  cross-compilation to Windows 64 bit and Windows 32 bit easy
* C++11 support (and even newer C++ extensions as far as GCC supports them)
* ZoomPan implementation (for SPATIAL applications with a 2D map type interface)
* autotools support
* Simple and encapsulated implementation of Data Science Algorithms

These functionalitites are made possible by using wxWidgets for the cross-plattform stuff and carefully implementing an interface for extending the application with varying data science applets defined by a single class.



# Initial Documentation
This is a simple autotools system. Basically, you should

./configure
make
sudo make install

and then be able to use it in your projects by adding -ldslab linker switch. Don't forget
to add additional libraries such as GL or GLU for your project.

# Preparing on Debian
In addition to a basic C++ development environment (g++, make, autotools, ...), this
project uses wxWidgets, which can be installed on Debian by



Cross-Compiling:

However, the system is ready for windows cross-compile. I am using
mxe and installed and made available the x86_64-w64-mingw32 toolchain.

The following config line was good on my debian system. Note, that
I first configured an up-to-date source of wx-widgets with a prefix

/usr/local/wx64

which creates essentially wx-config. you have to point configure to
this exact wx-config for your tool-chain

Secondly, I give a prefix in order to be able to install all files without
breaking my native installation (for linux, not windows)

Finally, the --enable-windows switch is being used in order to be able
to build the examples. This is due to the fact that the GNU linker
does not find WinMain as it is part of the library. The problem behind is
that the linker drops all functions from libraries, which are not needed, but
adds some mingw libraries looking for winmain at the end of the actual linker
call. Look at configure.ac and the rule for
--enable-windows
because you might have to do the same when compiling your own projects.

Note: I changed ownership of /opt/dslab to my user, because the cross compiler
is unavailable using sudo. So I do the configure below followed by 
make 
make install # (Without sudo!)

sudo make install fails because it did not find x86_64-w64-mingw32-ranlib...

Working config-line (check paths, compiler and installations!!) for Windows

./configure --prefix=/opt/dslab --host=x86_64-w64-mingw32 --with-wx-config=/usr/local/wx64/bin/wx-config --enable-windows




