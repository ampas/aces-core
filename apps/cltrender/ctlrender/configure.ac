AC_INIT([ctl], [1.4.2])
AC_SUBST(CTL_VERSION, 1.4.2)
AC_CONFIG_SRCDIR(ctl/IlmCtl/CtlModule.cpp)
AC_CONFIG_AUX_DIR([automake])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE(1.6.3)
AM_MAINTAINER_MODE

LIBTOOL_CURRENT=2
LIBTOOL_REVISION=0
LIBTOOL_AGE=0
LIBTOOL_VERSION=$LIBTOOL_CURRENT:$LIBTOOL_REVISION:$LIBTOOL_AGE
AC_SUBST(LIBTOOL_VERSION)

AS_IF([test -z "${CFLAGS}"], [
export CFLAGS="-Wall -g"
])

AS_IF([test -z "${CXXFLAGS}"], [
export CXXFLAGS="-Wall -g"
])

AC_PROG_LIBTOOL
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_CC

dnl Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDC
AC_CHECK_HEADERS(limits.h unistd.h)

AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

AC_MSG_CHECKING(for complete iomanip support in C++ standard library)
complete_iomanip="no"
AC_LANG_SAVE
AC_LANG_CPLUSPLUS
AC_TRY_COMPILE([#include <iomanip>],[

	std::right;
],
AC_DEFINE(HAVE_COMPLETE_IOMANIP) complete_iomanip=yes)
AC_MSG_RESULT($complete_iomanip)
AC_LANG_RESTORE


AC_MSG_CHECKING(for gcc optimization flags)
old_cflags=$CFLAGS
CFLAGS="$CFLAGS -pipe"
AC_TRY_COMPILE([#include <stdio.h>],
[ printf ("hello, world"); ],
[ EXTRA_OPT_CFLAGS="-pipe"],[ EXTRA_OPT_CFLAGS=""])
CFLAGS=$old_cflags
AC_MSG_RESULT([$EXTRA_OPT_CFLAGS])

AM_CFLAGS="$EXTRA_OPT_CFLAGS"
AM_CXXFLAGS="$EXTRA_OPT_CFLAGS"

LIB64_IF_EXISTS=""
if [[ -e /usr/lib64 ]]; then
   LIB64_IF_EXISTS="-L/usr/lib64"
fi

AS_IF([test -z "${PKG_CONFIG_PATH}"],[
export PKG_CONFIG_PATH=/usr/local/lib64/pkgconfig:/usr/local/lib/pkgconfig:/usr/lib/pkgconfig
])

AM_PATH_PKGCONFIG(
   [ILMBASE_CXXFLAGS],
   [ILMBASE_LDFLAGS],
   [ILMBASE_LIBS],
   [IlmBase],
   [OpenEXR],
   [$LIB64_IF_EXISTS -L/usr/local/lib],
   [-lImath -lHalf -lIex -lIlmThread],
   [ilmbase])

HAVE_OPENEXR=1
AC_ARG_ENABLE([openexr], AS_HELP_STRING([--disable-openexr], [Turn off OpenEXR support]), [AS_IF([test $enableval == no], [HAVE_OPENEXR=0])])

HAVE_LIBTIFF=1
AC_ARG_ENABLE([tiff], AS_HELP_STRING([--disable-tiff], [Turn off TIFF support]), [AS_IF([test $enableval == no], [HAVE_LIBTIFF=0])])

AS_IF([test $HAVE_OPENEXR == 1], [
AM_PATH_PKGCONFIG(
   [OPENEXR_CXXFLAGS],
   [OPENEXR_LDFLAGS],
   [OPENEXR_LIBS],
   [OpenEXR],
   [OpenEXR],
   [$LIB64_IF_EXISTS $ILMBASE_LDFLAGS -L/usr/local/lib],
   [-lIlmImf -lImath $ILMBASE_LIBS -lIex -lHalf -lz],
   [openexr])
])

AS_IF([test $HAVE_LIBTIFF],[
HAVE_JPEG=0
AC_ARG_WITH([jpeg],
   [AS_HELP_STRING([--with-jpeg=[PREFIX]],
      [jpeg image compression library @<:@default=check@:>@])])

AS_IF([test "x$with_jpeg" == xno], [HAVE_JPEG=0],
      [JPEG_LIBS="-ljpeg"
       HAVE_JPEG=1])

AS_IF([test "x$with_jpeg" != xyes],
      [JPEG_CXXFLAGS="-I$with_jpeg/include"
       JPEG_LDFLAGS="-L$with_jpeg/lib"])

AS_IF([test "$HAVE_JPEG" == 1],
      [save_CXXFLAGS="$CXXFLAGS"
       save_LDFLAGS="$LDFLAGS"
       save_LIBS="$LIBS"
       CXXFLAGS="$CXXFLAGS $JPEG_CXXFLAGS"
       LDFLAGS="$LDFLAGS $JPEG_LDFLAGS"
       AC_CHECK_LIB([jpeg], [jpeg_std_error])
       CXXFLAGS="$save_CXXFLAGS"
       LDFLAGS="$save_LDFLAGS"
       LIBS="$save_LIBS"])

HAVE_LIBTIFF=0
AC_ARG_WITH([tiff],
   [AS_HELP_STRING([--with-tiff=[PREFIX]],
      [TIFF image I/O library @<:@default=check@:>@])])

AS_IF([test "x$with_tiff" == xno], [HAVE_LIBTIFF=0],
      [TIFF_LIBS="-ltiff"
       HAVE_LIBTIFF=1])

AS_IF([test "x$with_tiff" != xyes],
      [TIFF_CXXFLAGS="-I$with_tiff/include"
       TIFF_LDFLAGS="-L$with_tiff/lib"])

AS_IF([test "$HAVE_LIBTIFF" == 1],
      [save_CXXFLAGS="$CXXFLAGS"
       save_LDFLAGS="$LDFLAGS"
       save_LIBS="$LIBS"
       CXXFLAGS="$CXXFLAGS $TIFF_CXXFLAGS"
       LDFLAGS="$LDFLAGS $TIFF_LDFLAGS"
       AC_CHECK_LIB([tiff], [TIFFOpen])
       CXXFLAGS="$save_CXXFLAGS"
       LDFLAGS="$save_LDFLAGS"
       LIBS="$save_LIBS"])
])

dnl --enable-threading
AC_ARG_ENABLE(threading,
              AC_HELP_STRING([--enable-threading],
                             [enable multi-threading [[default=yes]]]),
              [multithread="${enableval}"], [multithread=yes])

if test x$PKG_CONFIG == xno && test "x${multithread}" != xno ; then
    ACX_PTHREAD(
    [
    AC_DEFINE(HAVE_PTHREAD)
    ILMBASE_LIBS="$PTHREAD_LIBS $ILMBASE_LIBS"
    ILMBASE_CXXFLAGS="$ILMBASE_CXXFLAGS $PTHREAD_CFLAGS"
    CC="$PTHREAD_CC"

    AM_POSIX_SEM()
    ],
    [AC_MSG_ERROR([POSIX thread support required])])
    AC_MSG_NOTICE([multithread true, LIBS = $LIBS, CC = $CC, CXXFLAGS = $CXXFLAGS])
fi

AM_COMPILELINKRUN(
   [IlmBase],
   [ilmbasetest],
   [$ILMBASE_CXXFLAGS],
   [$ILMBASE_LDFLAGS],
   [$ILMBASE_LIBS],[[
#include <stdlib.h>
#include <ImathFun.h>
]],
   [[double d = Imath::succd(.23); d+= .2;]],
   AC_MSG_RESULT([Compiled and ran IlmBase test program.]),
   AC_MSG_ERROR([Could not compile IlmBase test program.]))

AS_IF([test $HAVE_OPENEXR == 1],[
AM_COMPILELINKRUN(
   [OpenEXR],
   [openexrtest],
   [$OPENEXR_CXXFLAGS],
   [$OPENEXR_LDFLAGS],
   [$OPENEXR_LIBS],[[
#include <stdlib.h>
#include <ImfTestFile.h>
]],
   [[Imf::isOpenExrFile("notExist");]],
   AC_MSG_RESULT([Compiled and ran OpenEXR test program.]),
   AC_MSG_ERROR([Could not compile OpenEXR test program.]))
   OPENEXR_CXXFLAGS="$OPENEXR_CXXFLAGS $ILMBASE_CXXFLAGS"
   OPENEXR_LDFLAGS="$OPENEXR_LDFLAGS $ILMBASE_LDFLAGS"
   OPENEXR_LIBS="$OPENEXR_LIBS $ILMBASE_LIBS"
   AC_SUBST(OPENEXR_CXXFLAGS)
   AC_SUBST(OPENEXR_LDFLAGS)
   AC_SUBST(OPENEXR_LIBS)
])

AS_IF([test $HAVE_LIBTIFF == 1],[
AM_COMPILELINKRUN(
	[libtiff],
	[libtifftest],
	[$TIFF_CXXFLAGS $JPEG_CXXFLAGS],
    [$TIFF_LDFLAGS $JPEG_LDFLAGS],
    [$TIFF_LIBS $JPEG_LIBS], [[
#include <stdlib.h>
#include <tiff.h>
#include <tiffio.h>
]],
    [[TIFFOpen("foo", "r");]],
   AC_MSG_RESULT([Compiled and ran libtiff test program.]),
   AC_MSG_ERROR([Could not compile libtiff test program.]))
])

dnl Platform-specific stuff
case "$host" in
*darwin*) 
  dnl OS X universal binary support, requires --disable-dependency-tracking
  AC_DEFINE(HAVE_DARWIN) 
  AC_ARG_ENABLE(osx-universal-binaries,
	      AC_HELP_STRING([--enable-osx-universal-binaries],
			     [build universal binaries on OS X [[default=no]]]),
	      [build_osxuniversal="${enableval}"], [build_osxuniversal=no])

  if test "${build_osxuniversal}" != no ; then
    if test "$enable_dependency_tracking" != no ; then
      AC_MSG_ERROR([--enable-osx-universal-binary requires --disable-dependency-tracking.
Please re-run configure with these options:
  --disable-dependency-tracking --enable-osx-universal-binary
  		  ])
    fi
    CXXFLAGS="$CXXFLAGS -isysroot /Developer/SDKs/MacOSX10.4u.sdk -arch ppc -arch i386"
    dnl LDFLAGS="$LDFLAGS -Wl,-syslibroot,/Developer/SDKs/MacOSX10.4u.sdk -arch ppc -arch i386"
  fi

  ;;
esac

AC_SUBST(AM_CFLAGS)
AC_SUBST(AM_CXXFLAGS)
AC_SUBST(TIFF_CXXFLAGS, ["$TIFF_CXXFLAGS $JPEG_CXXFLAGS"])
AC_SUBST(TIFF_LDFLAGS, ["$TIFF_LDFLAGS $JPEG_LDFLAGS"])
AC_SUBST(TIFF_LIBS, ["$TIFF_LIBS $JPEG_LIBS"])
AC_DEFINE(HAVE_LIBTIFF)
AC_DEFINE(HAVE_OPENEXR)
AC_DEFINE(HAVE_DARWIN)
AC_SUBST(HAVE_DARWIN)

AC_CONFIG_FILES([
Makefile
dpx/Makefile
openexr/Makefile
doc/Makefile
ctl/Makefile
ctl/CTL.pc
ctl/IlmCtl/Makefile
ctl/IlmCtlMath/Makefile
ctl/IlmCtlMathTest/Makefile
ctl/IlmCtlSimd/Makefile
ctl/IlmCtlTest/Makefile
openexr/CtlModules/Makefile
openexr/IlmImfCtl/Makefile
openexr/IlmImfCtlTest/Makefile
util/Makefile
util/ctlrender/Makefile
vc/Makefile
])
AC_OUTPUT

dnl need to add scoreboard of enabled options...
