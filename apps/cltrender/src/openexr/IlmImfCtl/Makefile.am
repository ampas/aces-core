## Process this file with automake to produce Makefile.in

lib_LTLIBRARIES = libIlmImfCtl.la

INCLUDES = -I../../ctl//IlmCtl @OPENEXR_CXXFLAGS@

AM_LDFLAGS = -L../../ctl/IlmCtl @OPENEXR_LDFLAGS@ @OPENEXR_LIBS@

libIlmImfCtl_la_SOURCES = \
	ImfCtlApplyTransforms.cpp \
	ImfCtlApplyTransforms.h \
	ImfCtlCopyFunctionArg.cpp \
	ImfCtlCopyFunctionArg.h

libIlmImfCtlincludedir = $(includedir)/OpenEXR

libIlmImfCtlinclude_HEADERS = \
	ImfCtlApplyTransforms.h

noinst_HEADERS = \
	ImfCtlCopyFunctionArg.h

