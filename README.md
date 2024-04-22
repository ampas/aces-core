## Academy Color Encoding System Core Transforms ##

[![CLA assistant](https://cla-assistant.io/readme/badge/ampas/aces-dev)](https://cla-assistant.io/ampas/aces-dev)

The Academy Color Encoding System (ACES) is a set of components that facilitates a wide range of motion picture and television workflows while eliminating the ambiguity of legacy file formats.  The system is designed to support both all-digital and hybrid film-digital motion picture workflows.

The basic ACES components are:

* Color encoding and metric specifications, file format specifications, color
transformations, and an open source reference implementation 
    * This repository (`aces-dev`) will be renamed (`aces-core`) and houses the main rendering algorithms for ACES.
    * A set of preset Output Transforms to common outputs are tracked at [aces-output](https://github.com/ampas/aces-output)
    * Input Transforms are tracked at  [aces-input-and-colorspaces](https://github.com/ampas/aces-input-and-colorspaces)
    * Look Transforms are collected in [aces-look](https://github.com/ampas/aces-look)
    * AMF schema and example files can be found at [aces-amf](https://github.com/ampas/aces-amf)
    * Documentation is written in markdown and tracked at [aces-docs](https://github.com/ampas/aces-docs). It is published using mkdocs to [ACEScentral](docs.acescentral.com).

* Corresponding reference images
* Documentation on the system and software tools can be found in ['aces-docs'](https://github.com/ampas/aces-docs)

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
