// 
// Output Device Transform to P3D60, encoded as X'Y'Z'
// v0.7
//

//
// Summary:
//  This ODT encodes XYZ colorimetry that is gamut-limited to P3 primaries with 
//  a D60 whitepoint. This has two advantages:
// 
//   1) "Gamut mapping" is controlled by the ODT (via a "smart-clip" that 
//      preserves hue). Otherwise, the projector would be relied upon to handle 
//      any values outside of the P3 gamut. In most devices, this is performed 
//      using a simple clip, which does not necessarily preserve hue.   
//      Furthermore, different projectors may handle out-of-gamut values 
//      differently, which means that if out-of-gamut values were left to be 
//      handled by the device, different image appearance could result on 
//      different devices even though they have the same gamut.
//
//   2) Assuming the content was graded (and approved) on a projector with a 
//      P3D60 gamut, limiting the colors to that gamut assures there will be
//      no unexpected color appearance if the DCP is later viewed on a device 
//      with a wider gamut.
// 
//  The assumed observer adapted white is D60, and the viewing environment is 
//  that of a dark theater. 
//
//  This transform shall be used for a device calibrated to match the Digital 
//  Cinema Reference Projector Specification outlined in SMPTE RP 431-2-2007.
//
// Assumed observer adapted white point:
//         CIE 1931 chromaticities:    x            y
//                                     0.32168      0.33767
//
// Viewing Environment:
//  Environment specified in SMPTE RP 431-2-2007
//



import "utilities";
import "transforms-common";
import "odt-transforms-common";



/* ----- ODT Parameters ------ */
const float OCES_PRI_2_XYZ_MAT[4][4] = RGBtoXYZ(ACES_PRI,1.0);

const float DISPGAMMA = 2.6; 



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
  /* --- Initialize a 3-element vector with input variables (OCES) --- */
    float oces[3] = { rIn, gIn, bIn};

  /* --- Apply hue-preserving tone scale with saturation preservation --- */
    float rgbPost[3] = odt_tonescale_fwd_f3( oces);

  /* --- Apply black point compensation --- */  
    float linearCV[3] = bpc_cinema_fwd( rgbPost);

  /* --- Convert to display primary encoding --- */
    // OCES RGB to CIE XYZ
    float XYZ[3] = mult_f3_f44( linearCV, OCES_PRI_2_XYZ_MAT);

  /* --- Limit XYZ values to P3D60 gamut --- */
    // Clip to P3 gamut using hue-preserving clip
    XYZ = huePreservingClip_to_p3d60( XYZ);
  
  /* --- Encode linear code values with transfer function --- */
    float outputCV[3];
    outputCV[0] = pow( 48./52.37 * XYZ[0], 1./DISPGAMMA);
    outputCV[1] = pow( 48./52.37 * XYZ[1], 1./DISPGAMMA);
    outputCV[2] = pow( 48./52.37 * XYZ[2], 1./DISPGAMMA);
    
  /* --- Cast outputCV to rOut, gOut, bOut --- */
    rOut = outputCV[0];
    gOut = outputCV[1];
    bOut = outputCV[2];
    aOut = aIn;
}