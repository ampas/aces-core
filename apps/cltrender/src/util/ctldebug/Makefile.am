
bin_PROGRAMS=ctldebug

ctldebug_SOURCES=main.cc

ctldebug_LIBS=ctldpx tiff jpeg readline

ctldebug_CXXFLAGS=-I$(top_srcdir)/dpx -I$(top_srcdir)/ctl/IlmCtl -I$(top_srcdir)/ctl/IlmCtlSimd $(ILMBASE_CXXFLAGS) @OPENEXR_CXXFLAGS@ @TIFF_CXXFLAGS@
AM_LDFLAGS=-L$(top_builddir)/ctl/IlmCtl -L$(top_builddir)/ctl/IlmCtlSimd -L$(top_builddir)/dpx -lctldpx -lIlmCtl -lIlmCtlSimd @OPENEXR_LDFLAGS@ @OPENEXR_LIBS@ @TIFF_LDFLAGS@ @TIFF_LIBS@
