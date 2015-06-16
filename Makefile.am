# Makefile.am - by Edward Grace <ej.grace@imperial.ac.uk>
#
# automake Makefile
#
# $Id:$
#

@SET_MAKE@

# Define the binaraies (programs) to build 
bin_PROGRAMS = pdf2svg

# For each binary define the sources that it depends on, the C flags and any librarys
# N.B. POPPLERGLIB_ should be set by the configure script either using pkg-config, 
# by guessing where the librarys may be, or by the user specifying.
pdf2svg_SOURCES = pdf2svg.c
pdf2svg_CFLAGS = $(CAIRO_CFLAGS) $(POPPLERGLIB_CFLAGS) 
pdf2svg_LDADD = $(CAIRO_LIBS) $(POPPLERGLIB_LIBS) 

MAINTAINERCLEANFILES = Makefile.in
