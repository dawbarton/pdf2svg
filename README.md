pdf2svg
=======

A simple PDF to SVG converter using the Poppler and Cairo libraries.

For Windows binaries see <https://github.com/jalios/pdf2svg-windows>.

For Linux binaries, see your package manager (e.g., "yum install pdf2svg" or "apt-get install pdf2svg").

Prerequisites:

* poppler [http://poppler.freedesktop.org/](http://poppler.freedesktop.org/)
    - tested with v0.5.4 but other versions may work as well
* cairo [http://cairographics.org/](http://cairographics.org/)
    - tested with v1.2.6 and v1.4.10 but other versions may work as well


To make and install pdf2svg:

1) Download pdf2svg-0.2.2.tar.gz and unpack
2) Change to the pdf2svg-0.2.2 directory
3) Do the normal ./configure && make && make install


Usage:

pdf2svg &lt;input.pdf&gt; &lt;output.svg&gt; \[&lt;page no of pdf or "all"&gt;\]


Copyright (C) 2007-2013 David Barton (davebarton@cityinthesky.co.uk)
[http://www.cityinthesky.co.uk/](http://www.cityinthesky.co.uk/)

Copyright (C) 2007 Matthew Flaschen (matthew.flaschen@gatech.edu)
Updated to allow conversion of all pages at once.

Copyright (C) 2008 Ed Grace
Added GNU Autotools commands.
