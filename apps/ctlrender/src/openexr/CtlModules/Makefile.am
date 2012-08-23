## Process this file with automake to produce Makefile.in

EXTRA_DIST = \
	./utilities/*.ctl \
	./rrt/*.ctl

ctldatadir = $(sharedir)/ctl/modules
ctldata_DATA = \
	./utilities/*.ctl \
	./rrt/*.ctl

# since ctl files appear above, tell the makefile to do nothing
# with them (except install them)
$(EXTRA_DIST):
	@true
