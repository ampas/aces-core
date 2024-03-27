/* ---- Signal encode/decode functions ---- */


// Forward monitor curve - moncurve_f() with gamma=2.4 and offset=0.055 matches the inverse EOTF found in IEC 61966-2-1:1999 (sRGB)
float moncurve_fwd( float x, 
                    float gamma, 
                    float offs )
{
    float y;
    const float fs = (( gamma - 1.0) / offs) * pow( offs * gamma / ( ( gamma - 1.0) * ( 1.0 + offs)), gamma);
    const float xb = offs / ( gamma - 1.0);
    if ( x >= xb) {
        y = pow( ( x + offs) / ( 1.0 + offs), gamma);
    } else {
        y = x * fs;
    }
    return y;
}

// Reverse monitor curve - moncurve_r() with gamma=2.4 and offset=0.055 matches the inverse EOTF found in IEC 61966-2-1:1999 (sRGB)
float moncurve_rev( float y, 
                    float gamma, 
                    float offs )
{
    float x;
    const float yb = pow( offs * gamma / ( ( gamma - 1.0) * ( 1.0 + offs)), gamma);
    const float rs = pow( ( gamma - 1.0) / offs, gamma - 1.0) * pow( ( 1.0 + offs) / gamma, gamma);
    if ( y >= yb) {
        x = ( 1.0 + offs) * pow( y, 1.0 / gamma) - offs;
    } else {
        x = y * rs;
    }
    return x;
}

float[3] moncurve_fwd_f3( float x[3], 
                          float gamma, 
                          float offs )
{
    float y[3];
    y[0] = moncurve_fwd( x[0], gamma, offs);
    y[1] = moncurve_fwd( x[1], gamma, offs);
    y[2] = moncurve_fwd( x[2], gamma, offs);
    return y;
}

float[3] moncurve_rev_f3( float y[3], 
                          float gamma, 
                          float offs )
{
    float x[3];
    x[0] = moncurve_rev( y[0], gamma, offs);
    x[1] = moncurve_rev( y[1], gamma, offs);
    x[2] = moncurve_rev( y[2], gamma, offs);
    return x;
}


// The forward OTF specified in Rec. ITU-R BT.1886
// L = a(max[(V+b),0])^g
float bt1886_fwd( float V, 
                  float gamma, 
                  float Lw, 
                  float Lb )
{
    float a = pow( pow( Lw, 1./gamma) - pow( Lb, 1./gamma), gamma);
    float b = pow( Lb, 1./gamma) / ( pow( Lw, 1./gamma) - pow( Lb, 1./gamma));
    float L = a * pow( max( V + b, 0.), gamma);
    return L;
}

// The reverse EOTF specified in Rec. ITU-R BT.1886
// L = a(max[(V+b),0])^g
float bt1886_rev( float L, 
                  float gamma, 
                  float Lw, 
                  float Lb )
{
    float a = pow( pow( Lw, 1./gamma) - pow( Lb, 1./gamma), gamma);
    float b = pow( Lb, 1./gamma) / ( pow( Lw, 1./gamma) - pow( Lb, 1./gamma));
    float V = pow( max( L / a, 0.), 1./gamma) - b;
    return V;
}

float[3] bt1886_fwd_f3( float V[3], 
                      float gamma, 
                      float Lw, 
                      float Lb )
{
    float L[3];
    L[0] = bt1886_fwd( V[0], gamma, Lw, Lb);
    L[1] = bt1886_fwd( V[1], gamma, Lw, Lb);
    L[2] = bt1886_fwd( V[2], gamma, Lw, Lb);
    return L;
}

float[3] bt1886_rev_f3( float L[3], 
                        float gamma, 
                        float Lw, 
                        float Lb )
{
    float V[3];
    V[0] = bt1886_rev( L[0], gamma, Lw, Lb);
    V[1] = bt1886_rev( L[1], gamma, Lw, Lb);
    V[2] = bt1886_rev( L[2], gamma, Lw, Lb);
    return V;
}


// SMPTE Range vs Full Range scaling formulas
float smpteRange_to_fullRange( float in )
{
    const float REFBLACK = (  64. / 1023.);
    const float REFWHITE = ( 940. / 1023.);

    return (( in - REFBLACK) / ( REFWHITE - REFBLACK));
}

float fullRange_to_smpteRange( float in )
{
    const float REFBLACK = (  64. / 1023.);
    const float REFWHITE = ( 940. / 1023.);

    return ( in * ( REFWHITE - REFBLACK) + REFBLACK );
}

float[3] smpteRange_to_fullRange_f3( float rgbIn[3] )
{
    float rgbOut[3];
    rgbOut[0] = smpteRange_to_fullRange( rgbIn[0]);
    rgbOut[1] = smpteRange_to_fullRange( rgbIn[1]);
    rgbOut[2] = smpteRange_to_fullRange( rgbIn[2]);

    return rgbOut;
}

float[3] fullRange_to_smpteRange_f3( float rgbIn[3] )
{
    float rgbOut[3];
    rgbOut[0] = fullRange_to_smpteRange( rgbIn[0]);
    rgbOut[1] = fullRange_to_smpteRange( rgbIn[1]);
    rgbOut[2] = fullRange_to_smpteRange( rgbIn[2]);

    return rgbOut;
}


// SMPTE 431-2 defines the DCDM color encoding equations. 
// Note: Here the 4095 12-bit scalar is not used since the output of CTL is 0-1.
// Decodes gamma and scaled XYZ' to linear XYZ
float[3] dcdm_decode_f3( float XYZp[3] )
{
    float XYZ[3];
    XYZ[0] = (52.37/48.0) * pow( XYZp[0], 2.6);  
    XYZ[1] = (52.37/48.0) * pow( XYZp[1], 2.6);  
    XYZ[2] = (52.37/48.0) * pow( XYZp[2], 2.6);  

    return XYZ;
}

// Encodes linear XYZ to XYZ' (gamma and scaled)
float[3] dcdm_encode_f3( float XYZ[3] )
{
    float XYZp[3];
    XYZp[0] = pow( (48./52.37) * XYZ[0], 1./2.6);
    XYZp[1] = pow( (48./52.37) * XYZ[1], 1./2.6);
    XYZp[2] = pow( (48./52.37) * XYZ[2], 1./2.6);

    return XYZp;
}


// Base functions from SMPTE ST 2084-2014

// Constants from SMPTE ST 2084-2014
const float pq_m1 = 0.1593017578125; // ( 2610.0 / 4096.0 ) / 4.0;
const float pq_m2 = 78.84375; // ( 2523.0 / 4096.0 ) * 128.0;
const float pq_c1 = 0.8359375; // 3424.0 / 4096.0 or pq_c3 - pq_c2 + 1.0;
const float pq_c2 = 18.8515625; // ( 2413.0 / 4096.0 ) * 32.0;
const float pq_c3 = 18.6875; // ( 2392.0 / 4096.0 ) * 32.0;

const float pq_C = 10000.0;

// Converts from the non-linear perceptually quantized space to linear cd/m^2
// Note that this is in float, and assumes normalization from 0 - 1
// (0 - pq_C for linear) and does not handle the integer coding in the Annex 
// sections of SMPTE ST 2084-2014
float ST2084_2_Y( float N )
{
  // Note that this does NOT handle any of the signal range
  // considerations from 2084 - this assumes full range (0 - 1)
  float Np = pow( N, 1.0 / pq_m2 );
  float L = Np - pq_c1;
  if ( L < 0.0 )
    L = 0.0;
  L = L / ( pq_c2 - pq_c3 * Np );
  L = pow( L, 1.0 / pq_m1 );
  return L * pq_C; // returns cd/m^2
}

// Converts from linear cd/m^2 to the non-linear perceptually quantized space
// Note that this is in float, and assumes normalization from 0 - 1
// (0 - pq_C for linear) and does not handle the integer coding in the Annex 
// sections of SMPTE ST 2084-2014
float Y_2_ST2084( float C )
{
  // Note that this does NOT handle any of the signal range
  // considerations from 2084 - this returns full range (0 - 1)
  float L = C / pq_C;
  float Lm = pow( L, pq_m1 );
  float N = ( pq_c1 + pq_c2 * Lm ) / ( 1.0 + pq_c3 * Lm );
  N = pow( N, pq_m2 );
  return N;
}

// converts from linear cd/m^2 to PQ code values
float[3] Y_2_ST2084_f3( float in[3] )
{  
  float out[3];
  out[0] = Y_2_ST2084( in[0]);
  out[1] = Y_2_ST2084( in[1]);
  out[2] = Y_2_ST2084( in[2]);

  return out;
}

// converts from PQ code values to linear cd/m^2
float[3] ST2084_2_Y_f3( float in[3] )
{
  float out[3];
  out[0] = ST2084_2_Y( in[0]);
  out[1] = ST2084_2_Y( in[1]);
  out[2] = ST2084_2_Y( in[2]);

  return out;
}


// Conversion of PQ signal to HLG, as detailed in Section 7 of ITU-R BT.2390-0
float[3] ST2084_2_HLG_1000nits_f3( float PQ[3] ) 
{
    // ST.2084 EOTF (non-linear PQ to display light)
    float displayLinear[3] = ST2084_2_Y_f3( PQ);

    // HLG Inverse EOTF (i.e. HLG inverse OOTF followed by the HLG OETF)
    // HLG Inverse OOTF (display linear to scene linear)
    float Y_d = 0.2627*displayLinear[0] + 0.6780*displayLinear[1] + 0.0593*displayLinear[2];
    const float L_w = 1000.;
    const float L_b = 0.;
    const float alpha = (L_w-L_b);
    const float beta = L_b;
    const float gamma = 1.2;
    
    float sceneLinear[3];
    if (Y_d == 0.) { 
        /* This case is to protect against pow(0,-N)=Inf error. The ITU document
        does not offer a recommendation for this corner case. There may be a 
        better way to handle this, but for now, this works. 
        */ 
        sceneLinear[0] = 0.;
        sceneLinear[1] = 0.;
        sceneLinear[2] = 0.;        
    } else {
        sceneLinear[0] = pow( (Y_d-beta)/alpha, (1.-gamma)/gamma) * ((displayLinear[0]-beta)/alpha);
        sceneLinear[1] = pow( (Y_d-beta)/alpha, (1.-gamma)/gamma) * ((displayLinear[1]-beta)/alpha);
        sceneLinear[2] = pow( (Y_d-beta)/alpha, (1.-gamma)/gamma) * ((displayLinear[2]-beta)/alpha);
    }

    // HLG OETF (scene linear to non-linear signal value)
    const float a = 0.17883277;
    const float b = 0.28466892; // 1.-4.*a;
    const float c = 0.55991073; // 0.5-a*log(4.*a);

    float HLG[3];
    if (sceneLinear[0] <= 1./12) {
        HLG[0] = sqrt(3.*sceneLinear[0]);
    } else {
        HLG[0] = a*log(12.*sceneLinear[0]-b)+c;
    }
    if (sceneLinear[1] <= 1./12) {
        HLG[1] = sqrt(3.*sceneLinear[1]);
    } else {
        HLG[1] = a*log(12.*sceneLinear[1]-b)+c;
    }
    if (sceneLinear[2] <= 1./12) {
        HLG[2] = sqrt(3.*sceneLinear[2]);
    } else {
        HLG[2] = a*log(12.*sceneLinear[2]-b)+c;
    }

    return HLG;
}


// Conversion of HLG to PQ signal, as detailed in Section 7 of ITU-R BT.2390-0
float[3] HLG_2_ST2084_1000nits_f3( float HLG[3]) 
{
    const float a = 0.17883277;
    const float b = 0.28466892; // 1.-4.*a;
    const float c = 0.55991073; // 0.5-a*log(4.*a);

    const float L_w = 1000.;
    const float L_b = 0.;
    const float alpha = (L_w-L_b);
    const float beta = L_b;
    const float gamma = 1.2;

    // HLG EOTF (non-linear signal value to display linear)
    // HLG to scene-linear
    float sceneLinear[3];
    if ( HLG[0] >= 0. && HLG[0] <= 0.5) {
        sceneLinear[0] = pow(HLG[0],2.)/3.;
    } else {
        sceneLinear[0] = (exp((HLG[0]-c)/a)+b)/12.;
    }        
    if ( HLG[1] >= 0. && HLG[1] <= 0.5) {
        sceneLinear[1] = pow(HLG[1],2.)/3.;
    } else {
        sceneLinear[1] = (exp((HLG[1]-c)/a)+b)/12.;
    }        
    if ( HLG[2] >= 0. && HLG[2] <= 0.5) {
        sceneLinear[2] = pow(HLG[2],2.)/3.;
    } else {
        sceneLinear[2] = (exp((HLG[2]-c)/a)+b)/12.;
    }        
    
    float Y_s = 0.2627*sceneLinear[0] + 0.6780*sceneLinear[1] + 0.0593*sceneLinear[2];

    // Scene-linear to display-linear
    float displayLinear[3];
    displayLinear[0] = alpha * pow( Y_s, gamma-1.) * sceneLinear[0] + beta;
    displayLinear[1] = alpha * pow( Y_s, gamma-1.) * sceneLinear[1] + beta;
    displayLinear[2] = alpha * pow( Y_s, gamma-1.) * sceneLinear[2] + beta;
        
    // ST.2084 Inverse EOTF
    float PQ[3] = Y_2_ST2084_f3( displayLinear);

    return PQ;
}