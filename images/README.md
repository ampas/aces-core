## Reference Images ##

Reference, or "golden", images have been provided in order to help you verify 
your implementation of the reference color transformations. Each provided file 
is the output of one of the transforms included with the system. One pictorial, 
one synthetically generated test chart, and one file representative of a film 
scan have been provided.

The reference images can be downloaded individually or as a zip file from: 
<https://www.dropbox.com/sh/9xcfbespknayuft/AAD5SqM-W9RyYiAo8YFsUrqha?dl=0>


### Confirmation ###
To confirm your implementation of each transform:

  1. Use your implementation to process the "input" reference image appropriate to the transform being tested
  2. Compare the output from your implementation to the provided corresponding "output" reference image
  

### Directory Structure ###

**NOTE**: The ACEScc and ACESproxy image files are included for the confirmation of transform implementations only.  
ACEScc and ACESproxy image data is **NOT** intended to be written out to image files. See the ACEScc and ACESproxy documentation for more information.

  * **camera/**
    * *DigitalLAD.2048x1556.dpx* - original file representative of a film scan
    * *SonyF35.StillLife.dpx* - pictorial camera original file
    * *syntheticChart.01.exr* - a synthetic test chart    
  * **ACES/**
    * *DigitalLAD.2048x1556.exr* - `ACEScsc.ADX10_to_ACES.a1.0.1.ctl` applied to *camera/DigitalLAD.2048x1556.dpx*
    * *SonyF35.StillLife.exr* - `IDT.Sony.SLog1_SGamut_10i.a1.v1.ctl` applied to *camera/SonyF35.StillLife.dpx*
    * *syntheticChart.01.exr* - `ACESlib.Unity.a1.0.1.ctl` applied to *camera/syntheticChart.01.exr*
  * **ACEScc/**
    * *SonyF35.StillLife.exr* - `ACEScsc.ACES_to_ACEScc.a1.0.1.ctl` applied to *ACES/SonyF35.StillLife.exr*
    * *syntheticChart.01.exr* - `ACEScsc.ACES_to_ACEScc.a1.0.1.ctl` applied to *ACES/syntheticChart.01.exr*
  * **ACEScg/**
    * *SonyF35.StillLife.exr* - `ACEScsc.ACES_to_ACEScg.a1.0.1.ctl` applied to *ACES/SonyF35.StillLife.exr*
    * *syntheticChart.01.exr* - `ACEScsc.ACES_to_ACEScg.a1.0.1.ctl` applied to *ACES/syntheticChart.01.exr*
  * **ACESproxy/**
    * *SonyF35.StillLife_ACESproxy10i.exr* - `ACEScsc.ACES_to_ACESproxy10i.a1.0.1.ctl` applied to *ACES/SonyF35.StillLife.exr*
    * *SonyF35.StillLife_ACESproxy12i.exr* - `ACEScsc.ACES_to_ACESproxy12i.a1.0.1.ctl` applied to *ACES/SonyF35.StillLife.exr*
    * *syntheticChart_ACESproxy10i.01.exr* - `ACEScsc.ACES_to_ACESproxy10i.a1.0.1.ctl` applied to *ACES/syntheticChart.01.exr*
    * *syntheticChart_ACESproxy12i.01.exr* - `ACEScsc.ACES_to_ACESproxy12i.a1.0.1.ctl` applied to *ACES/syntheticChart.01.exr*
  * **InvACES/**
    * *SonyF35.StillLife_from_ACEScc.exr* - `ACEScsc.ACEScc_to_ACES.a1.0.1.ctl` applied to *ACEScc/SonyF35.StillLife.exr*
    * *SonyF35.StillLife_from_ACEScc.exr* - `ACEScsc.ACEScg_to_ACES.a1.0.1.ctl` applied to *ACEScg/SonyF35.StillLife.exr*
    * *SonyF35.StillLife_from_ACESproxy10i.exr* - `ACEScsc.ACESproxy10i_to_ACES.a1.0.1.ctl` applied to *ACESproxy/SonyF35.StillLife_ACESproxy10i.exr*
    * *SonyF35.StillLife_from_ACESproxy12i.exr* - `ACEScsc.ACESproxy12i_to_ACES.a1.0.1.ctl` applied to *ACESproxy/SonyF35.StillLife_ACESproxy12i.exr*
    * *SonyF35.StillLife_from_InvRRT.exr* - `InvRRT.a1.0.1.ctl` applied to *OCES/SonyF35.StillLife.exr*
    * *syntheticChart.01_from_ACEScc.exr* - `ACEScsc.ACEScc_to_ACES.a1.0.1.ctl` applied to *ACEScc/syntheticChart.01.exr*
    * *syntheticChart.01_from_ACEScc.exr* - `ACEScsc.ACEScg_to_ACES.a1.0.1.ctl` applied to *ACEScg/syntheticChart.01.exr*
    * *syntheticChart.01_from_ACESproxy10i.exr* - `ACEScsc.ACESproxy10i_to_ACES.a1.0.1.ctl` applied to *ACESproxy/syntheticChart.01_ACESproxy10i.exr*
    * *syntheticChart.01_from_ACESproxy12i.exr* - `ACEScsc.ACESproxy12i_to_ACES.a1.0.1.ctl` applied to *ACESproxy/syntheticChart.01_ACESproxy12i.exr*
    * *syntheticChart.01_from_InvRRT.exr* - `InvRRT.a1.0.1.ctl` applied to *OCES/syntheticChart.01.exr*
  * **InvOCES/**
    * *SonyF35.StillLife_from_ODT.Academy.DCDM_P3D60.exr* - `InvODT.Academy.DCDM.a1.0.1.ctl` applied to *ODT/SonyF35.StillLife_ODT.Academy.DCDM_P3D60.tiff*
    * *SonyF35.StillLife_from_ODT.Academy.DCDM.exr* - `InvODT.Academy.DCDM.a1.0.1.ctl` applied to *ODT/SonyF35.StillLife_ODT.Academy.DCDM.tiff*
    * *SonyF35.StillLife_from_ODT.Academy.P3D60_48nits.exr* - `InvODT.Academy.P3D60_48nits.a1.0.1.ctl` applied to *ODT/SonyF35.StillLife_ODT.Academy.P3D60_48nits.tiff*
    * *SonyF35.StillLife_from_ODT.Academy.P3D60_ST2084_1000nits.exr* - `InvODT.Academy.P3D60_ST2084_1000nits.a1.0.1.ctl` applied to *ODT/SonyF35.StillLife_ODT.Academy.P3D60_ST2084_1000nits.tiff*
    * *SonyF35.StillLife_from_ODT.Academy.P3D60_ST2084_2000nits.exr* - `InvODT.Academy.P3D60_ST2084_2000nits.a1.0.1.ctl` applied to *ODT/SonyF35.StillLife_ODT.Academy.P3D60_ST2084_2000nits.tiff*
    * *SonyF35.StillLife_from_ODT.Academy.P3D60_ST2084_4000nits.exr* - `InvODT.Academy.P3D60_ST2084_4000nits.a1.0.1.ctl` applied to *ODT/SonyF35.StillLife_ODT.Academy.P3D60_ST2084_4000nits.tiff*
    * *SonyF35.StillLife_from_ODT.Academy.Rec2020_ST2084_1000nits.exr* - `InvODT.Academy.Rec2020_ST2084_1000nits.a1.0.1.ctl` applied to *ODT/SonyF35.StillLife_ODT.Academy.Rec2020_ST2084_1000nits.tiff*
    * *SonyF35.StillLife_from_ODT.Academy.P3DCI_48nits.exr* - `InvODT.Academy.P3DCI_48nits.a1.0.1.ctl` applied to *ODT/SonyF35.StillLife_ODT.Academy.P3DCI_48nits.tiff*
    * *SonyF35.StillLife_from_ODT.Academy.Rec709_100nits_dim.exr* - `InvODT.Academy.Rec709_100nits_dim.a1.0.1.ctl` applied to *ODT/SonyF35.StillLife_ODT.Academy.Rec709_100nits_dim.tiff*
    * *SonyF35.StillLife_from_ODT.Academy.Rec709_D60sim_100nits_dim.exr* - `InvODT.Academy.Rec709_D60sim_100nits_dim.a1.0.1.ctl` applied to *ODT/SonyF35.StillLife_ODT.Academy.Rec709_D60sim_100nits_dim.tiff*
    * *SonyF35.StillLife_from_ODT.Academy.Rec2020_100nits_dim.exr* - `InvODT.Academy.Rec2020_100nits_dim.a1.0.1.ctl` applied to *ODT/SonyF35.StillLife_ODT.Academy.Rec2020_100nits_dim.tiff*
    * *SonyF35.StillLife_from_ODT.Academy.RGBmonitor_D60sim_100nits_dim.exr* - `InvODT.Academy.DCDM.a1.0.1.ctl` applied to *ODT/SonyF35.StillLife_ODT.Academy.RGBmonitor_D60sim_100nits_dim.tiff*
    * *syntheticChart.01_from_ODT.Academy.DCDM_P3D60.exr* - `InvODT.Academy.DCDM.a1.0.1.ctl` applied to *ODT/syntheticChart.01_ODT.Academy.DCDM_P3D60.tiff*
    * *syntheticChart.01_from_ODT.Academy.DCDM.exr* - `InvODT.Academy.DCDM.a1.0.1.ctl` applied to *ODT/syntheticChart.01_ODT.Academy.DCDM.tiff*
    * *syntheticChart.01_from_ODT.Academy.P3D60_48nits.exr* - `InvODT.Academy.P3D60_48nits.a1.0.1.ctl` applied to *ODT/syntheticChart.01_ODT.Academy.P3D60_48nits.tiff*
    * *syntheticChart.01_from_ODT.Academy.P3D60_ST2084_1000nits.exr* - `InvODT.Academy.P3D60_ST2084_1000nits.a1.0.1.ctl` applied to *ODT/syntheticChart.01_ODT.Academy.P3D60_ST2084_1000nits.tiff*
    * *syntheticChart.01_from_ODT.Academy.P3D60_ST2084_2000nits.exr* - `InvODT.Academy.P3D60_ST2084_2000nits.a1.0.1.ctl` applied to *ODT/syntheticChart.01_ODT.Academy.P3D60_ST2084_2000nits.tiff*
    * *syntheticChart.01_from_ODT.Academy.P3D60_ST2084_4000nits.exr* - `InvODT.Academy.P3D60_ST2084_4000nits.a1.0.1.ctl` applied to *ODT/syntheticChart.01_ODT.Academy.P3D60_ST2084_4000nits.tiff*
    * *syntheticChart.01_from_ODT.Academy.Rec2020_ST2084_1000nits.exr* - `InvODT.Academy.Rec2020_ST2084_1000nits.a1.0.1.ctl` applied to *ODT/syntheticChart.01_ODT.Academy.Rec2020_ST2084_1000nits.tiff*
    * *syntheticChart.01_from_ODT.Academy.P3DCI_48nits.exr* - `InvODT.Academy.P3DCI_48nits.a1.0.1.ctl` applied to *ODT/syntheticChart.01_ODT.Academy.P3DCI_48nits.tiff*
    * *syntheticChart.01_from_ODT.Academy.Rec709_100nits_dim.exr* - `InvODT.Academy.Rec709_100nits_dim.a1.0.1.ctl` applied to *ODT/syntheticChart.01_ODT.Academy.Rec709_100nits_dim.tiff*
    * *syntheticChart.01_from_ODT.Academy.Rec709_D60sim_100nits_dim.exr* - `InvODT.Academy.Rec709_D60sim_100nits_dim.a1.0.1.ctl` applied to *ODT/syntheticChart.01_ODT.Academy.Rec709_D60sim_100nits_dim.tiff*
    * *syntheticChart.01_from_ODT.Academy.Rec2020_100nits_dim.exr* - `InvODT.Academy.Rec2020_100nits_dim.a1.0.1.ctl` applied to *ODT/syntheticChart.01_ODT.Academy.Rec2020_100nits_dim.tiff*
    * *syntheticChart.01_from_ODT.Academy.RGBmonitor_D60sim_100nits_dim.exr* - `InvODT.Academy.DCDM.a1.0.1.ctl` applied to *ODT/syntheticChart.01_ODT.Academy.RGBmonitor_D60sim_100nits_dim.tiff*
  * **LMT/**
    * *SonyF35.StillLife_LMT.Academy.ACES_0_1_1.exr* - `LMT.Academy.ACES_0_1_1.a1.0.1.ctl` applied to *ACES/SonyF35.StillLife.exr*
    * *SonyF35.StillLife_LMT.Academy.ACES_0_2_2.exr* - `LMT.Academy.ACES_0_2_2.a1.0.1.ctl` applied to *ACES/SonyF35.StillLife.exr*
    * *SonyF35.StillLife_LMT.Academy.ACES_0_7_1.exr* - `LMT.Academy.ACES_0_7_1.a1.0.1.ctl` applied to *ACES/SonyF35.StillLife.exr*
    * *syntheticChart.01_LMT.Academy.ACES_0_1_1.exr* - `LMT.Academy.ACES_0_1_1.a1.0.1.ctl` applied to *ACES/syntheticChart.01.exr*
    * *syntheticChart.01_LMT.Academy.ACES_0_2_2.exr* - `LMT.Academy.ACES_0_2_2.a1.0.1.ctl` applied to *ACES/syntheticChart.01.exr*
    * *syntheticChart.01_LMT.Academy.ACES_0_7_1.exr* - `LMT.Academy.ACES_0_7_1.a1.0.1.ctl` applied to *ACES/syntheticChart.01.exr*
  * **OCES/**
    * *SonyF35.StillLife.exr* - `RRT.a1.0.1.ctl` applied to *ACES/SonyF35.StillLife.exr*
    * *syntheticChart.01.exr* - `RRT.a1.0.1.ctl` applied to *ACES/syntheticChart.01.exr*
  * **ODT/**
    * *SonyF35.StillLife_ODT.Academy.DCDM_P3D60.tiff* - `ODT.Academy.DCDM_P3D60.a1.0.1.ctl` applied to *OCES/SonyF35.StillLife.exr*
    * *SonyF35.StillLife_ODT.Academy.DCDM.tiff* - `ODT.Academy.DCDM.a1.0.1.ctl` applied to *OCES/SonyF35.StillLife.exr*    
    * *SonyF35.StillLife_ODT.Academy.P3D60_48nits.tiff* - `ODT.Academy.P3D60_48nits.a1.0.1.ctl` applied to *OCES/SonyF35.StillLife.exr*    
    * *SonyF35.StillLife_ODT.Academy.P3D60_ST2084_1000nits.tiff* - `ODT.Academy.P3D60_ST2084_1000nits.a1.0.1.ctl` applied to *OCES/SonyF35.StillLife.exr*    
    * *SonyF35.StillLife_ODT.Academy.P3D60_ST2084_2000nits.tiff* - `ODT.Academy.P3D60_ST2084_2000nits.a1.0.1.ctl` applied to *OCES/SonyF35.StillLife.exr*    
    * *SonyF35.StillLife_ODT.Academy.P3D60_ST2084_4000nits.tiff* - `ODT.Academy.P3D60_ST2084_4000nits.a1.0.1.ctl` applied to *OCES/SonyF35.StillLife.exr*    
    * *SonyF35.StillLife_ODT.Academy.Rec2020_ST2084_1000nits.tiff* - `ODT.Academy.Rec2020_ST2084_1000nits.a1.0.1.ctl` applied to *OCES/SonyF35.StillLife.exr*    
    * *SonyF35.StillLife_ODT.Academy.P3DCI_48nits.tiff* - `ODT.Academy.P3DCI_48nits.a1.0.1.ctl` applied to *OCES/SonyF35.StillLife.exr*    
    * *SonyF35.StillLife_ODT.Academy.Rec709_100nits_dim.tiff* - `ODT.Academy.Rec709_100nits_dim.a1.0.1.ctl` applied to *OCES/SonyF35.StillLife.exr*    
    * *SonyF35.StillLife_ODT.Academy.Rec709_D60sim_100nits_dim.tiff* - `ODT.Academy.Rec709_D60sim_100nits_dim.a1.0.1.ctl` applied to *OCES/SonyF35.StillLife.exr*    
    * *SonyF35.StillLife_ODT.Academy.Rec2020_100nits_dim.tiff* - `ODT.Academy.Rec2020_100nits_dim.a1.0.1.ctl` applied to *OCES/SonyF35.StillLife.exr*    
    * *SonyF35.StillLife_ODT.Academy.RGBmonitor_D60sim_100nits_dim.tiff* - `ODT.Academy.RGBmonitor_D60sim_100nits_dim.a1.0.1.ctl` applied to *OCES/SonyF35.StillLife.exr*    
    * *syntheticChart.01_ODT.Academy.DCDM_P3D60.tiff* - `ODT.Academy.DCDM_P3D60.a1.0.1.ctl` applied to *OCES/syntheticChart.01.exr*
    * *syntheticChart.01_ODT.Academy.DCDM.tiff* - `ODT.Academy.DCDM.a1.0.1.ctl` applied to *OCES/syntheticChart.01.exr*    
    * *syntheticChart.01_ODT.Academy.P3D60_48nits.tiff* - `ODT.Academy.P3D60_48nits.a1.0.1.ctl` applied to *OCES/syntheticChart.01.exr*    
    * *syntheticChart.01_ODT.Academy.P3D60_ST2084_1000nits.tiff* - `ODT.Academy.P3D60_ST2084_1000nits.a1.0.1.ctl` applied to *OCES/syntheticChart.01.exr*    
    * *syntheticChart.01_ODT.Academy.P3D60_ST2084_2000nits.tiff* - `ODT.Academy.P3D60_ST2084_2000nits.a1.0.1.ctl` applied to *OCES/syntheticChart.01.exr*    
    * *syntheticChart.01_ODT.Academy.P3D60_ST2084_4000nits.tiff* - `ODT.Academy.P3D60_ST2084_4000nits.a1.0.1.ctl` applied to *OCES/syntheticChart.01.exr*    
    * *syntheticChart.01_ODT.Academy.Rec2020_ST2084_1000nits.tiff* - `ODT.Academy.Rec2020_ST2084_1000nits.a1.0.1.ctl` applied to *OCES/syntheticChart.01.exr*    
    * *syntheticChart.01_ODT.Academy.P3DCI_48nits.tiff* - `ODT.Academy.P3DCI_48nits.a1.0.1.ctl` applied to *OCES/syntheticChart.01.exr*    
    * *syntheticChart.01_ODT.Academy.Rec709_100nits_dim.tiff* - `ODT.Academy.Rec709_100nits_dim.a1.0.1.ctl` applied to *OCES/syntheticChart.01.exr*    
    * *syntheticChart.01_ODT.Academy.Rec709_D60sim_100nits_dim.tiff* - `ODT.Academy.Rec709_D60sim_100nits_dim.a1.0.1.ctl` applied to *OCES/syntheticChart.01.exr*    
    * *syntheticChart.01_ODT.Academy.Rec2020_100nits_dim.tiff* - `ODT.Academy.Rec2020_100nits_dim.a1.0.1.ctl` applied to *OCES/syntheticChart.01.exr*    
    * *syntheticChart.01_ODT.Academy.RGBmonitor_D60sim_100nits_dim.tiff* - `ODT.Academy.RGBmonitor_D60sim_100nits_dim.a1.0.1.ctl` applied to *OCES/syntheticChart.01.exr*    
