---
layout: default
---

# Software Overview

JasPer is a software toolkit for the handling of image data.
The software provides a means for representing images, and facilitates
the manipulation of image data, as well as the import/export of such data
in numerous formats (e.g., JPEG-2000 JP2, JPEG, PNM, BMP, Sun Rasterfile,
and PGX).
The import functionality supports the auto-detection (i.e., automatic
determination) of the image format, eliminating the need to explicitly
identify the format of coded input data.
A simple color management engine is also provided in order to allow the
accurate representation of color.
Partial support is included for the ICC color profile file format,
an industry standard for specifying color.

The JasPer software consists of a library and several application programs.
The code is written in the C programming language.
This language was chosen primarily due to the availability of C
development environments for most computing platforms when JasPer was
first developed, circa 1999.

Portability has been (and continues to be) a major consideration in the
development of JasPer.
For this reason, the software makes minimal assumptions about the compile-
and run-time environments.
The code should compile and run on any platform with a C language
implementation that is reasonably compliant with
[ISO 9899:1999](https://www.iso.org/standard/29237.html) (i.e., C99)
and has some limited support for
[ISO 9945](https://www.iso.org/standard/50516.html) (i.e., the POSIX C API).
For example, JasPer compiles and runs (without modification) under
Microsoft Windows and many variants of UNIX
(e.g., Linux, macOS, BSD, Solaris, AIX, HP-UX, and IRIX).

In order to provide JPEG support, JasPer relies on the popular IJG
JPEG library.
For graphics support, JasPer makes use of the OpenGL and GLUT libraries.
These particular libraries were chosen due to their availability on many
platforms, including Microsoft Windows and most flavors of UNIX, when
JasPer was first developed (circa 1999).

# Users of JasPer

This JasPer software has also been incorporated into numerous
other software projects (some commercial and some non-commercial).
Some projects known to use JasPer include:

 - [K Desktop Environment](http://www.kde.org)
 - [OpenCV](https://opencv.org)
 - [Kopete](https://kde.org/applications/en/internet/org.kde.kopete)
 - [Ghostscript](http://www.ghostscript.com)
 - [ImageMagick](http://www.imagemagick.org)
 - [Netpbm (as of Release 10.12)](http://sourceforge.net/projects/netpbm)
 - [XnView](http://www.xnview.com)
 - [Konvertor](http://www.logipole.com/indexe.html)
 - [Photoline 32](http://www.pl32.com)
 - [OpenSceneGraph Project](http://www.openscenegraph.org)
 - [Prophecy SDK](http://www.twilight3d.com)
 - [Meesoft's Image Analyzer](http://meesoft.logicnet.dk/Analyzer/index.htm)
 - [SlowView](http://www.slowview.at)
 - [AYTTM](http://ayttm.sourceforge.net)
 - [QJp2IO (JasPer wrapper for Qt)](http://www.dimin.net/software/sources.html)
 - [BorJas (JasPer wrapper for Borland C++ Builder)](http://www.dimin.net/software/sources.html)
 - [GeoJasper](http://www.dimin.net/software/utils.html)
 - [Labplot](http://labplot.sourceforge.net)
 - [GRIB](http://www.nco.ncep.noaa.gov/pmb/codes/GRIB2)
 - [GraphicsMagick](http://www.graphicsmagick.org)
 - [RAVL](http://ravl.sourceforge.net)
 - [RealWorld Icon Editor](http://www.rw-designer.com/3D_icon_editor.php)
 - [GYachI](http://gyachi.sf.net)
 - [libsecondlife](http://www.libsecondlife.org)
 - [MATLAB toolbox for JasPer](http://www.mathworks.com/matlabcentral/fileexchange/loadFile.do?objectId=2682&objectType=FILE)
 - [Ziproxy](http://ziproxy.sourceforge.net)
 - [FBI Improved (FIM)](https://savannah.nongnu.org/projects/fbi-improved)
 - [SAIL](https://github.com/HappySeaFox/sail)

# Historical Perspective

The JasPer software was initially developed in order to provide a free
reference implementation of the [ISO/IEC 15444-1](https://www.iso.org/standard/78321.html) (i.e., JPEG-2000 Part-1) codec.
JasPer was originally written by Michael Adams during his doctoral
studies at the University of British Columbia, Vancouver, Canada.
During this time, Michael became heavily involved in the JPEG-2000
standarization effort, serving as a Co-Editor of Part 5 of the
JPEG-2000 standard.
Work on JasPer began in September 1999, and in December 2000,
the software was first released in source code form to the general public.
The JasPer software has since been published in the JPEG-2000 Part-5
standard as a reference implementation of the JPEG-2000 Part-1 codec.

Since 2000 (circa JasPer version 0.016.0), JasPer has continued to evolve,
and in so doing, the motivations/objectives behind the development of
this software grew more ambitious.
Although originally conceived as simply a JPEG-2000 reference implementation,
JasPer has further evolved into a general-purpose toolkit for handling
image data that is useful in a wide variety of applications.

In September 2016, JasPer transitioned from using a private Subversion
repository to a public Git repository hosted by GitHub.
Since this time, the number of contributors to JasPer has grown.
In June 2020, the Git repository for JasPer, which was originally owned by
the GitHub user [mdadams](https://github.com/mdadams) was transferred to
the new [jasper-software](https://github.com/jasper-software) organization in
order to better facilitate collaborative efforts to develop and maintain
the software.

# Documentation

Some high level descriptions of JasPer can be found in:

 - M. D. Adams and R. K. Ward,
 "JasPer: A Portable Flexible Open-Source Software Tool Kit for
 Image Coding/Processing,"
 in Proc. of IEEE International Conference on Acoustics, Speech,
 and Signal Processing,
 Montreal, PQ, Canada,
 May 2004,
 vol. 5, pp. 241-244.
 [DOI: 10.1109/ICASSP.2004.1327092](http://dx.doi.org/10.1109/ICASSP.2004.1327092).
 [Preprint (in PDF format)](https://www.ece.uvic.ca/~mdadams/publications/icassp2004.pdf).

 - M. D. Adams and F. Kossentini,
 "JasPer: A Software-Based JPEG-2000 Codec Implementation,"
 in Proc. of IEEE International Conference on Image Processing,
 Vancouver, BC, Canada, Oct. 2000, vol. 2, pp. 53-56.
 (Part of the Special Session on JPEG 2000.)
 [DOI: 10.1109/ICIP.2000.899223](http://dx.doi.org/10.1109/ICIP.2000.899223).
 [Preprint (in PDF format)](https://www.ece.uvic.ca/~mdadams/publications/icip2000.pdf).

Some information on the JPEG-2000 standard can be found in:

 - M. D. Adams,
 The JPEG-2000 Still Image Compression Standard,
 ISO/IEC JTC 1/SC 29/WG 1 N 2412, Dec. 2002.
 [Paper in PDF format](https://www.ece.uvic.ca/~mdadams/publications/jpeg2000.pdf).
 This paper has also been published in
 PD 6777: 2003
 Guide to the practical implementation of JPEG 2000,
 British Standards Institute, UK.
