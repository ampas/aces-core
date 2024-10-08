**Version 2.0 Developer Release 3 (Sept 2, 2024):**
* Bugs
  * Change lower hull gamma approximation from a constant value to a value determined using the log of the peak luminance. For SDR outputs, the value reduces to the former value but this change minimimizes some clipping artifacts that could occur at edge values at higher luminance outputs.s
* Enhancements / simplifications
  * Replace calculation method for Mnorm used in the chroma compress function and remove REACH_GAMUT_TABLE
  * Make clamp to peak luminance active on forward and inverse direction (previously only active in forward direction) - helps avoid errors on inverse when users send in values outside the invertible range
  * Add extra clamp to PQ and HLG (derived from PQ 1000) EOTF options to protect against rare reintroduction of negative values. Though data is clamped from [0-peakLuminance] in a prior step, occasionally very small negative components can be reintroduced from precision errors during the XYZ to display primary matrix. Therefore, negative values are clamped as an extra protective step to avoid NaN errors from negative values which are undefined in the base PQ encoding.

**Version 2.0 Developer Release 2 (August 19, 2024):**
* Bugs
  * Correct a mistyped value used in pre-calculation of r_hit in the tone scale function init
  * Add a clamp to range [0 - peakLuminance] and remove explicit clamp in inverse EOTF function
* Enhancements / simplifications
  * Set upper limit clamp value in AP1 clamping step to a value equal to 3 stops (8x) the minimum value required to reach maximum output from the tone scale function
  * Update table generation and lookup code to assure that hue values falling in the "wrap-around" hue region are handled correctly
    * Add two extra table entries in gamut cusp and upper gamma hull approximation tables that are duplicates of the first and last entries
    * Update indexing and lookup functions to expect a baseIndex offset to maintain proper indexing where 360 entries are assumed
  * Add a bool to determine whether white scaling should be applied - This allows the user to control when the white is or isn't applied, and is clearer and more robust than relying on a mismatch in the white point chromaticities. For example, a DCDM P3D65 ODT already has white point fitting handled by a 48/52.37 scale factor, so wouldn't need that additional white scaling that would otherwise be applied because the encoding white and limiting white would differ.
  * Remove unused smoothJ value
  * Simply compressFuncParams and compressPowerP function 
    * Unused values in the 4-element compressionFuncParams were removed, retaining the one value that is used and renaming it to compressionThreshold to better describe what it controls
    * The compressionFuncParams related to power equaled 1, so the compressPowerP function was simplified where many of the pow() functions could be solved out when power=1
  * Rename REACH_CUSP_TABLE as REACHM_TABLE, because it represents the M value for the reach gamut at limitJmax. Also reduce REACHM_TABLE to only 1 column (M) since M is the only value that needs pre-computation (i.e. J is constant and always equal to limitJmax and hue is uniformly spaced and corresponds to row index)
  * Refactor the white scaling step as a discrete operator before display encoding on forward (plus a clamp) and immediately after display decoding on inverse
  * Reorder the enum values for EOTF encoding/decoding to be less haphazard
  * Remove unused parameters from display encoding/decoding function
  * Remove unused functions for applying surround parameter - The implementation was unvalidated and would need to be changed anyway if implemented in a future release, so the inactive code and supporting functions were removed to avoid any confusion for implementers.
  * Other minor refactorings of code for improved readability
* Other ACES repos
  * Output Transforms
    * Update all TransformIDs to be more verbosely defined from transform parameter settings
    * Change default list of transforms
      * Add:
        * HLG 1000 nit
        * DCDM 48 nit and 300 nit
        * Rec2020 500 nit
        * D60 limited / sim versions of all provided transforms, organized in a separate d60 directory
      * Remove: 
        * Rec2020 100 nit
        * P3-DCI
  * Input and Color Space Conversion Transforms
    * Set name space in Panasonic IDT to Panasonic
    * Update Sony Venice TransformIDs to be more consistent
    * Add a few missing transforms to provide to/from conversion to provided inputs 
    * Add "Unity" transform



**Version 2.0 Developer Release 1 (April 19, 2024):**
* Reorganization of code:
    * This repository (`aces-dev`) will be renamed (`aces-core`) and houses the main rendering algorithms for ACES.
    * Output Transforms moved to [aces-output](https://github.com/ampas/aces-output)
    * Input Transforms moved to [aces-input-and-colorspaces](https://github.com/ampas/aces-input-and-colorspaces)
    * Look Transforms moved to [aces-look](https://github.com/ampas/aces-look)
    * AMF schema and example files moved to [aces-amf](https://github.com/ampas/aces-amf)
    * Documentation tracked at [aces-docs](https://github.com/ampas/aces-docs) and published using mkdocs to [ACEScentral](docs.acescentral.com)
* All TransformIDs have been conformed to the v2 TransformID specification
* New core rendering algorithm:
    * New tonescale function with a lower default contrast and adaptability to produce output for any peak luminance between 100-10000 nits.
    * A hue-preserving rendering transform, applying luminance mapping mostly independent of color adjustments.
    * Gamut mapping to mostly avoid undesirable clipping but still allow for reaching the edges of the display gamut volume.
    * Invertibility up to at least P3 gamut
* Reference images have been moved to be tied to their respective transform repositories



**Version 1.3 (April 30, 2021):**

* New Features: 
    * Add gamut compression transform to assist with remapping problematic colorimetry into AP1
    * Update AMF schema to with refinements by the AMF Implementation Working Group:
        * Update all `aces:transformType`s to have choice of `aces:uuid`, `aces:file`, or `aces:transformId`
        * Modify `outputTransformType` to no longer be an extension of `transformType`
        * Update choice occurences to disallow the possibility of empty transform tags
    * Add CSC transforms for Sony Venice
* Bug fixes:
    * Remove clamp in ACES to ACEScg conversion transform
    * Correct typos in the TransformIDs of CSC files added in v1.2
* Other:
    * Relocate ACES documentation to its own repository
    * Rename `outputTransforms` directory to `outputTransform` (singular)
    * Add color primary subdirectories to `outputTransform` directory to be consistent with the `odt` directory
    * Update reference images:
        * Add images to accompany new gamut compression transform
        * Add images to accompany Sony Venice CSC transforms
        * Update images for ACES <--> ACEScg conversion transforms

**Version 1.2 (April 1, 2020):**

* New Features: 
    * Add ACES Metadata File specification document (S-2019-001), XML schema, and example files
    * Add new version of Common LUT Format specification document (S-2014-006)
    * Add new ACES Project Organization and Development Procedure document (P-2019-001)
    * Add ACES Color Space Conversion transforms between:
        * ACES and Canon Canon Log 2 Cinema Gamut
        * ACES and Canon Canon Log 3 Cinema Gamut
        * ACES and ARRI ALEXA LogC (EI800) WideGamut
        * ACES and RED Log3G10 REDWideGamutRGB
        * ACES and Sony S-Log3 S-Gamut3
        * ACES and Sony S-Log3 S-Gamut3.Cine
        * ACES and Panasonic Varicam V-Log V-Gamut
    * Add HDR Output Transforms (RRT+ODT):
        * P3D65 (1000 cd/m^2) ST.2084 (and inverse)
        * P3D65 (2000 cd/m^2) ST.2084 (and inverse)
        * P3D65 (4000 cd/m^2) ST.2084 (and inverse)
    * Add vendor-supplied IDTs for Sony VENICE
* Bug Fixes:
    * Add missing D65 to D60 CAT to 'InvODT.Academy.P3DCI_D65sim_48nits.ctl'
* Other:
    * Revert function parameters of ODTs with full/legal option from 'uniform bool' to 'varying int'
    * Update ACES System Versioning document (S-2014-002)
    * Update TransformIDs of ACES reference implementation transforms
    * Remove "Academy Color Encoding System (ACES) Clip-level Metadata File Format Definition and Usage" (TB-2014-009)
    * Add reference images to accompany new ACEScsc transforms
    * Various minor typo fixes in document LaTeX source files


**Version 1.1 (June 21, 2018):**

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
    * Add new subfunctions to existing ACESlib files:
        * Utilities_Color: 
            * functions for converting between PQ and HLG at 1000 nits
            * `_f3` versions of `moncurve` and `bt1886` encoding functions
        * ODT_Common:
            * `_f3` version of `Y_2_linCV` and `linCV_2_Y`
        * RRT_Common:
            * functions containing the non-tone scale portion of the RRT for simpler application in forward and inverse Output Transforms
    * Add new ACESutil functions:
        * DolbyPQ_to_HLG_1000nits
        * HLG_to_DolbyPQ_1000nits
    * Add HDR Output Transforms (RRT+ODT):
        * Rec.2020 (1000 cd/m^2) ST.2084 (and inverse)
        * Rec.2020 (2000 cd/m^2) ST.2084 (and inverse)
        * Rec.2020 (4000 cd/m^2) ST.2084 (and inverse)
        * Rec.2020 (1000 cd/m^2) HLG (and inverse)
    * Remove HDR ODTs (and inverses)
        * `ODT.Academy.P3D60_ST2084_1000nits.ctl`
        * `ODT.Academy.P3D60_ST2084_2000nits.ctl`
        * `ODT.Academy.P3D60_ST2084_4000nits.ctl`
        * `ODT.Academy.Rec2020_ST2084_1000nits.ctl`
        * `InvODT.Academy.P3D60_ST2084_1000nits.ctl`
        * `InvODT.Academy.P3D60_ST2084_2000nits.ctl`
        * `InvODT.Academy.P3D60_ST2084_4000nits.ctl`
        * `InvODT.Academy.Rec2020_ST2084_1000nits.ctl`
    * Rename some existing transforms for clarity:
        * Rename `DCDM_P3D60` to `DCDM_P3D60limited`
        * Rename `P3DCI` to `P3DCI_D60sim`
        * Rename `RGBmonitor` to `sRGB`
    * Add LMT that can help correct bright blue light clipping or hue shifts
    * Add new reference images for new transforms
    * Add documentation:
        * TB-2018-001 - Derivation of the ACES White Point CIE Chromaticity Coordinates
            * Python module with reference implementation of TB-2018-001
            * iPython Notebook with calculation of values used in TB-2018-001
* Bug Fixes:
    * Arri IDT - Improve linearization of LogC data
* Other:
    * Miscellaneous white space and line wrap fixes in CTL transforms
    * Miscellaneous typo fixes in CTL transform comments
    * Miscellaneous README and CTL comment updates
    * Miscellaneous LaTeX documentation typo and code fixes
    * Update ACEStransformIDs where appropriate
    * Update README and CHANGELOG

**Version 1.0.3 (September 20, 2016):**

  * New Features: 
     * Add new ACEScct color correction working space transforms
     * Add ACEScct specification document
     * Add Sony S-Log3 / S-Gamut3 IDTs
     * Add functions to convert between premultiplied and straight alpha
     * Add D65 RGB Monitor ODT
     * Add new reference images for new transforms
  * Bug Fixes:
     * Update copy and paste typo in ACESproxy document
     * Update ODT functions legal range input variable usage to avoid a situation where it may not execute as intended.
     * Update miscellaneous to local variables in utility functions to avoid clashes with existing global variables
     * Update miscellaneous minor errors in Transform IDs
     * Update miscellaneous transforms missing ACESuserName Tags
  * Other:
     * Update IDT READMEs to reflect latest manufacturer provided information including broken links
     * Restructure utility and lib functions directories for use clarification
     * Restructure directories to consolidate CSC transforms
     * Update equation variables names in ACEScc and ACESproxy documents for greater clarity 
     * Miscellaneous math simplifications in utility functions
     * Miscellaneous white space fixes in CTL transforms
     * Miscellaneous typo fixes in CTL transform comments
     * Remove version number from CTL file names
     * Add Python script to rename CTL files based on TransformID
     * Update all documents to remove version numbers and use date as unique identifier
     * Update all documents to use vector logo
     * Update reference images to reflect code changes
     * Update README and CHANGELOG

**Version 1.0.2 (April 12, 2016):**

  * Add Missing chromatic adaptation transform in Rec2020 1000-nit InvODT
  * Fix ACEStransformID references in DolbyPQ utility CTLs
  * Fix lingering ST2048 references
  * Fix typo in file names in images/README
  * Fix file names in images README
  * Various minor typographical and stylistic fixes
  * Rename all references to ST2084 from ST2048
  * Tabs => spaces and blank space cleanup
  * Update README and CHANGELOG

**Version 1.0.1 (September 4, 2015):**

  * Add ACES technical documentation as LaTeX source to facilitate tracking of document revisions
  * Add utility functions for making the OCIO config
  * Bug fixes:
      * Fix "ACES to ACESproxy" transforms - Correct a bug in piecewise function logic for lin_2_acesProxy() 
  * Improve readability and robustness of some transforms based on suggestions from users
  * Fix various typos in CTL transform comments and documentation
  * Fix a broken link to the reference images
  * Modify HDR ODT transforms
      * Adjust the lowest four coefficients for the HDR ODT tonescales in conjunction with a small offset to allow for obtaining a code value of 0
      * Add a Rec.2020 version of the 1000 nit ODT
      * Change HDR transforms references to "PQ" to "ST2084"

**Version 1.0 (December 20, 2014):**

  * Additional transforms, encodings, documents, and reference images are included as part of the ACES Version 1.0 release. Please carefully review the ACES Version 1.0 documentation package for details on new features and enhancements for ACES Version 1.0
  * Update filenames have been to conform to the ACES System Versioning Specification
  * RRT     
      *  Add a new set of rendering primaries to improve gradeability and vectorscope behavior.  The new primaries, known as AP1, are near the spectrum locus but exceed anticipated device gamuts, including ITU-R BT.2020 at a range of white points.  
      * Modify the global desaturation to apply in RGB space prior to the RRT tone scale.  This was done to improve the overall look of the images based on end-user feedback.
    * Modify The red modifier and glow module variables have been modified.  This was done to improve the overall look of the images based on end-user feedback. 
      * Add a clip of negative values prior to the application of the 3x3 matrix that converts ACES to the rendering primaries. This is added to avoid an error that can occur with negative and saturated ACES values turning positive and saturated.
    * Modify the RRT tone scale to address end-user concerns that the default rendering in v0.7.1 unnecessarily crushed shadow detail
    * Modify the output luminance of an 18% scene reflector from 5.0 nits to 4.8 nits to slightly darken the overall image in response to end-user feedback     
      * Remove the hue restore function to improve grading behavior and address rare instances where image noise could be enhanced.
      * Modify the RRT tone scale to allow for the use of b-splines in the new HDR ODTs.
  * ODTs
      * Add a new set of rendering primaries to improve gradeability and vectorscope behavior.  The new primaries, known as AP1, are near the spectrum locus but exceed anticipated device gamuts, including ITU-R BT.2020 at a range of white points.
    * Modify ODT tone scale to address end-user concerns that the default rendering in v0.7.1 unnecessarily crushed shadow detail
    * Remove hue restore and smart-clip functions to improve grading behavior and address rare instances where image noise could be enhanced
      * Modify ODT tone scales to allow the ability to achieve device black on-set and more quickly in the DI environment
      * Add Rec.709, Rec.2020, and rgbMonitor ODTs supporting dim surround environments
      * Add a runtime flag for full range or legal range output in Rec.709 ODTs. The default is full range.
  * Update ACEScc (formerly ACESlog) and ACESproxy transforms
  * Miscellaneous code cleanups. Removal of unused code

  
**Version 0.7.1 (February 26, 2014):**

  * Bug fixes:
    * Correct the value for a constant in the F65-F55 10-bit IDTs
    * Correct the forward Rec2020 ODTs which were applying the inverse tonescale

**Version 0.7 (January 31, 2014):**

  * Update RRT (forward and inverse):
    * Removal of rendering primaries to address a bug which can occur during exposure grading. Tone scale application is performed directly on ACES RGB.
    * Replacement of the red chroma scaling step with an alternate algorithm to modify reds. This algorithm darkens red colors instead of reducing chroma.
    * Addition of a "glow module", which provides a modest lightening and saturation boost in colored shadow regions.
  * Update ODTs (forwards and inverses):
    * Removal of rendering primaries to address a bug which can occur during exposure grading. Tone scale application is performed directly on OCES RGB.
    * Add new math in the ODT tone scale application to preserve more highlight saturation. 
    * Add Device code value clamping at 1.0, followed by a hue-restore step. 
    * Change in clipping behavior for the Rec709 and rgbMonitor ODTs to produce a closer visual approximation to image appearance on a digital cinema projector.
  * Refactor code to centralize functions common to more than one transform. This removes redundancy and makes the differences between various transforms easier to identify for implementers.  
  * Update IDTs:
    * Update ARRI Alexa IDTs - Modify sensor-RGB-to-ACES matrices for nitpickers. ACES diff ~0.00x; LAB diff after RRT + rgbMonitor ODT ~0.0000x.
    * Update the Sony IDTs - Update F35 ODTs to use the piecewise S-Log1 function. Include F65/F55 IDTs for daylight and tungsten.
  * Fix a typo in ACES-to-ACESproxy10 transform.

**Version 0.2.2 (October 15, 2013):**

  * Restore DCDM ODT with bug fixes
  * Add a variant of the DCDM ODT that limits X'Y'Z' values to P3D60 gamut
  * Relabel "dcsim" ODTs as "d60sim"
  * Modify highlight handling of "d60sim" ODTs to avoid chromaticity shifts in highlights
  * Refactor ODTs for consistency and improved readability
  * Fix a bug in RRT that could occur when ACES triplets had a negative valued mean

**Version 0.2.1 (August 9, 2013):**

  * Temporarily remove DCDM ODT
  * Rename RGB monitor ODT transforms to clarify intended usage (dcsim)

**Version 0.2 (August 2, 2013):**

  * Update RRT
  * Update ODTs
  * Add ODTs for display devices which conform to ITU-R BT.2020
  * Rename ODTs to better reflect their intended usage
  * Add Inverse RRT and ODT transforms
  * Add ACESlog and ACESproxy documentation, reference implementations, and images  
  * Remove ctlrender - now part of CTL
  * Remove document binaries and replaced with links
  * New distribution links for golden reference images

**Version 0.1.1 (June 25, 2012):**

  * Add dw ratio preserving ODT tone curve
  * Fix possible underflow conditions in RRT and ODT CTL transforms
  * Bug fix in ctlrender to avoid streaks in output images
  * Add reverse RRT and RDT splines
  * Modify RDT spline coefficients to tweak shadow reproduction

**Version 0.1 (March 1, 2012):**

  * Add transforms including RRT
  * Add Test Images
  * Add ctlrender sample application for processing images through CTL transforms
  * Add ACES, ADX, and CTL documentation