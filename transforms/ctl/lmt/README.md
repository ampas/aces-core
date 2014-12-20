## Look Modification Transforms (LMTs) ##

The following is a very brief overview of Look Modification Transforms (LMTs). For more information, please review TB-2014-010, which can be found in the [`documents`](../../../documents) directory.

### Introduction to LMTs ###

Look Modification Transforms (LMTs) provide a means to apply a variety of looks to ACES images. LMTs can be used to change the look from the default associated with the RRT to a custom look that one might want to use. The range of looks achievable via LMTs are only limited by the transformation used in the LMT itself.

LMTs are always ACES to ACES transformations. In the example below the resulting ACES data after the application of an LMT is designated ACES'. ACES' data is then viewed through the RRT and an ODT as illustrated in the diagram below.  

                |--------|           |-------|          |-------|
                |        |           |       |          |       |
      ACES ---->|  LMT   |---ACES'-->|  RRT  |---OCES-->|  ODT  |--> code values
                |        |           |       |          |       |
                |--------|           |-------|          |-------| 

### Building the Included LMTS ###

The LMTs included with this package are "empirical" LMTs, described in section 7.2.2 of TB-2014-010. By using inverse RRT and ODT transforms, this method can be used to create a match to an existing look such as a Print Film Emulation or a custom look that was created outside of the ACES system. An example of an LMT created using this method is the ACES v0.1.1 LMT included with this package. ( see [`LMT.Academy.ACES_0_1_1.a1.0.0.ctl`](./LMT.Academy.ACES_0_1_1.a1.0.0.ctl) )

The ACES v0.1.1 LMT was created to provide a means to recreate the look of the RRT/ODT system that shipped with v0.1.1 of ACES.  To create it, a series of ACES values were processed through the v0.1.1 RRT and the v0.1.1 P3D60 ODT. The resulting P3 code values were then processed through the inverse P3D60 ODT and RRT of the current ACES release transforms to yield a corresponding set of ACES' values. The original ACES values and the ACES' values were used to build a 3DLUT mapping which serves as the ACES v0.1.1 LMT.  The following diagram illustrates the process.

Generation of the LMT to ACESv0.1.1:

                  |--------|          |--------|
    :- - - :      | RRT    |          | P3D60  |
    : ACES :----->| v0.1.1 |---OCES-->|  ODT   |---- P3 code values
    :  ||  :      |        |          | v0.1.1 |          |
    :  ||  :      |--------|          |--------|          |
    : 3DLUT:                                              |
    :mapping                                              |
    :  ||  :      |--------|          |-------|           |
    :  \/  :      | Current|          |Inverse|           |
    : ACES':<-----| Inverse|<--OCES---| P3D60 |<----------| 
    : - - -:      | RRT    |          |  ODT  |   
       ::         |--------|          |-------|        
       ::
       ::
       :: = = = = = ::
                    ::
                    ::
                    \/
                |--------|           |-------|          |-------|
                | LMT to |           |Current|          |Current|
      ACES ---->|  ACES  |---ACES'-->|  RRT  |---OCES-->|  ODT  |--> code values
                | v0.1.1 |           |-------|          |-------|
                |--------|


### Application of LMTs to ACES data ###
                
Care should be taken when using LMTs as the LMT transform can inadvertently limit the dynamic range of the ACES' data. This is particularly true when using empirical LMTs. This is due to the inherent dynamic range limitation associated with the transformation of ACESdata to a set of display code values. ACES' data created using an LMT such as the ACESv0.1.1 LMT will not contain the additional dynamic range usually associated with ACES data.

ACES' data created using an analytic LMT may not have this limitation. If the algorithm does not limit the dynamic range during the transformation from ACES to ACES', then the LMT may preserve the dynamic range associated with the original ACES data.