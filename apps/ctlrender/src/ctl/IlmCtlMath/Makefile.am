## Process this file with automake to produce Makefile.in

lib_LTLIBRARIES = libIlmCtlMath.la

libIlmCtlMath_la_SOURCES = \
	CtlColorSpace.cpp \
	CtlColorSpace.h \
	CtlLinearSolver.h \
	CtlLookupTable.cpp \
	CtlLookupTable.h \
	CtlPointTree.h \
	CtlRbfInterpolator.cpp \
	CtlRbfInterpolator.h \
	CtlSparseMatrix.h

libIlmCtlMath_la_LDFLAGS = -version-info @LIBTOOL_VERSION@ @ILMBASE_LDFLAGS@ \
			   -no-undefined

libIlmCtlMath_la_LIBADD = @ILMBASE_LIBS@ -lImath -lIex

libIlmCtlMathincludedir = $(includedir)/CTL

libIlmCtlMathinclude_HEADERS = \
	CtlColorSpace.h \
	CtlLookupTable.h \
	CtlRbfInterpolator.h \
	CtlSparseMatrix.h

noinst_HEADERS = \
	CtlLinearSolver.h \
	CtlPointTree.h

INCLUDES = @ILMBASE_CXXFLAGS@ -I$(top_builddir)
