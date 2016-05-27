
// <ACEStransformID>ACEScsc.ACES_to_ACESproxy12i.a1.0.1</ACEStransformID>
// <ACESuserName>ACES2065-1 to ACESproxy</ACESuserName>

//
// ACES Color Space Conversion - ACES to ACESproxy (12-bit)
//
// converts ACES2065-1 (AP0 w/ linear encoding) to 
//          ACESproxy (AP1 w/ ACESproxy encoding)
//
// This transform follows the formulas from S-2013-001. Please refer to the 
// aforementioned document for the exact math and a table of reference values.
//
 
// *-*-*-*-*-*-*-*-*
// ACESproxy is intended to be a transient encoding used only for signal 
// transmission in systems limited to 10- or 12-bit video signals.
// ACESproxy is not intended for interchange, mastering finals, or archiving 
// and as such should NOT be written into a container file in actual 
// implementations! 
// *-*-*-*-*-*-*-*-*



import "ACESlib.Transform_Common";



const float StepsPerStop = 200.;
const float MidCVoffset = 1700.;
const int CVmin = 256;
const int CVmax = 3760;



int lin_to_ACESproxy
( 
  input varying float in,
  input varying float StepsPerStop,
  input varying float MidCVoffset,
  input varying float CVmin,
  input varying float CVmax
)
{
  if (in <= pow(2., -9.72))
    return CVmin;
  else
    return max( CVmin, min( CVmax, round( (log2(in) + 2.5) * StepsPerStop + MidCVoffset)));
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

	// Pre-clamp: This clamp serves to avoid large negative ACES values from potentially folding over to large positive values when multiplied through the AP0-to-AP1 matrix. This is usually not an issue in real camera images.
    ACES = clamp_f3( ACES, 0.0, HALF_POS_INF);

    float ACESproxy_lin[3] = mult_f3_f44( ACES, AP0_2_AP1_MAT);

    int ACESproxy[3];
    ACESproxy[0] = lin_to_ACESproxy( ACESproxy_lin[0], StepsPerStop, MidCVoffset, CVmin, CVmax);
    ACESproxy[1] = lin_to_ACESproxy( ACESproxy_lin[1], StepsPerStop, MidCVoffset, CVmin, CVmax);
    ACESproxy[2] = lin_to_ACESproxy( ACESproxy_lin[2], StepsPerStop, MidCVoffset, CVmin, CVmax);

    // Prepare output values based on application bit depth handling
    //  NOTE: This step is required for use with ctlrender. 
    //  ctlrender scales the output values from the CTL transformation that are 
    //  between 0.0-1.0 to the bit depth of the output file. For the reference //  images provided with the ACES Release, the ACESproxy files are written 
    //  into a 16-bit TIFF file, and so will be multiplied by 65535 when 
    //  writing ACES-to-ACESproxy (EXR to TIFF16). Therefore, it is important to 
    //  scale the output of the transform into the range 0.0-1.0 
    rOut = ACESproxy[0] / 4095.;
    gOut = ACESproxy[1] / 4095.;
    bOut = ACESproxy[2] / 4095.;
    aOut = aIn;
}