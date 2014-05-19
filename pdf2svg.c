// Copyright (C) 2007-2013 David Barton (davebarton@cityinthesky.co.uk)
// <http://www.cityinthesky.co.uk/>

// Copyright (C) 2007 Matthew Flaschen (matthew.flaschen@gatech.edu)
// Updated to allow conversion of all pages at once.

//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

// Definitely works with Cairo v1.2.6 and Poppler 0.5.4

#include <assert.h>
#include <stdlib.h>
#include <glib.h>
#include <poppler.h>
#include <poppler-document.h>
#include <poppler-page.h>
#include <cairo.h>
#include <cairo-svg.h>
#include <stdio.h>
#include <string.h>

// Begin theft from ePDFview (Copyright (C) 2006 Emma's Software) under the GPL
gchar *getAbsoluteFileName(const gchar *fileName)
{
	gchar *absoluteFileName = NULL;
	if (g_path_is_absolute(fileName)) {
		absoluteFileName = g_strdup(fileName);
	}
	else {
		gchar *currentDir = g_get_current_dir();
		absoluteFileName = g_build_filename(currentDir, fileName, NULL);
		g_free(currentDir);
	}
	return absoluteFileName;
}
// End theft from ePDFview


int convertPage(PopplerPage *page, const char* svgFilename)
{
	// Poppler stuff 
	double width, height;

	// Cairo stuff
	cairo_surface_t *surface;
	cairo_t *drawcontext;

	if (page == NULL) {
		fprintf(stderr, "Page does not exist\n");
		return -1;
	}
	poppler_page_get_size (page, &width, &height);

	// Open the SVG file
	surface = cairo_svg_surface_create(svgFilename, width, height);
	drawcontext = cairo_create(surface);

	// Render the PDF file into the SVG file
	poppler_page_render_for_printing(page, drawcontext);
	cairo_show_page(drawcontext);

	// Close the SVG file
	cairo_destroy(drawcontext);
	cairo_surface_destroy(surface);

	// Close the PDF file
	g_object_unref(page);
	
	return 0;     
}

int main(int argn, char *args[])
{
	// Poppler stuff
	PopplerDocument *pdffile;
	PopplerPage *page;

	// Initialise the GType library
	g_type_init ();

	// Get command line arguments
	if ((argn < 3)||(argn > 4)) {
		printf("Usage: pdf2svg <in file.pdf> <out file.svg> [<page no>]\n");
		return -2;
	}
	gchar *absoluteFileName = getAbsoluteFileName(args[1]);
	gchar *filename_uri = g_filename_to_uri(absoluteFileName, NULL, NULL);
	gchar *pageLabel = NULL;

	char* svgFilename = args[2];

	g_free(absoluteFileName);
	if (argn == 4) {
		// Get page number
		pageLabel = g_strdup(args[3]);
	}

	// Open the PDF file
	pdffile = poppler_document_new_from_file(filename_uri, NULL, NULL);
	g_free(filename_uri);
	if (pdffile == NULL) {
		fprintf(stderr, "Unable to open file\n");
		return -3;
	}

	int conversionErrors = 0;
	// Get the page

	if(pageLabel == NULL) {
		page = poppler_document_get_page(pdffile, 0);
		conversionErrors = convertPage(page, svgFilename);
	}
	else {
		if(strcmp(pageLabel, "all") == 0) {
			int curError;
			int pageCount = poppler_document_get_n_pages(pdffile);

			if(pageCount > 9999999) {
				fprintf(stderr, "Too many pages (>9,999,999)\n");
				return -5;
			}

			size_t svgFilenameBufLen = strlen(svgFilename) + 1;
			char *svgFilenameBuffer = (char*)malloc(svgFilenameBufLen);
			assert(svgFilenameBuffer != NULL);

			int pageInd;
			for(pageInd = 0; pageInd < pageCount; pageInd++) {
				while (1) {
					size_t _wr_len = snprintf(svgFilenameBuffer, svgFilenameBufLen, svgFilename, pageInd + 1);
					if (_wr_len >= svgFilenameBufLen) {
						svgFilenameBufLen = _wr_len + 1;
						svgFilenameBuffer = (char*)realloc(svgFilenameBuffer, svgFilenameBufLen);
						assert(svgFilenameBuffer != NULL);
						continue;
					}
					break;
				}

				page = poppler_document_get_page(pdffile, pageInd);
				curError = convertPage(page, svgFilenameBuffer);
				if(curError != 0)
					conversionErrors = -1;
			}
			free(svgFilenameBuffer);
		}
		else {
			page = poppler_document_get_page_by_label(pdffile, pageLabel);
			conversionErrors = convertPage(page, svgFilename);
			g_free(pageLabel);
		}
	}

	g_object_unref(pdffile);

	if(conversionErrors != 0) {
		return -4;
	}
	else {
		return 0;
	}

}
