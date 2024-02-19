---
title: 'JasPer: A Portable Flexible Software Toolkit for Image Coding/Processing'
tags:
  - C
  - JPEG-2000
  - image representation
  - image coding
  - image processing
authors:
  - name: Michael D. Adams
    orcid: 0000-0001-6163-7062
    corresponding: true
    affiliation: 1
affiliations:
 - name: Department of Electrical and Computer Engineering, University of Victoria, Victoria, British Columbia, Canada
   index: 1
date: 05 February 2024
bibliography: paper.bib
---

# Summary

The use of digital imagery by computer software is pervasive.
Therefore, many application programs and libraries have a need to represent
  images as well as encode and decode image data in a variety of formats.
Consequently, a very strong need exists for software packages that can
  provide such functionality.
In what follows, we present one such software package, known as JasPer.

JasPer [@JasPer_project_web_site] is a portable flexible open-source
  software toolkit for handling image data.
It consists of a library and several application programs that use this
  library.
The code is written in the C programming language [@iso9899-2011].
The JasPer library provides a means to represent and manipulate images,
  as well as encode and decode image data in a variety of formats.
Native support is provided for several commonly-used image formats, including:

  - JPEG-2000 file (JP2) and codestream (JPC) [@iso15444p1]
  - Portable Graymap/Pixmap (PNM) [@PNM_Format]
  - Microsoft Bitmap (BMP) [@Microsoft_BMP_Format]
  - Sun Rasterfile (RAS) [@Sun_Raster_Format]
  - JPEG-2000 Verification Model (PGX) [@iso15444p4]

In addition, a few other image formats are supported non-natively
(i.e., with some assistance from external libraries), including
  JPEG [@iso10918p1] and
  HEIC [@iso23008p12].
The JasPer library supports multithreaded applications.
It is also highly configurable, allowing a number of key
  parameters and callbacks to be specified by the library user
  (e.g., for custom memory allocation and logging).
The library is also extensible, providing an application programming interface
  (API) whereby additional user-defined codecs can be employed with the
  library.
The JasPer software provides several application programs, including:

  - jasper, an image transcoder program for converting between image formats;
  - imginfo, a program for querying the properties of an image, such as:
    width, height, number of components, and the number of bits per sample;
  - imgcmp, a program for comparing two images using various distance metrics,
    such as peak absolute error (PAE), mean absolute error (MAE), and peak
    signal to noise ratio (PSNR); and
  - jiv, an image viewer.

The JasPer software has detailed documentation in the form of a
  (Doxygen-generated) manual,
  which includes specifications of all of the key
  APIs provided by the library
  as well as descriptions of the command-line interfaces used by the
  application programs.
The online manual for the most recent release of JasPer (as well as numerous
  past releases) can be found at [@JasPer_manual_web_site].

Portability has been a primary consideration in the development of JasPer.
This software was designed to make minimal assumptions about the
  compile- and run-time environments, and should compile and run
  on any platform with a C language implementation that is reasonably
  compliant with the C11 standard [@iso9899-2011] and has some
  limited support for the POSIX C API [@iso9945-2009].
The JasPer software is known to work on many platforms, including
  Unix/Linux, Windows, macOS, and
  WebAssembly (WA) runtimes with
  WA System-Interface (WASI)
  support [@WebAssembly_Core_Specification]
  [@WASI_web_site].

The JasPer software is available under an open-source license that is
  based on the well-known BSD license.
The license used by JasPer is listed as an officially approved open-source
  license by the Fedora Project [@Fedora_accepted_licenses] and
  OpenSuse [@OpenSUSE_accepted_licenses].
Moreover, JasPer is used in many open-source projects, and
  has been packaged for many Linux distributions
  (e.g., Fedora, openSUSE, Gentoo, Arch, Red Hat Enterprise Linux, SUSE Linux Enterprise Server, and Amazon Linux),
  numerous BSD variants (e.g., FreeBSD, NetBSD, and OpenBSD),
  NixOS,
  Homebrew [@homebrew_jasper_formula] (for macOS),
  and
  Conan [@conan_jasper_recipe].

# A Brief Historical Perspective

The JasPer software has long history, spanning over two decades.
The author of this paper is also the primary author of JasPer and has been
  responsible for all major feature development for this software
  over the years.
The JasPer software was initially developed in order to provide
  an open-source implementation of the JPEG-2000 Part-1 codec as well
  as a portable and extensible software framework for image representation,
  encoding, and decoding.
Work on the software began in September 1999, and in December 2000,
  the software was first released in source code form to the general public.
In approximately 2002, the JasPer software was published in the JPEG-2000
  Part-5 standard [@iso15444p5] as an official reference implementation
  of the JPEG-2000 Part-1 codec [@iso15444p1].
The very early versions of JasPer were briefly discussed in the two
  conference papers [@Adams_2000c] and [@Adams_2004a],
  which were co-authored by the author
  of this paper.
Since that time, the JasPer software has continued to evolve with many major
  improvements being made over the years, such as adding
  support for multithreading, memory-usage limiting, and greater
  configurability.
In September 2016, JasPer development transitioned from using a private
  local repository to a public Git repository hosted by GitHub
  [@jasper_github_page].
Since its initial creation, this public Git repository has accumulated
  over 1500 commits.
In recent years, the JasPer project has placed a much greater emphasis on
  security, with many code changes having been made to improve the
  resilience of the software to attacks by bad actors and a high
  priority being placed on timely bug fixes for reported security
  vulnerabilities.

# Statement of Need

Digital imagery is used in a great many research areas, including
  image processing,
  computer vision,
  digital forensics,
  AI image synthesis,
  geographic information systems,
  remote sensing,
  biology,
  and
  medicine,
  to name but a few.
For this reason, the functionality provided by JasPer is of
  potential interest to a great many researchers.

To date, JasPer has been used in many
  research projects
  and has been cited in many papers in the research literature.
The two conferences papers [@Adams_2000c] and [@Adams_2004a] (mentioned
  earlier) that briefly discuss the very early versions of JasPer
  (circa 2000--2004) are quite well cited.
For example, as of the time of this writing (February 2024),
  Google Scholar indicates that [@Adams_2000c] and [@Adams_2004a] have
  306 and 66 citations, respectively.
Since digital imagery is used in many research disciplines, the uses
  of JasPer for research purposes is quite varied.
For example, JasPer has been used for
  research in areas related to
  JPEG 2000 [@Yeung_2005a; @Menichelli_2011a],
  image coding [@Rubino_2019a; @Verhack_2015a],
  watermarking [@Nasir_2008a],
  steganography [@SaeTang_2014a],
  image processing [@Park_2011a],
  wavelet/subband transforms [@Chung_2022a; @Wong2007a],
  biometrics [@HungTsai_2015a],
  microscopy [@Corredor_2015a; @Gomez_2007a],
  medical imaging [@FalconRuiz_2010a],
  and
  deep space communications [@Bisio_2010a].
JasPer is also used by other software packages that are often used
  for research purposes, such as the Open Computer Vision Library (OpenCV)
  [@OpenCV_web_site].
All of the above demonstrates that JasPer has been clearly beneficial
  to the research community.

Since native JPEG-2000 support is often one of the features of JasPer that
  motivates its use, we would like to note, in passing, that another
  open-source implementation of the JPEG-2000 codec can be found in the
  OpenJPEG software [@OpenJPEG_web_site].
At present, however, the OpenJPEG software is not as well maintained as
  JasPer, with OpenJPEG having a very large number of unresolved bugs,
  some of which are associated with security vulnerabilities that
  are quite old.
Moreover, at the time of this writing, the last official release of
  OpenJPEG (namely, 2.5.0) on GitHub is dated May 13, 2022, which is
  almost two years ago.

# Acknowledgements

The author would like to thank Michael Vetter, Max Kellermann, and the many
  other people who have contributed in some way to the JasPer project over
  the years (e.g., by filing bug reports, submitting bug fixes or
  pull requests, or packaging JasPer for use in various environments).

# References
