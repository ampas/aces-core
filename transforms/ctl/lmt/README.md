## Look Modification Transforms (LMTs) ##

### Introduction to LMTs ###

Look Modification Transforms (LMTs) provide a means to apply a variety of looks 
to ACES images. LMTs can used to change the look from the default associated 
with the RRT to a custom look that one might want to use. The range of looks 
achievable via LMTs are only limited by the transformation used in the LMT 
itself.

LMTs are always ACES to ACES transformations. In the example below the resulting 
ACES data after the application of an LMT is designated ACES'. ACES' data is 
then viewed through the RRT and an ODT as illustrated in the diagram below.  

                |--------|           |-------|          |-------|
                |        |           |       |          |       |
      ACES ---->|  LMT   |---ACES'-->|  RRT  |---OCES-->|  ODT  |--> code values
                |        |           |       |          |       |
                |--------|           |-------|          |-------| 

### Building LMTS ###

The most direct way to create an LMT is to interactively modify the ACES data 
via an LMT algorithm while viewing the results through the RRT and ODT. An 
example of an LMT created using this method is the "low contrast" LMT included 
with this package. ( see [`lmt_lowContrast.ctl`](./lmt_lowContrast.ctl) )

A second method to create an LMT is to use the inverse RRT and ODT transforms. 
This method can be used to create a match to an existing look such as a Print 
Film Emulation or a custom look created outside of the ACES system. An example 
of an LMT created using this method is the ACESv0.1.1 LMT included
with this package. ( see [`lmt_aces_v0.1.1.ctl`](./lmt_aces_v0.1.1.ctl) )

The ACESv0.1.1 LMT was created to provide a means to recreate the look of the
RRT/ODT system that shipped with v0.1.1 of the ACES system.  To create that LMT 
a series of ACES values were processed through the v0.1.1 RRT and the v0.1.1 
P3D60 ODT. The resulting P3 code values were then transformed through the 
inverse v0.2 P3D60 ODT and v0.2 RRT to yield a corresponding set of ACES' 
values. The original ACES values and the ACES' values were used to build a 3DLUT 
mapping which serves as the ACESv0.1.1 LMT.  The following diagram illustrates 
the process.

Generation of the LMT to ACESv0.1.1:

                  |--------|          |--------|
    :- - - :      | RRT    |          | P3D60  |
    : ACES :----->| v0.1.1 |---OCES-->|  ODT   |---- P3 code values
    :  ||  :      |        |          | v0.1.1 |          |
    :  ||  :      |--------|          |--------|          |
    : 3DLUT:                                              |
    :mapping                                              |
    :  ||  :      |--------|          |-------|           |
    :  \/  :      | Inverse|          |Inverse|           |
    : ACES':<-----| RRT    |<--OCES---| P3D60 |<----------| 
    : - - -:      | v0.2   |          |  ODT  |   
       ::         |--------|          |-------|        
       ::
       ::
       :: = = = = = ::
                    ::
                    ::
                    \/
                |--------|           |------|          |------|
                | LMT to |           | RRT  |          | ODT  |
      ACES ---->|  ACES  |---ACES'-->| v0.2 |---OCES-->| v0.2 |--> code values
                | v0.1.1 |           |------|          |------|
                |--------|


### Application of LMTs to ACES data ###
                
Care should be taken when using LMTs as the LMT transform can inadvertently 
limit the dynamic range of the ACES' data. This is particularly true when using 
LMTs created using the inverse ODT and inverse RRT method. This is due to the
inherent dynamic range limitation associated with the transformation of ACES
data to a set of display code values. ACES' data created using an LMT such as
the ACESv0.1.1 LMT will not contain the additional dynamic range usually 
associated with ACES data.

ACES' data created using an algorithmic LMT such as the low contrast LMT may not
have this limitation. If the algorithm does not limit the dynamic range during
the transformation from ACES to ACES', then the LMT may preserve the dynamic 
range associated with the original ACES data.