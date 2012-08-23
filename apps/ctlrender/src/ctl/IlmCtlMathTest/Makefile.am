## Process this file with automake to produce Makefile.in

check_PROGRAMS = IlmCtlMathTest

IlmCtlMathTest_SOURCES = \
	main.cpp \
	testAffineRec.cpp \
	testAffineRec.h \
	testGaussRec.cpp \
	testGaussRec.h

IlmCtlMathTest_CPPFLAGS =

INCLUDES = @ILMBASE_CXXFLAGS@ \
	-I$(top_builddir) \
	-I$(top_srcdir)/IlmCtlMath

AM_LDFLAGS = @ILMBASE_LDFLAGS@

LDADD =	@ILMBASE_LIBS@ -L$(top_builddir)/IlmCtlMath \
	-lIlmCtlMath -lImath -lIex -lHalf -lpthread -lm

TESTS = IlmCtlMathTest
