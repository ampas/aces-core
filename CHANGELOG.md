**Version 1.0.1 (September 4, 2015):**
  * ACES technical documentation has been added as LaTeX source to facilitate tracking of document revisions.
  * Utility functions for making the OCIO config have been added.
  * Bug fixes:
      * "ACES to ACESproxy" transforms - A bug in piecewise function logic for lin_2_acesProxy() was fixed. 
  * Readability and robustness of some transforms have been improved with suggestions from users.
  * Various typos in transform comments and documentation have been corrected.
  * A broken link to the reference images has been corrected.
  * HDR ODT transforms have been modified
      * The lowest four coefficients for the HDR ODT tonescales have been adjusted in conjunction with a small offset to allow for obtaining a code value of 0.
      * A Rec.2020 version of the 1000 nit ODT has been added.
      * HDR transforms have had references to "PQ" changed to "ST2084".

**Version 1.0 (December 20, 2014):**
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
  * ACEScc (formerly ACESlog) and ACESproxy transforms have been updated
  * Miscellaneous code cleanups. Removal of unused code

  
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
  * Transforms including RRT
  * Test Images
  * ctlrender sample application for processing images through CTL transforms
  * ACES, ADX, and CTL documentation