
// <ACEStransformID>urn:ampas:aces:transformId:v2.0:Lib.Utilities.a1.v1</ACEStransformID>
// <ACESuserName>Utilities</ACESuserName>

//
// Generic functions that may be useful for writing CTL programs
//

float min( float a, 
           float b )
{
    if (a < b)
        return a;
    else
        return b;
}

float max( float a, 
           float b )
{
    if (a > b)
        return a;
    else
        return b;
}

float min_f3( float a[3] )
{
    return min( a[0], min( a[1], a[2]));
}

float max_f3( float a[3] )
{
    return max( a[0], max( a[1], a[2]));
}

float clip( float v )
{
    return min(v, 1.0);
}

float[3] clip_f3( float in[3] )
{
    float out[3];
    out[0] = clip( in[0]);
    out[1] = clip( in[1]);
    out[2] = clip( in[2]);

    return out;
}

float clamp( float in, 
             float clampMin, 
             float clampMax )
{
    // Note: Numeric constants can be used in place of a min or max value (i.e. 
    // use HALF_NEG_INF in place of clampMin or HALF_POS_INF in place of clampMax)

    return max( clampMin, min(in, clampMax));
}

float[3] clamp_f3( float in[3], 
                   float clampMin, 
                   float clampMax )
{
    // Note: Numeric constants can be used in place of a min or max value (i.e. 
    // use HALF_NEG_INF in place of clampMin or HALF_POS_INF in place of clampMax)

    float out[3];
    out[0] = clamp( in[0], clampMin, clampMax);
    out[1] = clamp( in[1], clampMin, clampMax);
    out[2] = clamp( in[2], clampMin, clampMax);
  
    return out;
}

float[3] add_f_f3( float a, 
                   float b[3] )
{
    float out[3];
    out[0] = a + b[0];
    out[1] = a + b[1];
    out[2] = a + b[2];
    return out;
}

float[3] pow_f3( float a[3], 
                 float b )
{
    float out[3];
    out[0] = pow(a[0], b);
    out[1] = pow(a[1], b);
    out[2] = pow(a[2], b);
    return out;
}

float[3] pow10_f3( float a[3] )
{
    float out[3];
    out[0] = pow10(a[0]);
    out[1] = pow10(a[1]);
    out[2] = pow10(a[2]);
    return out;
}

float[3] log10_f3( float a[3] )
{
    float out[3];
    out[0] = log10(a[0]);
    out[1] = log10(a[1]);
    out[2] = log10(a[2]);
    return out;
}

float round( float x )
{
    int x1;

    if (x < 0.0)
        x1 = x - 0.5;
    else
        x1 = x + 0.5;

    return x1;
}

float log2( float x )
{
    return log(x) / log(2.);
}

int sign( float x )
{
    // Signum function:
    //  sign(X) returns 1 if the element is greater than zero, 0 if it equals zero 
    //  and -1 if it is less than zero

    int y;
    if (x < 0) { 
        y = -1;
    } else if (x > 0) {
        y = 1;
    } else {
        y = 0;
    }

    return y;
}

float[3] sign_f3( float in[3] ) 
{
	float return_val[3] = { sign(in[0]), 
							sign(in[1]), 
							sign(in[2]) };
	return return_val;
} 

// returns a value combining the magnitude of x with the sign of y
float copysign( float x, 
                float y )
{
	return sign(y) * fabs(x);
}

void print_f2( float m[2] )
{
    print( m[0], ",\t", m[1], "\n");
}

void print_f3( float m[3] )
{
    print( m[0], ",\t", m[1], ",\t", m[2], "\n");
}

void print_f33( float m[3][3] )
{
    print( "{ {", m[0][0], ",\t", m[0][1], ",\t", m[0][2], "},\n");
    print( "  {", m[1][0], ",\t", m[1][1], ",\t", m[1][2], "},\n");
    print( "  {", m[2][0], ",\t", m[2][1], ",\t", m[2][2], "} };\n");
}

void print_f44( float m[4][4] )
{
    print( "{ { ", m[0][0], ",\t", m[0][1], ",\t", m[0][2], ",\t", m[0][3], "},\n");
    print( "  { ", m[1][0], ",\t", m[1][1], ",\t", m[1][2], ",\t", m[1][3], "},\n");
    print( "  { ", m[2][0], ",\t", m[2][1], ",\t", m[2][2], ",\t", m[2][3], "},\n");
    print( "  { ", m[3][0], ",\t", m[3][1], ",\t", m[3][2], ",\t", m[3][3], "} };\n");
}

float ceil( float a )
{
	return floor( a + 1.0 );
}

float[3] vector_dot( float m[3][3], float v[3] )
{
	float r[3];
	r[0] = m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2];
	r[1] = m[1][0] * v[0] + m[1][1] * v[1] + m[1][2] * v[2];
	r[2] = m[2][0] * v[0] + m[2][1] * v[1] + m[2][2] * v[2];
	return r;
}

// linear interpolation between two values a & b with theß bias t
float lerp( float a, 
            float b, 
            float t )
{
    return a + t * (b - a);
}

float radians_to_degrees( float radians )
{
	return radians * 180.0 / M_PI;
}

float degrees_to_radians( float degrees )
{
	return degrees / 180.0 * M_PI;
}

// safe power function to avoid NaNs or Infs when taking a fractional power of a negative base
// this initially returned -pow(abs(b), e) for negative b but this ended up producing 
// undesirable results in some cases, so now it just returns 0.0 instead
float spow( float base, 
            float exponent )
{ 
	if (base < 0.0 && exponent != floor(exponent))
	{
		return 0.0;
	} else {
		return pow(base, exponent);
	}

}

float[3] spow_f3( float base[3], 
                  float exponent )
{
	float return_val[3] = { spow(base[0], exponent), 
							spow(base[1], exponent), 
							spow(base[2], exponent) };
	return return_val;
} 

float[3] fabs_f3( float in[3] )
{
	float return_val[3] = { fabs( in[0]),
							fabs( in[1]),
							fabs( in[2]) };
	return return_val;
}

// safe divide function - return 0 if a divide by zero
float sdiv( float a, 
            float b )
{
	if (b == 0.0)
	{
		return 0.0;
	} else {
		return a / b;
	}
}

float[3][3] RGBtoXYZ_f33( Chromaticities C, 
                          float Y )
{
    // X and Z values of RGB value (1, 1, 1), or "white"
    float X = C.white[0] * Y / C.white[1];
    float Z = (1. - C.white[0] - C.white[1]) * Y / C.white[1];

    // Scale factors for matrix rows
    float d = C.red[0] * (C.blue[1]  - C.green[1]) + 
             C.blue[0] * (C.green[1] - C.red[1]) +
            C.green[0] * (C.red[1]   - C.blue[1]);

    float Sr = (          X * (C.blue[1] - C.green[1]) -
                 C.green[0] * (Y * (C.blue[1] - 1) + C.blue[1] * (X + Z)) +
                  C.blue[0] * (Y * (C.green[1] - 1) + C.green[1] * (X + Z))
               ) / d;

    float Sg = (          X * (C.red[1] - C.blue[1]) +
                   C.red[0] * (Y * (C.blue[1] - 1) + C.blue[1] * (X + Z)) - 
                  C.blue[0] * (Y * (C.red[1] - 1) + C.red[1] * (X + Z))
               ) / d;

    float Sb = (          X * (C.green[1] - C.red[1]) -
                   C.red[0] * (Y * (C.green[1] - 1) + C.green[1] * (X + Z)) +
                 C.green[0] * (Y * (C.red[1] - 1) + C.red[1] * (X + Z))
               ) / d;

    // Assemble the matrix
    float M[3][3];

    M[0][0] = Sr * C.red[0];
    M[0][1] = Sr * C.red[1];
    M[0][2] = Sr * (1. - C.red[0] - C.red[1]);

    M[1][0] = Sg * C.green[0];
    M[1][1] = Sg * C.green[1];
    M[1][2] = Sg * (1. - C.green[0] - C.green[1]);

    M[2][0] = Sb * C.blue[0];
    M[2][1] = Sb * C.blue[1];
    M[2][2] = Sb * (1. - C.blue[0] - C.blue[1]);

    return M;
}

float[3][3] XYZtoRGB_f33( Chromaticities C, 
                          float Y )
{
    return invert_f33( RGBtoXYZ_f33(C, Y) );
}

float[3] HSV_to_RGB( float HSV[3] )
{
    float C = HSV[2] * HSV[1];
    float X = C * (1. - fabs( fmod(HSV[0] * 6., 2.) - 1.));
    float m = HSV[2] - C;
    
    float RGB[3];
    if (HSV[0] < 1./6.) {
        RGB[0] = C; RGB[1] = X; RGB[2] = 0.;
    } else if (HSV[0] < 2./6.) {
        RGB[0] = X; RGB[1] = C; RGB[2] = 0.;
    } else if (HSV[0] < 3./6.) {
        RGB[0] = 0.; RGB[1] = C; RGB[2] = X;
    } else if (HSV[0] < 4./6.) {
        RGB[0] = 0.; RGB[1] = X; RGB[2] = C;
    } else if (HSV[0] < 5./6.) {
        RGB[0] = X; RGB[1] = 0.; RGB[2] = C;
    } else {
        RGB[0] = C; RGB[1] = 0.; RGB[2] = X;
    }
    RGB = add_f_f3( m, RGB);
    return RGB;
}

// smooth minimum of a and b
float smin(float a, float b, float s)
{
    float h = max(s - fabs(a - b), 0.0) / s;
    return min(a, b) - h * h * h * s * (1.0 / 6.0);
}

bool f2_equal_to_tolerance( float a[2], 
                            float b[2], 
                            float tolerance ) 
{
    return ( fabs(a[0]-b[0]) <= tolerance && fabs(a[1]-b[1]) <= tolerance );
}
