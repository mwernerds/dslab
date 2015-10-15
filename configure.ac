AC_PREREQ(2.57)
AC_INIT([libdslab], [0.3], [martin@martinwerner.de])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AM_OPTIONS_WXCONFIG
reqwx=2.4.0
AM_PATH_WXCONFIG($reqwx, wxWin=1)
if test "$wxWin" != 1; then
	AC_MSG_ERROR([
		wxWidgets must be installed on your system.
 
		Please check that wx-config is in path, the directory
		where wxWidgets libraries are installed (returned by
		'wx-config --libs' or 'wx-config --static --libs' command)
		is in LD_LIBRARY_PATH or equivalent variable and
		wxWidgets version is $reqwx or above.
		])
fi

#Custom OpenGL linker flags
 AC_ARG_WITH([opengl-libs], AS_HELP_STRING([--with-opengl-libs=linker flags],
 [OpenGL linker flags (-lGL -lGLU for some linux, some -framework OpenGL stuff for Mac]),
 [GL_LIBS="$withval"], [GL_LIBS='-lGL -lGLU'])

#
#  This is a special Windows option
#
AC_ARG_ENABLE(windows,Use this option if building for Windows,
[
CXXFLAGS="$CXXFLAGS -uWinMain"
GL_LIBS=""
])

WX_LIBS_OGL=`$WX_CONFIG_WITH_ARGS --gl-libs $4`

CPPFLAGS="$CPPFLAGS $WX_CPPFLAGS -m64"
CXXFLAGS="$CXXFLAGS $WX_CXXFLAGS_ONLY -m64"
CFLAGS="$CFLAGS $WX_CFLAGS_ONLY -m64"
LIBS="$LIBS $WX_LIBS $WX_LIBS_OGL $GL_LIBS"
AC_PROG_CXX
AC_PROG_RANLIB
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
