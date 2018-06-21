## Academy Color Encoding System Developer Resources ##

The Academy Color Encoding System (ACES) is a set of components that facilitates a wide range of motion picture and television workflows while eliminating the ambiguity of legacy file formats.  The system is designed to support both all-digital and hybrid film-digital motion picture workflows.

The basic ACES components are:

* Color encoding and metric specifications, file format specifications, color
transformations, and an open source reference implementation 
* A set of reference images and calibration targets for film scanners and recorders 
* Documentation on the system and software tools

This toolkit is intended to serve as a distribution mechanism for key components of the system, including the reference implementation transforms, reference images, and documentation.

### Package Contents ###
 
* [`documents/`](./documents) – ACES-related documents 
* [`images/`](./images) - "golden" images created using the reference implementation transforms
* [`transforms/`](./transforms) - ACES reference implementation transforms

### Changes from Previous Release ###

Though the "master" branch is 1.1, the current major version of ACES remains 1.  This means the 1.1 update adds a number of transforms but does not change the look or modify the existing core transforms (beyond addressing reported bugs and/or inconsequential formatting/whitespace changes).

As always, you should check the hotfixes and dev branches for the latest bug fixes and new features that will ultimately be rolled into a future version of ACES.  Improvements will continue to be staged on the dev branch for testing as they become available.

Included in ACES 1.1:

* New Features: 
    * Add P3 ODTs:
        * P3D65 (and inverse)
        * P3D65 "D60 simulation" (i.e. D60 adapted white point) (and inverse)
        * P3DCI "D65 simulation" (i.e. D65 adapted white point) (and inverse)
        * P3D65 limited to Rec.709 (inverse not required)
    * Add Rec.2020 ODTs:
        * Rec.2020 limited to Rec.709 (inverse not required)
        * Rec.2020 limited to P3D65 (inverse not required)
    * Add DCDM ODT:
        * DCDM with D65 adapted white point and limited to P3D65 (and inverse)
    * Add new ACESlib files:
        * SSTS: code for the Single Stage Tone Scale used in HDR Output Transforms
        * OutputTransforms: beginning of modules needed for parameterizing Output Transforms
    * Add HDR Output Transforms (RRT+ODT):
        * P3D65 (108 cd/m^2) ST.2084 - designed for use in Dolby Cinema (and inverse)
        * Rec.2020 (1000 cd/m^2) ST.2084 (and inverse)
        * Rec.2020 (2000 cd/m^2) ST.2084 (and inverse)
        * Rec.2020 (4000 cd/m^2) ST.2084 (and inverse)
        * Rec.2020 (1000 cd/m^2) HLG (and inverse)
    * Add new subfunctions to existing ACESlib files
    * Add new ACESutil functions
    * Remove HDR ODTs (and inverses)
    * Rename some existing transforms for clarity:
        * Rename `DCDM_P3D60` to `DCDM_P3D60limited`
        * Rename `P3DCI` to `P3DCI_D60sim`
        * Rename `RGBmonitor` to `sRGB`
    * Add LMT that can help correct bright blue light clipping or hue shifts
    * Add new reference images for new transforms
    * Add documentation:
        * TB-2018-001 - Derivation of the ACES White Point CIE Chromaticity Coordinates
* Bug Fixes:
    * Arri IDT - Improve linearization of LogC data
* Other:
    * Miscellaneous white space, line wrap, and typo fixes
    * Miscellaneous README and CTL comment updates
    * Miscellaneous LaTeX documentation typo and code fixes
    * Update ACEStranformIDs where appropriate
    * Update README and CHANGELOG

For a more detailed list of changes see the [CHANGELOG](./CHANGELOG.md) and in the [commit history](https://github.com/ampas/aces-dev/commits/master).

#### Notes on New ODTs ####

A series of new standard dynamic range (SDR) ODTs are included in this release.  These ODTs were added at the request of the ACES Community based on their production needs.  Examples include: P3 ODTs for devices with a D65 calibration white point, simulation of a D65 white point on a P3 device with a DCI calibration white point, and limiting of output image colorimetry to Rec.709 when using a P3D65 calibrated device.  Additional limiting ODTs were also added for Rec.2020 to restrict the image colorimetry to Rec.709 and P3.  A DCDM ODT with limiting to P3D65 were added to compliment the existing DCDM ODT with limiting to P3D60.  These transforms provide support for additional use cases not included in previous ACES releases.

#### Notes on New HDR Output Transforms ####

ACES 1.1 also includes the first release of a series of Output Transforms that combine the RRT and an ODT into a single transform.  The new Output Transforms replace the previous HDR ODTs.  The new Output Transforms are based on a unified, parametric output function.  These individual Output Transforms pass a series of parameters to the underlying output function to improve the consistency of the image processing operations.  Examples of the parameters that that are specified in the Output Transforms include: display primaries, display white point, display max luminance, display min luminance, luminance reproduction of mid-gray, limiting primaries (if any), surround, display EOTF, etc.  In the future, this will make it trivial to generate Output Transforms for non-standard devices.  

Output Transforms using the underlying parametric output function are only provided for HDR devices in dark surround environments at this time, although the transforms are suitable for use in both dark and dim surround environments (they’ve been used this way on several major motion pictures with a corresponding “trim pass” to adjust contrast and saturation). Modifying the surround parameter inside the individual HDR Output Transforms has no effect when the EOTF is set to either ST-2084(PQ) or HLG.  A functional dim surround compensation for HDR devices will be addressed in a future ACES release.  
   
#### Notes on rename of some existing transforms ####
The CTL file names and values of <ACESuserName> of some of the existing ACES transforms have been modified for clarity.  In each case, the output code values from the transform remains the same.  Academy S-2014-002 states “Any transform updates that do not change the output of that transform shall not require the Transform Identifier to be incremented – e.g. whitespace changes, modifications to code comments, etc.”  For this reason, the values <ACEStransformID> have not been changed.  Implementors should take care to update their user interfaces to display the new values of <ACESuserName> where applicable and to alert end-users of the name changes in their product documentation.

### Versioning ###
 
The links to the current and all past versions of the ACES Developer Resources
can be found at [https://github.com/ampas/aces-dev/releases](https://github.com/ampas/aces-dev/releases).  

Source code is version controlled using the [git version control system](http://git-scm.com/) and hosted on GitHub at [https://github.com/ampas/aces-dev/](https://github.com/ampas/aces-dev/).

Individual files now conform to the ACES System Versioning Specification.  Details can be found in the Academy Specification "S-2014-002 - Academy Color Encoding System - Versioning System" included in [`documents/`](./documents)

### Branch Structure ###

__Master Branch__
 
The current release version of ACES can always be found at the HEAD of the master branch.  The master branch contains no intermediate commits and all commits on the master branch are tagged to represent a release of ACES.

__Dev Branch__
 
Intermediate commits between releases will be staged on the dev branch.  Commits staged on the dev branch, but not yet merged into the master, should be considered as "planned for inclusion" in the next release version.  Commits on the dev branch will ultimately be merged into the master branch as part of a future release.

__Hotfixes Branch__

In some instances it may be necessary to create a hotfixes branch.  The hotfixes branch will include important, but not fully tested, fixes for bugs found in a particular release.  Hotfixes should only be implemented by developers if the bug they are intending to correct is encountered in the course of production and is deemed to be a barrier to using a particular ACES release.  Hotfixes, once fully tested, will be merged into the dev branch, and ultimately the master.

## Prerequisites ##

### Color Transformation Language ###

Color Transformation Language (CTL) can be downloaded from
https://github.com/ampas/CTL

## License Terms for Academy Color Encoding System Components ##

Academy Color Encoding System (ACES) software and tools are provided by the
Academy under the following terms and conditions: A worldwide, royalty-free,
non-exclusive right to copy, modify, create derivatives, and use, in source and
binary forms, is hereby granted, subject to acceptance of this license.

Copyright 2018 Academy of Motion Picture Arts and Sciences (A.M.P.A.S.).
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
