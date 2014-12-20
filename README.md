## Academy Color Encoding System Developer Resources ##

The Academy Color Encoding System (ACES) is a set of components that facilitates
a wide range of motion picture workflows while eliminating the ambiguity of
legacy file formats. The system is designed to support both all-digital and
hybrid film-digital motion picture workflows.

The basic ACES components are:

* Color encoding and metric specifications, file format specifications, color
transformations, and an open source reference implementation. 
* A set of reference images and calibration targets for film scanners and
recorders 
* Documentation on the system and software tools

This toolkit is intended to serve as a distribution mechanism for key components
of the system, including the reference implementation transforms, reference
images, and documentation.

### Package Contents ###
 
* [`documents/`](./documents) – ACES-related documents 
* [`images/`](./images) - "golden" images created using the reference implementation transforms
* [`transforms/`](./transforms) - ACES reference implementation transforms

### Changes from Previous Releases ###

* Additional transforms, encodings, documents, and reference images are included as part of the ACES Version 1.0 release. Please carefully review the ACES Version 1.0 documentation package for details on new features and enhancements for ACES Version 1.0
* Filenames have been updated to conform to the ACES System Versioning Specification
* RRT     
    * New set of rendering primaries have been introduced to improve gradeability and vectorscope behavior.  The new primaries, known as AP1, are near the spectrum locus but exceed anticipated device gamuts, including ITU-R BT.2020 at a range of white points.  
    * The global desaturation is now applied in RGB space prior to the RRT tone scale.  This was done to improve the overall look of the images based on end-user feedback.
    
    * The red modifier and glow module variables have been modified.  This was done to improve the overall look of the images based on end-user feedback. 
    * A clip of negative values has been added prior to the application of the 3x3 matrix that converts ACES to the rendering primaries. This is added to avoid an error that can occur with negative and saturated ACES values turning positive and saturated.
    
    * The RRT tone scale has been modified to address end-user concerns that the default rendering in v0.7.1 unnecessarily crushed shadow detail. 
    
    * The output luminance of an 18% scene reflector was moved from 5.0 nits to 4.8 nits to slightly darken the overall image in response to end-user feedback.     
    * The hue restore function has been removed to improve grading behavior and address rare instances where image noise could be enhanced.
    * The RRT tone scale has been modified to allow for the use of b-splines in the new HDR ODTs.
* ODTs
    * New set of rendering primaries have been introduced to improve gradeability and vectorscope behavior.  The new primaries, known as AP1, are near the spectrum locus but exceed anticipated device gamuts, including ITU-R BT.2020 at a range of white points.
    
    * The ODT tone scale has been modified to address end-user concerns that the default rendering in v0.7.1 unnecessarily crushed shadow detail.
    
    * The hue restore and smart-clip functions have been removed to improve grading behavior and address rare instances where image noise could be enhanced.
    * ODT tone scales have been modified to allow the ability to achieve device black on-set and more quickly in the DI environment.
    * Rec.709, Rec.2020, and rgbMonitor ODTs supporting dim surround environments have been added
    * Rec.709 ODTs now have a runtime flag for full range or legal range output. The default is full range.
* ACEScc (formerly ACESlog) and ACESproxy tranforms have been updated
* Miscellaneous code cleanups. Removal of unused code

For a detailed list of changes see the [CHANGELOG](./CHANGELOG.md) and in the [commit history](https://github.com/ampas/aces-dev/commits/master).

### Versioning ###
 
The links to the current and all past versions of the ACES Developer Resources
can be found at [https://github.com/ampas/aces-dev/releases](https://github.com/ampas/aces-dev/releases).  

Source code is version controlled using the [git version control system](http://git-scm.com/) and hosted on Github at [https://github.com/ampas/aces-dev/](https://github.com/ampas/aces-dev/).

Individual files now conform to the ACES System Versioning Specification.  Details can be found in the Academy Specification "S-2014-002 - Academy Color Encoding System - Versioning System" included in [`documents/`](./documents)

### Branch Structure ###

__Master Branch__
 
The current release version of ACES can always be found at the HEAD of the
master branch.  Previous release versions are tagged and are also commits on the
master.  The master branch contains no intermediate commits so all commits on
the master branch are tagged and represent a release of ACES.

__Dev Branch__
 
All commits between releases will be on the dev branch. Commits on the dev
branch will be included in the next release version. Commits staged on the dev
branch, but not yet merged into the master, should be considered non-critical. 
All intermediate commits for a release will be staged on the dev branch before
being merged into the master and tagged.

__Hotfixes Branch__
 
In some cases it may be necessary to create a hotfixes branch.  The hotfixes
branch will include important, but not fully tested, fixes for bugs found in a
particular release.
Hotfixes should only be implemented by developers if the bug they are intended
to correct is encountered in the course of production and is deemed to be a
barrier to using a particular ACES release.  Hotfixes, once fully tested, will
be merged into dev branch, and ultimately the master.    

## Prerequisites ##

### Color Transformation Language ###

Color Transformation Language (CTL) can be downloaded from
https://github.com/ampas/CTL

## License Terms for Academy Color Encoding System Components ##

Academy Color Encoding System (ACES) software and tools are provided by the
Academy under the following terms and conditions: A worldwide, royalty-free,
non-exclusive right to copy, modify, create derivatives, and use, in source and
binary forms, is hereby granted, subject to acceptance of this license.

Copyright 2014 Academy of Motion Picture Arts and Sciences (A.M.P.A.S.).
Portions contributed by others as indicated. All rights reserved.

Performance of any of the aforementioned acts indicates acceptance to be bound
by the following terms and conditions:

* Copies of source code, in whole or in part, must retain the above copyright
notice, this list of conditions and the Disclaimer of Warranty.

* Use in binary form must retain the above copyright notice, this list of
conditions and the Disclaimer of Warranty in the documentation and/or other
materials provided with the distribution.

* Nothing in this license shall be deemed to grant any rights to trademarks,
copyrights, patents, trade secrets or any other intellectual property of
A.M.P.A.S. or any contributors, except as expressly stated herein.

* Neither the name "A.M.P.A.S." nor the name of any other contributors to this
software may be used to endorse or promote products derivative of or based on
this software without express prior written permission of A.M.P.A.S. or the
contributors, as appropriate.

This license shall be construed pursuant to the laws of the State of
California, and any disputes related thereto shall be subject to the
jurisdiction of the courts therein.

Disclaimer of Warranty: THIS SOFTWARE IS PROVIDED BY A.M.P.A.S. AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND
NON-INFRINGEMENT ARE DISCLAIMED. IN NO EVENT SHALL A.M.P.A.S., OR ANY
CONTRIBUTORS OR DISTRIBUTORS, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, RESITUTIONARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

WITHOUT LIMITING THE GENERALITY OF THE FOREGOING, THE ACADEMY SPECIFICALLY
DISCLAIMS ANY REPRESENTATIONS OR WARRANTIES WHATSOEVER RELATED TO PATENT OR
OTHER INTELLECTUAL PROPERTY RIGHTS IN THE ACADEMY COLOR ENCODING SYSTEM, OR
APPLICATIONS THEREOF, HELD BY PARTIES OTHER THAN A.M.P.A.S.,WHETHER DISCLOSED OR
UNDISCLOSED.
