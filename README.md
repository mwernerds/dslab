# dslab
Data Science Lab
# dslab 1.0
DSLAB is a cross-platform library built using wx-widgets and C++ in order to facilitate easy and fast visual data science applications in C++.
# Initial Documentation
This is a simple autotools system. Basically, you should

./configure
make
sudo make install

and then be able to use it in your projects by adding -ldslab linker switch


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




