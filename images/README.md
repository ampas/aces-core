## Reference Images ##

Reference, or "golden", images have been provided in order to help you verify your implementation of the reference color transformations. Each provided file is the output of one of the transforms included with the system. One pictorial, one synthetically generated test chart, and one file representative of a film scan have been provided.

The reference images can be downloaded individually or as a zip file from: 
<https://www.dropbox.com/sh/bwfrqgyt20gz4dp/XShJffwvXR>


### Confirmation ###
To confirm your implementation of each transform:

  1. Use your implementation to process the "input" reference image appropriate to the transform being tested
  2. Compare the output from your implementation to the provided corresponding "output" reference image

### Directory Structure ###
  * **camOrig/**
    * *DigitalLAD.2048x1556.dpx* - original file representative of a film scan
    * *SonyF35.StillLife.dpx* - pictorial camera original file
  * **aces/**
    * *DigitalLAD.2048x1556.exr* - `adx10_to_aces.ctl` applied to *DigitalLAD.2048x1556.dpx*
    * *SonyF35.StillLife.exr* - `S-log10_to_ACES.ctl` applied to *SonyF35.StillLife.dpx*
    * *syntheticChart.01.exr* - synthetic ACES image
  * **oces/**
    * *SonyF35.StillLife_oces.exr* - `rrt.ctl` applied to *SonyF35.StillLife.exr*
    * *syntheticChart.01_oces.exr* - `rrt.ctl` applied to *syntheticChart.01.exr*
  * **odt/**
    * *SonyF35.StillLife_dcdm_p3d60.tiff* - `odt_dcdm_p3d60.ctl` applied to *SonyF35.StillLife_oces.exr*
    * *SonyF35.StillLife_dcdm.tiff* - `odt_dcdm.ctl` applied to *SonyF35.StillLife_oces.exr*
    * *SonyF35.StillLife_p3d60.tiff* - `odt_p3d60.ctl` applied to *SonyF35.StillLife_oces.exr*
    * *SonyF35.StillLife_p3dci.tiff* - `odt_p3dci.ctl` applied to *SonyF35.StillLife_oces.exr*
    * *SonyF35.StillLife_rec709_d60sim_full_100nits.tiff* - `odt_rec709_d60sim_full_100nits.ctl` applied to *SonyF35.StillLife_oces.exr*
    * *SonyF35.StillLife_rec709_d60sim_smpte_100nits.tiff* - `odt_rec709_d60sim_smpte_100nits.ctl` applied to *SonyF35.StillLife_oces.exr*
    * *SonyF35.StillLife_rec709_full_100nits.tiff* - `odt_rec709_full_100nits.ctl` applied to *SonyF35.StillLife_oces.exr*
    * *SonyF35.StillLife_rec709_smpte_100nits.tiff* - `odt_rec709_smpte_100nits.ctl` applied to *SonyF35.StillLife_oces.exr*
    * *SonyF35.StillLife_rec2020_d60sim_full_100nits.tiff* - `odt_rec2020_d60sim_full_100nits.ctl` applied to *SonyF35.StillLife_oces.exr*
    * *SonyF35.StillLife_rec2020_d60sim_smpte_100nits.tiff* - `odt_rec2020_d60sim_smpte_100nits.ctl` applied to *SonyF35.StillLife_oces.exr*
    * *SonyF35.StillLife_rec2020_full_100nits.tiff* - `odt_rec2020_full_100nits.ctl` applied to *SonyF35.StillLife_oces.exr*
    * *SonyF35.StillLife_rec2020_smpte_100nits.tiff* - `odt_rec2020_smpte_100nits.ctl` applied to *SonyF35.StillLife_oces.exr*
    * *SonyF35.StillLife_rgbMonitor_d60sim_100nits.tiff* - `odt_rgbMonitor_d60sim_100nits.ctl` applied to *SonyF35.StillLife_oces.exr*
    * *syntheticChart.01_dcdm_p3d60.tiff* - `odt_dcdm_p3d60.ctl` applied to *syntheticChart.01_oces.exr*
    * *syntheticChart.01_dcdm.tiff* - `odt_dcdm.ctl` applied to *syntheticChart.01_oces.exr*
    * *syntheticChart.01_p3d60.tiff* - `odt_p3d60.ctl` applied to *syntheticChart.01_oces.exr*
    * *syntheticChart.01_p3dci.tiff* - `odt_p3dci.ctl` applied to *syntheticChart.01_oces.exr*
    * *syntheticChart.01_rec709_d60sim_full_100nits.tiff* - `odt_rec709_d60sim_full_100nits.ctl` applied to *syntheticChart.01_oces.exr*
    * *syntheticChart.01_rec709_d60sim_smpte_100nits.tiff* - `odt_rec709_d60sim_smpte_100nits.ctl` applied to *syntheticChart.01_oces.exr*
    * *syntheticChart.01_rec709_full_100nits.tiff* - `odt_rec709_full_100nits.ctl` applied to *syntheticChart.01_oces.exr*
    * *syntheticChart.01_rec709_smpte_100nits.tiff* - `odt_rec709_smpte_100nits.ctl` applied to *syntheticChart.01_oces.exr*
    * *syntheticChart.01_rec2020_d60sim_full_100nits.tiff* - `odt_rec2020_d60sim_full_100nits.ctl` applied to *syntheticChart.01_oces.exr*
    * *syntheticChart.01_rec2020_d60sim_smpte_100nits.tiff* - `odt_rec2020_d60sim_smpte_100nits.ctl` applied to *syntheticChart.01_oces.exr*
    * *syntheticChart.01_rec2020_full_100nits.tiff* - `odt_rec2020_full_100nits.ctl` applied to *syntheticChart.01_oces.exr*
    * *syntheticChart.01_rec2020_smpte_100nits.tiff* - `odt_rec2020_smpte_100nits.ctl` applied to *syntheticChart.01_oces.exr*
    * *syntheticChart.01_rgbMonitor_d60sim_100nits.tiff* - `odt_rgbMonitor_d60sim_100nits.ctl` applied to *syntheticChart.01_oces.exr*
  * **aces_inv/**
    * *SonyF35.StillLife_aces_from_rrt_inv.exr* - `rrt_inv.ctl` applied to `SonyF35.StillLife_oces.exr`
    * *SonyF35.StillLife_aces_from_acesProxy10.exr* - `acesProxy10_to_aces.ctl` applied to *SonyF35.StillLife_acesProxy10.tiff*
    * *SonyF35.StillLife_aces_from_acesProxy12.exr* - `acesProxy12_to_aces.ctl` applied to *SonyF35.StillLife_acesProxy12.tiff*
    * *SonyF35.StillLife_aces_from_acesLog16i.exr* - `acesLog_to_aces.ctl` applied to *SonyF35.StillLife_acesLog16i.tiff*
    * *SonyF35.StillLife_aces_from_acesLog32f.exr* - `acesLog_to_aces.ctl` applied to *SonyF35.StillLife_acesLog32f.tiff*
    * *syntheticChart.01_aces_from_rrt_inv.exr* - `rrt_inv.ctl` applied to `syntheticChart.01_oces.exr`
    * *syntheticChart.01_aces_from_acesProxy10.exr* - `acesProxy10_to_aces.ctl` applied to *syntheticChart.01_acesProxy10.tiff*
    * *syntheticChart.01_aces_from_acesProxy12.exr* - `acesProxy12_to_aces.ctl` applied to *syntheticChart.01_acesProxy12.tiff*
    * *syntheticChart.01_aces_from_acesLog16i.exr* - `acesLog_to_aces.ctl` applied to *syntheticChart.01_acesLog16i.tiff*
    * *syntheticChart.01_aces_from_acesLog32f.exr* - `acesLog_to_aces.ctl` applied to *syntheticChart.01_acesLog32f.tiff*
  * **oces_inv/**
    * *SonyF35.StillLife_oces_from_dcdm_inv.tiff* - `odt_dcdm_inv.ctl` applied to *SonyF35.StillLife_dcdm.exr*
    * *SonyF35.StillLife_oces_from_dcdm_p3d60_inv.tiff* - `odt_dcdm_inv.ctl` applied to *SonyF35.StillLife_dcdm_p3d60.exr*
    * *SonyF35.StillLife_oces_from_p3d60_inv.exr* - `odt_p3d60_inv.ctl` applied to *SonyF35.StillLife_p3d60.tiff*
    * *SonyF35.StillLife_oces_from_p3dci_inv.exr* - `odt_p3dci_inv.ctl` applied to *SonyF35.StillLife_p3dci.tiff*
    * *SonyF35.StillLife_oces_from_rec709_d60sim_full_100nits_inv.exr* - `odt_rec709_d60sim_full_100nits_inv.ctl` applied to *SonyF35.StillLife_rec709_d60sim_full_100nits.tiff*
    * *SonyF35.StillLife_oces_from_rec709_d60sim_smpte_100nits_inv.exr* - `odt_rec709_d60sim_smpte_100nits_inv.ctl` applied to *SonyF35.StillLife_rec709_smpte_100nits.tiff*
    * *SonyF35.StillLife_oces_from_rec709_full_100nits.exr_inv* - `odt_rec709_full_100nits_inv.ctl` applied to *SonyF35.StillLife_rec709_full_100nits.tiff*
    * *SonyF35.StillLife_oces_from_rec709_smpte_100nits.exr_inv* - `odt_rec709_smpte_100nits_inv.ctl` applied to *SonyF35.StillLife_rec709_smpte_100nits.tiff*
    * *SonyF35.StillLife_oces_from_rec2020_d60sim_full_100nits_inv.exr* - `odt_rec2020_d60sim_full_100nits_inv.ctl` applied to *SonyF35.StillLife_rec2020_d60sim_full_100nits.tiff*
    * *SonyF35.StillLife_oces_from_rec2020_d60sim_smpte_100nits_inv.exr* - `odt_rec2020_d60sim_smpte_100nits_inv.ctl` applied to *SonyF35.StillLife_rec2020_d60sim_smpte_100nits.tiff*
    * *SonyF35.StillLife_oces_from_rec2020_full_100nits_inv.exr* - `odt_rec2020_full_100nits_inv.ctl` applied to *SonyF35.StillLife_rec2020_full_100nits.tiff*
    * *SonyF35.StillLife_oces_from_rec2020_smpte_100nits_inv.exr* - `odt_rec2020_smpte_100nits_inv.ctl` applied to *SonyF35.StillLife_rec2020_smpte_100nits.tiff*
    * *SonyF35.StillLife_oces_from_rgbMonitor_d60sim_100nits_inv.exr* - `odt_rgbMonitor_d60sim_100nits_inv.ctl` applied to *SonyF35.StillLife_rgbMonitor_d60sim_100nits.tiff*   
    * *syntheticChart.01_oces_from_dcdm.tiff* - `odt_dcdm_inv.ctl` applied to *syntheticChart.01_dcdm.exr*
    * *syntheticChart.01_oces_from_dcdm_p3d60.tiff* - `odt_dcdm_inv.ctl` applied to *syntheticChart.01_dcdm_p3d60.exr*
    * *syntheticChart.01_oces_from_p3d60_inv.exr* - `odt_p3d60_inv.ctl` applied to *syntheticChart.01_p3d60.tiff*
    * *syntheticChart.01_oces_from_p3dci_inv.exr* - `odt_p3dci_inv.ctl` applied to *syntheticChart.01_p3dci.tiff*
    * *syntheticChart.01_oces_from_rec709_d60sim_full_100nits_inv.exr* - `odt_rec709_d60sim_full_100nits_inv.ctl` applied to *syntheticChart.01_rec709_d60sim_full_100nits.tiff*
    * *syntheticChart.01_oces_from_rec709_d60sim_smpte_100nits_inv.exr* - `odt_rec709_d60sim_smpte_100nits_inv.ctl` applied to *syntheticChart.01_rec709_d60sim_smpte_100nits.tiff*
    * *syntheticChart.01_oces_from_rec709_full_100nits_inv.exr* - `odt_rec709_full_100nits_inv.ctl` applied to *syntheticChart.01_rec709_full_100nits.tiff*
    * *syntheticChart.01_oces_from_rec709_smpte_100nits_inv.exr* - `odt_rec709_smpte_100nits_inv.ctl` applied to *syntheticChart.01_rec709_smpte_100nits.tiff*
    * *syntheticChart.01_oces_from_rec2020_d60sim_full_100nits_inv.exr* - `odt_rec2020_d60sim_full_100nits_inv.ctl` applied to *syntheticChart.01_rec2020_d60sim_full_100nits.tiff*
    * *syntheticChart.01_oces_from_rec2020_d60sim_smpte_100nits_inv.exr* - `odt_rec2020_d60sim_smpte_100nits_inv.ctl` applied to *syntheticChart.01_rec2020_d60sim_smpte_100nits.tiff*
    * *syntheticChart.01_oces_from_rec2020_full_100nits_inv.exr* - `odt_rec2020_full_100nits_inv.ctl` applied to *syntheticChart.01_rec2020_full_100nits.tiff*
    * *syntheticChart.01_oces_from_rec2020_smpte_100nits_inv.exr* - `odt_rec2020_smpte_100nits_inv.ctl` applied to *syntheticChart.01_rec2020_smpte_100nits.tiff*
    * *syntheticChart.01_oces_from_rgbMonitor_d60sim_100nits_inv.exr* - `odt_rgbMonitor_d60sim_100nits_inv.ctl` applied to *syntheticChart.01_rgbMonitor_d60sim_100nits.tiff*
  * **lmt/**
    * *SonyF35.StillLife_lmt_aces_v0.1.1.exr* - `lmt_aces_v0.1.1.ctl` applied to *SonyF35.StillLife.exr*
    * *SonyF35.StillLife_lmt_lowContrast.exr* - `lmt_lowContrast.ctl` applied to *SonyF35.StillLife.exr*
    * *syntheticChart.01_lmt_aces_v0.1.1.exr* - `lmt_aces_v0.1.1.ctl` applied to *syntheticChart.01.exr*
    * *syntheticChart.01_lmt_lowContrast.exr* - `lmt_lowContrast.ctl` applied to *syntheticChart.01.exr*

**NOTE**: The following image files are included for the confirmation of transform implementations only.  
ACESlog and ACESproxy image data is **NOT** intended to be written out to image files. See the ACESlog and ACESproxy documentation for more information.

  * **acesLog/**
    * *SonyF35.StillLife_acesLog16i.tiff* - `aces_to_acesLog16i.ctl` applied to *SonyF35.StillLife.exr*
    * *SonyF35.StillLife_acesLog32f.tiff* - `aces_to_acesLog32f.ctl` applied to *SonyF35.StillLife.exr*
    * *syntheticChart.01_acesLog16i.tiff* - `aces_to_acesLog16i.ctl` applied to *syntheticChart.01.exr*
    * *syntheticChart.01_acesLog32f.tiff* - `aces_to_acesLog32f.ctl` applied to *syntheticChart.01.exr*
  * **acesProxy/**
    * *SonyF35.StillLife_acesProxy10.tiff* - `aces_to_acesProxy10.ctl` applied to *SonyF35.StillLife.exr*
    * *SonyF35.StillLife_acesProxy12.tiff* - `aces_to_acesProxy12.ctl` applied to *SonyF35.StillLife.exr*
    * *syntheticChart.01_acesProxy10.tiff* - `aces_to_acesProxy10.ctl` applied to *syntheticChart.01.exr*
    * *syntheticChart.01_acesProxy12.tiff* - `aces_to_acesProxy12.ctl` applied to *syntheticChart.01.exr*
