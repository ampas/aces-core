## Process this file with automake to produce Makefile.in

lib_LTLIBRARIES = libIlmCtlSimd.la

libIlmCtlSimd_la_SOURCES = \
	CtlSimdAddr.cpp \
	CtlSimdAddr.h \
	CtlSimdCFunc.h \
	CtlSimdFunctionCall.cpp \
	CtlSimdFunctionCall.h \
	CtlSimdHalfExpLog.cpp \
	CtlSimdHalfExpLog.h \
	CtlSimdInst.cpp \
	CtlSimdInst.h \
	CtlSimdInterpreter.cpp \
	CtlSimdInterpreter.h \
	CtlSimdLContext.cpp \
	CtlSimdLContext.h \
	CtlSimdModule.cpp \
	CtlSimdModule.h \
	CtlSimdOp.h \
	CtlSimdReg.cpp \
	CtlSimdReg.h \
	CtlSimdStdLibAssert.cpp \
	CtlSimdStdLibAssert.h \
	CtlSimdStdLibColorSpace.cpp \
	CtlSimdStdLibColorSpace.h \
	CtlSimdStdLibInterpolator.cpp \
	CtlSimdStdLibInterpolator.h \
	CtlSimdStdLibLimits.cpp \
	CtlSimdStdLibLimits.h \
	CtlSimdStdLibLookupTable.cpp \
	CtlSimdStdLibLookupTable.h \
	CtlSimdStdLibMath.cpp \
	CtlSimdStdLibMath.h \
	CtlSimdStdLibPrint.cpp \
	CtlSimdStdLibPrint.h \
	CtlSimdStdLibrary.cpp \
	CtlSimdStdLibrary.h \
	CtlSimdStdLibTemplates.h \
	CtlSimdStdTypes.cpp \
	CtlSimdStdTypes.h \
	CtlSimdSyntaxTree.cpp \
	CtlSimdSyntaxTree.h \
	CtlSimdType.cpp \
	CtlSimdType.h \
	CtlSimdXContext.cpp \
	CtlSimdXContext.h

libIlmCtlSimd_la_LDFLAGS = -version-info @LIBTOOL_VERSION@ @ILMBASE_LDFLAGS@ \
			   -no-undefined

libIlmCtlSimd_la_LIBADD = @ILMBASE_LIBS@ \
			  ../IlmCtl/libIlmCtl.la ../IlmCtlMath/libIlmCtlMath.la

libIlmCtlSimdincludedir = $(includedir)/CTL

libIlmCtlSimdinclude_HEADERS = \
	CtlSimdInterpreter.h

noinst_HEADERS = \
	CtlSimdAddr.h \
	CtlSimdCFunc.h \
	CtlSimdFunctionCall.h \
	CtlSimdHalfExpLog.h \
	CtlSimdInst.h \
	CtlSimdLContext.h \
	CtlSimdModule.h \
	CtlSimdOp.h \
	CtlSimdReg.h \
	CtlSimdStdLibAssert.h \
	CtlSimdStdLibColorSpace.h \
	CtlSimdStdLibInterpolator.h \
	CtlSimdStdLibLimits.h \
	CtlSimdStdLibLookupTable.h \
	CtlSimdStdLibMath.h \
	CtlSimdStdLibPrint.h \
	CtlSimdStdLibrary.h \
	CtlSimdStdLibTemplates.h \
	CtlSimdStdTypes.h \
	CtlSimdSyntaxTree.h \
	CtlSimdType.h \
	CtlSimdXContext.h

INCLUDES = @ILMBASE_CXXFLAGS@ -I.. \
	-I../IlmCtlMath \
	-I../IlmCtl

# halfExpLogTable.ccp is used to build halfExpLogTable.h dynamically
EXTRA_DIST = halfExpLogTable.cpp

CLEANFILES = halfExpLogTable halfExpLogTable.h

halfExpLogTable_SOURCES = halfExpLogTable.cpp

halfExpLogTable_LDFLAGS = -version-info @LIBTOOL_VERSION@ @ILMBASE_LDFLAGS@ \
			  @ILMBASE_LIBS@ -no-undefined

halfExpLog.h halfExpLogTable.h:	halfExpLogTable
	./halfExpLogTable  halfExpLogTable.h halfExpLog.h

BUILT_SOURCES = halfExpLogTable.h

noinst_PROGRAMS = halfExpLogTable
