## Process this file with automake to produce Makefile.in

lib_LTLIBRARIES = libIlmCtl.la

libIlmCtl_la_SOURCES = \
	CtlAddr.cpp \
	CtlAddr.h \
	CtlAlign.cpp \
	CtlAlign.h \
	CtlErrors.h \
	CtlExc.h \
	CtlExc.cpp \
	CtlFunctionCall.cpp \
	CtlFunctionCall.h \
	CtlInterpreter.cpp \
	CtlInterpreter.h \
	CtlLContext.cpp \
	CtlLContext.h \
	CtlLex.cpp \
	CtlLex.h \
	CtlMessage.cpp \
	CtlMessage.h \
	CtlModule.cpp \
	CtlModule.h \
	CtlModuleSet.cpp \
	CtlModuleSet.h \
	CtlParser.cpp \
	CtlParser.h \
	CtlRcPtr.cpp \
	CtlRcPtr.h \
	CtlReadWriteAccess.h \
	CtlSymbolTable.cpp \
	CtlSymbolTable.h \
	CtlSyntaxTree.cpp \
	CtlSyntaxTree.h \
	CtlTokens.cpp \
	CtlTokens.h \
	CtlType.cpp \
	CtlTypeStorage.cpp \
	CtlStdType.cpp \
	CtlStdType.h \
	CtlType.h \
	CtlVersion.h

libIlmCtl_la_LDFLAGS = -version-info @LIBTOOL_VERSION@ @ILMBASE_LDFLAGS@ \
			-no-undefined
libIlmCtl_la_LIBADD =  @ILMBASE_LIBS@ -lHalf -lIlmThread -lIex

libIlmCtlincludedir = $(includedir)/CTL

libIlmCtlinclude_HEADERS = \
	CtlAddr.h \
	CtlAlign.h \
	CtlErrors.h \
	CtlExc.h \
	CtlFunctionCall.h \
	CtlInterpreter.h \
	CtlLContext.h \
	CtlMessage.h \
	CtlModule.h \
	CtlRcPtr.h \
	CtlReadWriteAccess.h \
	CtlSymbolTable.h \
	CtlSyntaxTree.h \
	CtlTokens.h \
	CtlType.h \
	CtlTypeStorage.h \
	CtlStdType.h \
	CtlVersion.h

noinst_HEADERS = \
	CtlLex.h \
	CtlParser.h \
	CtlModuleSet.h

EXTRA_DIST = $(noinst_HEADERS)

INCLUDES = @ILMBASE_CXXFLAGS@ -I$(top_srcdir)
