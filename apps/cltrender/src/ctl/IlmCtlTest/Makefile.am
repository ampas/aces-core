## Process this file with automake to produce Makefile.in

check_PROGRAMS = IlmCtlTest

IlmCtlTest_SOURCES = \
	main.cpp \
	testCppCall.cpp \
	testCppCall.h \
	testEndOfLine.cpp \
	testEndOfLine.h \
	testExamples.cpp \
	testExamples.h \
	testHugeInit.cpp \
	testHugeInit.h \
	testParser.cpp \
	testParser.h \
	testVarying.cpp \
	testVarying.h \
	testVaryingLookup.cpp \
	testVaryingLookup.h \
	testVaryingReturn.cpp \
	testVaryingReturn.h

IlmCtlTest_CPPFLAGS =

INCLUDES = -I$(top_builddir) \
	-I$(top_srcdir)/IlmCtl \
	-I$(top_srcdir)/IlmCtlSimd \
	@ILMBASE_CXXFLAGS@

LDADD =	-L$(top_builddir)/IlmCtlSimd \
	-L$(top_builddir)/IlmCtlMath \
	-L$(top_builddir)/IlmCtl \
	@ILMBASE_LDFLAGS@ @ILMBASE_LIBS@ \
	-lIlmCtl -lIlmCtlSimd -lIlmCtlMath -lImath -lIex -lHalf -lpthread

TESTS = IlmCtlTest
TESTS_ENVIRONMENT = CTL_MODULE_PATH=$(top_srcdir)/IlmCtlTest:.

EXTRA_DIST = \
	common.ctl \
	example.ctl \
	testArray.ctl \
	testCast.ctl \
	testComments.ctl \
	testCppCall.ctl \
	testCtlVersion.ctl \
	test.ctl \
	testDefaults.ctl \
	testEmpty.ctl \
	testExamples.ctl \
	testExamplesNamespace.ctl \
	testExpr.ctl \
	testFunc.ctl \
	testHugeInit.ctl \
	testInterpolator.ctl \
	testLiterals.ctl \
	testLookupTables.ctl \
	testLoops.ctl \
	testName2.ctl \
	testName.ctl \
	testNameSpace2.ctl \
	testNameSpace.ctl \
	testNoName.ctl \
	testParse.ctl \
	testScope2.ctl \
	testScope.ctl \
	testStdLibrary.ctl \
	testStruct.ctl \
	testTypes.ctl \
	testVarying.ctl \
	testVaryingLookup.ctl \
	testVaryingReturn.ctl \
	testVSArrays.ctl
