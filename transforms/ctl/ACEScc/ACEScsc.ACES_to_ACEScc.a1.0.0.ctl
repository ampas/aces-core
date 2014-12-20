
// <ACEStransformID>ACEScsc.ACES_to_ACEScc.a1.0.0</ACEStransformID>
// <ACESuserName>ACES2065-1 to ACEScc</ACESuserName>

//
// ACES Color Space Conversion - ACES to ACEScc
//
// converts ACES2065-1 (AP0 w/ linear encoding) to 
//          ACEScc (AP1 w/ logarithmic encoding)
//
// This transform follows the formulas from section 4.4 in S-2014-003
//

// *-*-*-*-*-*-*-*-*
// ACEScc is intended to be transient and internal to software or hardware 
// systems, and is specifically not intended for interchange or archiving.
// ACEScc should NOT be written into a container file in actual implementations!
// *-*-*-*-*-*-*-*-*



import "ACESlib.Transform_Common.a1.0.0";



float lin_to_ACEScc( input varying float in)
{
  if (in <= 0)
    return -0.35828683; // =(log2( pow(2.,-15.)*0.5)+9.72)/17.52
  else if (in < pow(2.,-15.))
    return (log2( pow(2.,-16.) + in * 0.5) + 9.72) / 17.52;
  else // (in >= pow(2.,-15))
    return (log2(in) + 9.72) / 17.52;
}



void main
(   
    input varying float rIn,
    input varying float gIn,
    input varying float bIn,
    input varying float aIn,
    output varying float rOut,
    output varying float gOut,
    output varying float bOut,
    output varying float aOut
)
{
    float ACES[3] = { rIn, gIn, bIn};

    ACES = clamp_f3( ACES, 0.0, HALF_POS_INF);
    // NOTE: (from Annex A of S-2014-003)
    // When ACES values are matrixed into the smaller AP1 space, colors outside 
    // the AP1 gamut can generate negative values even before the log encoding. 
    // If these values are clipped, a conversion back to ACES will not restore 
    // the original colors. A specific method of reserving negative values 
    // produced by the transformation matrix has not been defined in part to 
    // help ease adoption across various color grading systems that have 
    // different capabilities and methods for handling negative values. Clipping 
    // these values has been found to have minimal visual impact when viewed 
    // through the RRT and ODT on currently available display technology. 
    // However, to preserve creative choice in downstream processing and to 
    // provide the highest quality archival master, developers implementing 
    // ACEScc encoding are encouraged to adopt a method of preserving negative 
    // values so that a conversion from ACES to ACEScc and back can be made 
    // lossless.

    float ACESccLin[3] = mult_f3_f44( ACES, AP0_2_AP1_MAT);

    rOut = lin_to_ACEScc( ACESccLin[0]);
    gOut = lin_to_ACEScc( ACESccLin[1]);
    bOut = lin_to_ACEScc( ACESccLin[2]);
    aOut = aIn;
}