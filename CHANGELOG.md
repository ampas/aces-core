**Version 0.7.1 (February 26, 2014):**
  * Bug fixes:
    * Corrects the value for a constant in the F65-F55 10-bit IDTs
    * Corrects the forward Rec2020 ODTs which were applying the inverse tonescale

**Version 0.7 (January 31, 2014):**
  * Updates RRT (forward and inverse):
    * Removal of rendering primaries to address a bug which can occur during exposure grading. Tone scale application is performed directly on ACES RGB.
    * Replacement of the red chroma scaling step with an alternate algorithm to modify reds. This algorithm darkens red colors instead of reducing chroma.
    * Addition of a "glow module", which provides a modest lightening and saturation boost in colored shadow regions.
  * Updates ODTs (forwards and inverses):
    * Removal of rendering primaries to address a bug which can occur during exposure grading. Tone scale application is performed directly on OCES RGB.
    * New math in the ODT tone scale application to preserve more highlight saturation. 
    * Device code value clamping added at 1.0, followed by a hue-restore step. 
    * Change in clipping behavior for the Rec709 and rgbMonitor ODTs to produce a closer visual approximation to image appearance on a digital cinema projector.
  * Major refactoring of the code to centralize functions common to more than one transform. This removes redundancy and makes the differences between various transforms easier to identify for implementers.  
  * Updates IDTs:
    * ARRI Alexa - Updates sensor-RGB-to-ACES matrices for nitpickers. ACES diff ~0.00x; LAB diff after RRT + rgbMonitor ODT ~0.0000x.
    * Sony - Updates the Sony IDTs. F35 ODTs updated to use the piecewise S-Log1 function. F65/F55 IDTs are now included for daylight and tungsten.
  * Fixes a typo in ACES-to-ACESproxy10 transform.

**Version 0.2.2 (October 15, 2013):**
  * Restores DCDM ODT with bug fixes
  * Adds a variant of the DCDM ODT that limits X'Y'Z' values to P3D60 gamut
  * Relabels "dcsim" ODTs as "d60sim"
  * Modifies highlight handling of "d60sim" ODTs to avoid chromaticity shifts in highlights
  * Refactors ODTs for consistency and improved readability
  * Fixes a bug in RRT that could occur when ACES triplets had a negative valued mean

**Version 0.2.1 (August 9, 2013):**
  * Temporarily removes DCDM ODT
  * Renames RGB monitor ODT transforms to clarify intended usage (dcsim)

**Version 0.2 (August 2, 2013):**
  * Updates RRT
  * Updates ODTs
  * Adds ODTs for display devices which conform to ITU-R BT.2020
  * ODTs renamed to better reflect their intended usage
  * Adds Inverse RRT and ODT transforms
  * Adds ACESlog and ACESproxy documentation, reference implementations, and images  
  * ctlrender removed - now part of CTL
  * Documents binaries removed and replaced with links
  * New distribution links for golden reference images

**Version 0.1.1 (June 25, 2012):**
  * Adds dw ratio preserving ODT tone curve
  * Fixes possible underflow conditions in RRT and ODT CTL transforms
  * Bug fix in ctlrender to avoid streaks in output images
  * Adds reverse RRT and RDT splines
  * Modifies RDT spline coefficients to tweak shadow reproduction

**Version 0.1 (March 1, 2012):**
  * Initial Release
  * Transforms including RRT
  * Test Images
  * ctlrender sample application for processing images through CTL transforms
  * ACES, ADX, and CTL documentation