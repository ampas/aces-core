// <ACEStransformID>urn:ampas:aces:transformId:v1.5:IDT.Canon.CanonLog3_CinemaGamut_D55.a1.v2</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - Canon CanonLog3 CinemaGamut (Daylight)</ACESuserName>

/* ********************************************* */
//
// Input Transform for Canon Log 3 / Cinema Gamut
// ACES Version: 1.0
// IDT Version: 2.0  2021/08/23
// Copyright(c) 2021 Canon Inc. All Rights Reserved.
//
/* ********************************************* */
//
// Illuminant   : Daylight (CIE Illuminant D55)
// Color Gamut  : Cinema Gamut
// Gamma        : Canon Log 3
// Color Matrix : Neutral
//
// [ NOTE ]
//
// This Input Transform is defined for images those were shot
//  under daylight and general illumination sources except Tungsten.
//
/* ********************************************* */


/* ============ SUBFUNCTIONS ============ */
/* CanonLog3 to Linear */
float CanonLog3_to_linear (
	float clog3
)
{
	float out;
	if(clog3 < 0.097465473)
		out = -( pow( 10, ( 0.12783901 - clog3 ) / 0.36726845 ) - 1 ) / 14.98325;
	else if(clog3 <= 0.15277891)
		out = ( clog3 - 0.12512219 ) / 1.9754798;
	else
		out = ( pow( 10, ( clog3 - 0.12240537 ) / 0.36726845 ) - 1 ) / 14.98325;

	return out;
}


/* ============ Main Algorithm ============ */
void main
(   input varying float rIn,
    input varying float gIn,
    input varying float bIn,
    input varying float aIn,
    output varying float rOut,
    output varying float gOut,
    output varying float bOut,
    output varying float aOut
)
{
	// CanonLog3 to linear 
	float lin[3];
	lin[0] = 0.9 * CanonLog3_to_linear( rIn );
	lin[1] = 0.9 * CanonLog3_to_linear( gIn );
	lin[2] = 0.9 * CanonLog3_to_linear( bIn );

	// ACES conversion matrix
	float aces[3];

	aces[0] =   0.763064455 * lin[0] + 0.149021161 * lin[1] + 0.087914384 * lin[2];
	aces[1] =   0.003657457 * lin[0] + 1.106960380 * lin[1] - 0.110617837 * lin[2];
	aces[2] = - 0.009407794 * lin[0] - 0.218383305 * lin[1] + 1.227791099 * lin[2];

	rOut = aces[0];
	gOut = aces[1];
	bOut = aces[2];
	aOut = aIn;
}

/* ============ End of Input Transform ============ */
