// <ACEStransformID>urn:ampas:aces:transformId:v1.5:IDT.Canon.CanonLog2_BT2020_D55.a1.v2</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - Canon CanonLog2 BT2020 (Daylight)</ACESuserName>

/* ********************************************* */
//
// Input Transform for Canon Log 2 / BT.2020
// ACES Version: 1.0
// IDT Version: 2.0  2021/08/23
// Copyright(c) 2021 Canon Inc. All Rights Reserved.
//
/* ********************************************* */
//
// Illuminant   : Daylight (CIE Illuminant D55)
// Color Gamut  : BT.2020
// Gamma        : Canon Log 2
// Color Matrix : Neutral
//
// [ NOTE ]
//
// This Input Transform is defined for images those were shot
//  under daylight and general illumination sources except Tungsten.
//
/* ********************************************* */


/* ============ SUBFUNCTIONS ============ */
/* CanonLog2 to Linear */
float CanonLog2_to_linear (
	float clog2
)
{
	float out;
	if(clog2 < 0.092864125)
		out = -( pow( 10, ( 0.092864125 - clog2 ) / 0.24136077 ) - 1 ) / 87.099375;
	else
		out = ( pow( 10, ( clog2 - 0.092864125 ) / 0.24136077 ) - 1 ) / 87.099375;

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
	// CanonLog2 to linear 
	float lin[3];
	lin[0] = 0.9 * CanonLog2_to_linear( rIn );
	lin[1] = 0.9 * CanonLog2_to_linear( gIn );
	lin[2] = 0.9 * CanonLog2_to_linear( bIn );

	// ACES conversion matrix
	float aces[3];

	aces[0] =   0.678891151 * lin[0] + 0.158868422 * lin[1] + 0.162240427 * lin[2];
	aces[1] =   0.045570831 * lin[0] + 0.860712772 * lin[1] + 0.093716397 * lin[2];
	aces[2] = - 0.000485710 * lin[0] + 0.025060196 * lin[1] + 0.975425515 * lin[2];

	rOut = aces[0];
	gOut = aces[1];
	bOut = aces[2];
	aOut = aIn;
}

/* ============ End of Input Transform ============ */
