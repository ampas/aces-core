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