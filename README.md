## Academy Color Encoding System Developer Resources ##

The Academy Color Encoding System (ACES) is a set of components that facilitates a wide range of motion picture and television workflows while eliminating the ambiguity of legacy file formats.  The system is designed to support both all-digital and hybrid film-digital motion picture workflows.

The basic ACES components are:

* Color encoding and metric specifications, file format specifications, color
transformations, and an open source reference implementation 
* A set of reference images and calibration targets for film scanners and recorders 
* Documentation on the system and software tools can be found in ['aces-docs'](https://github.com/ampas/aces-docs)

This toolkit is intended to serve as a distribution mechanism for key components of the system, including the reference implementation transforms, reference images, and documentation.

### Package Contents ###
 
* [`formats/`](./formats) - ACES file formats code and examples
* [`images/`](./images) - "golden" images created using the reference implementation transforms
* [`transforms/`](./transforms) - ACES reference implementation transforms

### Changes from Previous Release ###

Though the "master" branch is 1.3, the current major version of ACES remains 1.  The 1.3 update adds transforms but does not change the look or modify the existing core transforms (beyond addressing reported bugs and/or inconsequential formatting/whitespace changes).

The dev branch always reflects the "bleeding edge" and indicates new features or updates that will ultimately be rolled into a future release version of ACES.  Improvements are staged on the dev branch for testing as they become available.

Included in ACES 1.3:

* New Features: 
    * Add gamut compression transform to assist with remapping problematic colorimetry into AP1
    * Update AMF schema with refinements by the AMF Implementation Working Group
    * Add CSC transforms for Sony Venice
* Bug fixes:
    * Remove clamp in ACES to ACEScg conversion transform
    * Correct typos in the TransformIDs of CSC files added in v1.2
* Other:
    * Relocate ACES documentation to its own repository
    * Rename `outputTransforms` directory to `outputTransform` (singular)
    * Add color primary subdirectories to `outputTransform` directory to be consistent with the `odt` directory
    * Update reference images

For a more detailed list of changes see the [CHANGELOG](./CHANGELOG.md) and the [commit history](https://github.com/ampas/aces-dev/commits/master).

#### Notes on the gamut compression transform ####
The gamut compression transform is the result of the work of the Gamut Mapping Working Group, to create a more robust solution to issues with out of gamut colors and the resulting artifacts (loss of texture, intensification of color fringes).  The delivered gamut compression transform performs well with wide gamut, high dynamic range, scene referred content and is robust and invertible. 

The gamut compression replaces the simple and less robust  `LMT.Academy.BlueLightArtifactFix.ctl`.  The ACES gamut compression transform has numerous advantages over the BlueLightArtifactFix LMT.  In particular, the gamut compression transform avoids changing colors that are well inside the destination gamut.  Only colors that are out of gamut or very near the gamut boundary are affected. 

As of this release, the Gamut Compression Implementation Working Group is working to provide recommendations and guidance to vendors to implement this transform directly their in products.

For those looking for more detail, the Gamut Mapping Working Group delivered [a report](https://paper.dropbox.com/doc/ACES-Gamut-Mapping-Architecture-VWG-Technical-Documentation-Deliverable--BJ2ttNh9kf5Ikep6Qm1Nxc1WAg-tZHiuOCj0RdYw8PPkrTam) to accompany the technical deliverable.

#### Notes on the updated AMF specification ####
ACES 1.2 included the release of the Academy Metadata Format (AMF) specification.  Since that time a working group has been formed to provide guidance to vendors on the implementation of AMF into products and end-user guidance on production usage. In the process of testing and working with the initial specification, the group suggested extensions to the AMF specification to make it more useful in meeting specific use cases. ACES 1.3 includes an updated AMF specification and associated schema that allow all transforms to now be referenced by UUID, relative file path, or TransformID.

### Versioning ###
 
The links to the current and all past versions of the ACES Developer Resources
can be found at [https://github.com/ampas/aces-dev/releases](https://github.com/ampas/aces-dev/releases).  

Source code is version controlled using the [git version control system](http://git-scm.com/) and hosted on GitHub at [https://github.com/ampas/aces-dev/](https://github.com/ampas/aces-dev/).

File names conform to the ACES System Versioning Specification.  Details can be found in the Academy Specification "S-2014-002 - Academy Color Encoding System - Versioning System" included in [`documents/`](./documents)

### Branch Structure ###

__Master Branch__
 
The current official release version of ACES can always be found at the HEAD of the master branch.  The master branch contains no intermediate commits and all commits on the master branch are tagged to represent a release of ACES.

__Dev Branch__
 
Intermediate commits between releases are staged on the dev branch.  Commits staged on the dev branch, but not yet merged into the master, should be considered as "planned for inclusion" in the next release version.  These represent the "bleeding edge" and development and, in turn, pull requests should be based against the dev branch. 

__Hotfixes Branch__

In some instances it may be necessary to create a hotfixes branch.  The hotfixes branch will include critical fixes for bugs found in a particular release.  Hotfixes should only be implemented by developers if the bug they are intending to correct is encountered in the course of production and is deemed to be a barrier to using a particular ACES release.  Hotfixes, once fully tested, will be merged into the dev branch, and ultimately the master.

## Prerequisites ##

### Color Transformation Language ###

Color Transformation Language (CTL) can be downloaded from
https://github.com/ampas/CTL

## License Terms for Academy Color Encoding System Components ##

Academy Color Encoding System (ACES) software and tools are provided by the
Academy under the following terms and conditions: A worldwide, royalty-free,
non-exclusive right to copy, modify, create derivatives, and use, in source and
binary forms, is hereby granted, subject to acceptance of this license.

Copyright 2019 Academy of Motion Picture Arts and Sciences (A.M.P.A.S.).
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
