import "common";

namespace testStdLibrary 
{

// Test the functions in the CTL standard library.  We verify that
// the right functions are called, but don't try to exhaustively test
// the underlying implementation (C++ or other) of the functions.

bool
equalWithAbsErr_f3 (float a[3], float b[3], float e)
{
    int i = 0;

    while (i < 3)
    {
	if (!equalWithAbsErr (a[i], b[i], e))
	    return false;

	i = i + 1;
    }

    return true;
}


bool
equalWithAbsErr_f33 (float a[3][3], float b[3][3], float e)
{
    int i = 0;

    while (i < 3)
    {
	int j = 0;

	while (j < 3)
	{
	    if (!equalWithAbsErr (a[i][j], b[i][j], e))
		return false;

	    j = j + 1;
	}

	i = i + 1;
    }

    return true;
}


bool
equalWithAbsErr_f44 (float a[4][4], float b[4][4], float e)
{
    int i = 0;

    while (i < 4)
    {
	int j = 0;

	while (j < 4)
	{
	    if (!equalWithAbsErr (a[i][j], b[i][j], e))
		return false;

	    j = j + 1;
	}

	i = i + 1;
    }

    return true;
}


void
testEqualWithAbsErr()
{
    assert (equalWithAbsErr (1.0, 0.995, 0.01));
    assert (equalWithAbsErr (1.0, 1.005, 0.01));
    assert (!equalWithAbsErr (1.0, 0.985, 0.01));
    assert (!equalWithAbsErr (1.0, 1.015, 0.01));

    {
	float M1[3][3] = {{1, 0, 0},
			  {0, 1, 0},
			  {0, 0, 1}};

	float M2[3][3] = {{1, 0, 0},
			  {0, 1, 0},
			  {0, 0, 1.01}};

	assert (equalWithAbsErr_f33 (M1, M1, 0));
	assert (equalWithAbsErr_f33 (M2, M2, 0));
	assert (!equalWithAbsErr_f33 (M1, M2, 0));
	assert (equalWithAbsErr_f33 (M1, M2, 0.1));
    }

    {
	float M1[4][4] = {{1, 0, 0, 0},
			  {0, 1, 0, 0},
			  {0, 0, 1, 0},
			  {0, 0, 0, 1}};

	float M2[4][4] = {{1, 0, 0, 0},
			  {0, 1, 0, 0},
			  {0, 0, 1, 0},
			  {0, 0, 0, 1.01}};

	assert (equalWithAbsErr_f44 (M1, M1, 0));
	assert (equalWithAbsErr_f44 (M2, M2, 0));
	assert (!equalWithAbsErr_f44 (M1, M2, 0));
	assert (equalWithAbsErr_f44 (M1, M2, 0.1));
    }

    {
	float v1[3] = {1, 2, 3};
	float v2[3] = {1, 2, 3.01};

	assert (equalWithAbsErr_f3 (v1, v1, 0));
	assert (equalWithAbsErr_f3 (v2, v2, 0));

	assert (!equalWithAbsErr_f3 (v1, v2, 0));
	assert (equalWithAbsErr_f3 (v1, v2, 0.1));
    }
}


void
setToZero (output float f)
{
    f = 0;
}


void
testChangeBuiltinConstants ()
{
    return;

    // verify that it is not possible to
    // change the values of built-in constants

    @error20 FLT_MAX = 0;
    @error20 HALF_MAX = 0;
    @error20 INT_MAX = 0;
    @error20 UINT_MAX = 0;
    @error26 setToZero (M_PI);
}


void
testBuiltinConstants ()
{
    assert (equalWithAbsErr (M_E,  2.718281, 0.000001));
    assert (equalWithAbsErr (M_PI, 3.141593, 0.000001));

    assert (FLT_MAX > 1e+38);
    assert (FLT_MIN > 0 && FLT_MIN < 1e-37);
    assert (FLT_EPSILON > 0 && FLT_EPSILON < 1e-6);

    assert (HALF_MAX >= 65000);
    assert (HALF_MIN > 0 && HALF_MIN < 1e-4);
    assert (HALF_EPSILON > 0 && HALF_EPSILON < 1e-3);

    assert (INT_MAX >= 2147483647);
    assert (INT_MIN <= -2147483648);

    assert (UINT_MAX >= 4294967295);
}


void
testClassification ()
{
    assert (!isfinite_f (FLT_POS_INF));
    assert (!isnormal_f (FLT_POS_INF));
    assert ( isinf_f    (FLT_POS_INF));
    assert (!isnan_f    (FLT_POS_INF));
    assert (FLT_POS_INF > 0);

    assert (!isfinite_f (FLT_NEG_INF));
    assert (!isnormal_f (FLT_NEG_INF));
    assert ( isinf_f    (FLT_NEG_INF));
    assert (!isnan_f    (FLT_NEG_INF));
    assert (FLT_NEG_INF < 0);

    assert (!isfinite_f (FLT_NAN));
    assert (!isnormal_f (FLT_NAN));
    assert (!isinf_f    (FLT_NAN));
    assert ( isnan_f    (FLT_NAN));

    assert ( isfinite_f (0.0));
    assert (!isnormal_f (0.0));
    assert (!isinf_f    (0.0));
    assert (!isnan_f    (0.0));

    assert ( isfinite_f (1.0));
    assert ( isnormal_f (1.0));
    assert (!isinf_f    (1.0));
    assert (!isnan_f    (1.0));

    assert (!isfinite_h (HALF_POS_INF));
    assert (!isnormal_h (HALF_POS_INF));
    assert ( isinf_h    (HALF_POS_INF));
    assert (!isnan_h    (HALF_POS_INF));
    assert (HALF_POS_INF > 0);

    assert (!isfinite_h (HALF_NEG_INF));
    assert (!isnormal_h (HALF_NEG_INF));
    assert ( isinf_h    (HALF_NEG_INF));
    assert (!isnan_h    (HALF_NEG_INF));
    assert (HALF_NEG_INF < 0);

    assert (!isfinite_h (HALF_NAN));
    assert (!isnormal_h (HALF_NAN));
    assert (!isinf_h    (HALF_NAN));
    assert ( isnan_h    (HALF_NAN));

    assert ( isfinite_h (0.0h));
    assert (!isnormal_h (0.0h));
    assert (!isinf_h    (0.0h));
    assert (!isnan_h    (0.0h));

    assert ( isfinite_h (1.0h));
    assert ( isnormal_h (1.0h));
    assert (!isinf_h    (1.0h));
    assert (!isnan_h    (1.0h));
}


void
testMathFunctions ()
{
    print ("Testing cos(), exp(), sqrt() etc.\n");

    const float err = 0.00001;
    const float err_h = 0.002;
    const float pi = 3.1415926535897931;
    const float e = 2.71828182845905;

    assert (equalWithAbsErr (acos (-1), pi, err));
    assert (equalWithAbsErr (acos (0), pi/2, err));
    assert (equalWithAbsErr (acos (1), 0, err));

    assert (equalWithAbsErr (asin (-1), -pi/2, err));
    assert (equalWithAbsErr (asin (0), 0, err));
    assert (equalWithAbsErr (asin (1), pi/2, err));

    assert (equalWithAbsErr (atan (-1), -pi/4, err));
    assert (equalWithAbsErr (atan (0), 0, err));
    assert (equalWithAbsErr (atan (1), pi/4, err));

    assert (equalWithAbsErr (atan2 (-2, 2), -pi/4, err));
    assert (equalWithAbsErr (atan2 (0, 2), 0, err));
    assert (equalWithAbsErr (atan2 (2, 2), pi/4, err));

    assert (equalWithAbsErr (cos (-pi/2), 0, err));
    assert (equalWithAbsErr (cos (0), 1, err));
    assert (equalWithAbsErr (cos (pi/2), 0, err));

    assert (equalWithAbsErr (sin (-pi/2), -1, err));
    assert (equalWithAbsErr (sin (0), 0, err));
    assert (equalWithAbsErr (sin (pi/2), 1, err));

    assert (equalWithAbsErr (tan (-pi/4), -1, err));
    assert (equalWithAbsErr (tan (0), 0, err));
    assert (equalWithAbsErr (tan (pi/4), 1, err));

    assert (equalWithAbsErr (cosh (-1), (e + 1/e) / 2, err));
    assert (equalWithAbsErr (cosh (0), 1, err));
    assert (equalWithAbsErr (cosh (1), (e + 1/e) / 2, err));

    assert (equalWithAbsErr (sinh (-1), (1/e - e) / 2, err));
    assert (equalWithAbsErr (sinh (0), 0, err));
    assert (equalWithAbsErr (sinh (1), (e - 1/e) / 2, err));

    assert (equalWithAbsErr (tanh (-1), sinh (-1) / cosh (-1), err));
    assert (equalWithAbsErr (tanh (0), 0, err));
    assert (equalWithAbsErr (tanh (1), sinh (1) / cosh (1), err));

    assert (equalWithAbsErr (exp (-1), 1/e, err));
    assert (equalWithAbsErr (exp (0), 1, err));
    assert (equalWithAbsErr (exp (1), e, err));

    assert (equalWithAbsErr (exp_h (-1), 1/e, err_h));
    assert (equalWithAbsErr (exp_h (0), 1, err_h));
    assert (equalWithAbsErr (exp_h (1), e, err_h));

    assert (equalWithAbsErr (log (1/e), -1, err));
    assert (equalWithAbsErr (log (1), 0, err));
    assert (equalWithAbsErr (log (e), 1, err));

    assert (equalWithAbsErr (log_h (1/e), -1, err_h));
    assert (equalWithAbsErr (log_h (1), 0, err_h));
    assert (equalWithAbsErr (log_h (e), 1, err_h));

    assert (equalWithAbsErr (log10 (0.1), -1, err));
    assert (equalWithAbsErr (log10 (1), 0, err));
    assert (equalWithAbsErr (log10 (10), 1, err));

    assert (equalWithAbsErr (log10_h (0.1), -1, err_h));
    assert (equalWithAbsErr (log10_h (1), 0, err_h));
    assert (equalWithAbsErr (log10_h (10), 1, err_h));

    assert (equalWithAbsErr (pow (2, 1), 2, err));
    assert (equalWithAbsErr (pow (2, 3), 8, err));
    assert (equalWithAbsErr (pow (4, -1), 0.25, err));

    assert (equalWithAbsErr (pow_h (2, 1), 2, err_h));
    assert (equalWithAbsErr (pow_h (2, 3), 8, err_h));
    assert (equalWithAbsErr (pow_h (4, -1), 0.25, err_h));

    assert (equalWithAbsErr (pow10 (-1), 0.1, err));
    assert (equalWithAbsErr (pow10 (0), 1.0, err));
    assert (equalWithAbsErr (pow10 (1), 10.0, err));

    assert (equalWithAbsErr (pow10_h (-1), 0.1, err_h));
    assert (equalWithAbsErr (pow10_h (0), 1.0, err_h));
    assert (equalWithAbsErr (pow10_h (1), 10.0, err_h));

    assert (equalWithAbsErr (sqrt (1), 1, 0));
    assert (equalWithAbsErr (sqrt (2), 1.414213562, err));
    assert (equalWithAbsErr (sqrt (4), 2, 0));

    assert (equalWithAbsErr (fabs (-1.5), 1.5, 0));
    assert (equalWithAbsErr (fabs (0), 0, 0));
    assert (equalWithAbsErr (fabs (1.5), 1.5, 0));

    assert (equalWithAbsErr (floor (-1.5), -2, 0));
    assert (equalWithAbsErr (floor (0), 0, 0));
    assert (equalWithAbsErr (floor (1.5), 1, 0));

    assert (equalWithAbsErr (fmod (6, 3), 0, err));
    assert (equalWithAbsErr (fmod (8, 3), 2, err));
    assert (equalWithAbsErr (fmod (5, 2.5), 0, err));

    assert (equalWithAbsErr (hypot (3, 0), 3, err));
    assert (equalWithAbsErr (hypot (3, 4), 5, err));
    assert (equalWithAbsErr (hypot (0, 4), 4, err));

    print ("ok\n");
}


void
testVecMatrixFunctions ()
{
    print ("Testing operations on 3x3 and 4x4 matrices and 3D vectors\n");

    {
	const float M1[3][3] = {{1, 0, 0},
				{0, 1, 0},
				{0, 0, 1}};

	const float M2[3][3] = {{2, 0, 0},
				{0, 2, 0},
				{0, 0, 2}};

	const float M3[3][3] = {{1, 2, 3},
				{1, 2, 3},
				{1, 2, 1}};

	{
	    float A[3][3] = mult_f33_f33 (M1, M1);
	    assert (equalWithAbsErr_f33 (A, M1, 0));
	}

	{
	    const float B[3][3] = {{2, 4, 6},
				   {2, 4, 6},
				   {2, 4, 2}};

	    float A[3][3] = mult_f33_f33 (M2, M3);
	    assert (equalWithAbsErr_f33 (A, B, 0));
	}

	{
	    float A[3][3] = mult_f_f33 (2, M1);
	    assert (equalWithAbsErr_f33 (A, M2, 0));
	}

	{
	    const float B[3][3] = {{3, 6, 9},
				   {3, 6, 9},
				   {3, 6, 3}};

	    float A[3][3] = mult_f_f33 (3, M3);
	    assert (equalWithAbsErr_f33 (A, B, 0));
	}

	{
	    float A[3][3] = add_f33_f33 (M1, M1);
	    assert (equalWithAbsErr_f33 (A, M2, 0));
	}

	{
	    const float B[3][3] = {{3, 2, 3},
				   {1, 4, 3},
				   {1, 2, 3}};

	    float A[3][3] = add_f33_f33 (M2, M3);
	    assert (equalWithAbsErr_f33 (A, B, 0));
	}

	{
	    float A[3][3] = invert_f33 (M1);
	    assert (equalWithAbsErr_f33 (A, M1, 0));
	}

	{
	    const float B[3][3] = {{0.5, 0.0, 0.0},
				   {0.0, 0.5, 0.0},
				   {0.0, 0.0, 0.5}};

	    float A[3][3] = invert_f33 (M2);
	    assert (equalWithAbsErr_f33 (A, B, 0));
	}

	{
	    float A[3][3] = transpose_f33 (M1);
	    assert (equalWithAbsErr_f33 (A, M1, 0));
	}

	{
	    const float B[3][3] = {{1, 1, 1},
				   {2, 2, 2},
				   {3, 3, 1}};

	    float A[3][3] = transpose_f33 (M3);
	    assert (equalWithAbsErr_f33 (A, B, 0));
	}

	const float v[3] = {1, 2, 3};

	{
	    float a[3] = mult_f3_f33 (v, M1);
	    assert (equalWithAbsErr_f3 (a, v, 0));
	}

	{
	    const float b[3] = {6, 12, 12};
	    float a[3] = mult_f3_f33 (v, M3);
	    assert (equalWithAbsErr_f3 (a, b, 0.00001));
	}
    }

    {
	const float M1[4][4] = {{1, 0, 0, 0},
				{0, 1, 0, 0},
				{0, 0, 1, 0},
				{0, 0, 0, 1}};

	const float M2[4][4] = {{2, 0, 0, 0},
				{0, 2, 0, 0},
				{0, 0, 2, 0},
				{0, 0, 0, 2}};

	const float M3[4][4] = {{1, 2, 3, 4},
				{1, 2, 3, 4},
				{1, 2, 3, 4},
				{1, 2, 3, 2}};

	{
	    float A[4][4] = mult_f44_f44 (M1, M1);
	    assert (equalWithAbsErr_f44 (A, M1, 0));
	}

	{
	    const float B[4][4] = {{2, 4, 6, 8},
				   {2, 4, 6, 8},
				   {2, 4, 6, 8},
				   {2, 4, 6, 4}};

	    float A[4][4] = mult_f44_f44 (M2, M3);
	    assert (equalWithAbsErr_f44 (A, B, 0));
	}

	{
	    float A[4][4] = mult_f_f44 (2, M1);
	    assert (equalWithAbsErr_f44 (A, M2, 0));
	}

	{
	    const float B[4][4] = {{3, 6, 9, 12},
				   {3, 6, 9, 12},
				   {3, 6, 9, 12},
				   {3, 6, 9,  6}};

	    float A[4][4] = mult_f_f44 (3, M3);
	    assert (equalWithAbsErr_f44 (A, B, 0));
	}

	{
	    float A[4][4] = add_f44_f44 (M1, M1);
	    assert (equalWithAbsErr_f44 (A, M2, 0));
	}

	{
	    const float B[4][4] = {{3, 2, 3, 4},
				   {1, 4, 3, 4},
				   {1, 2, 5, 4},
				   {1, 2, 3, 4}};

	    float A[4][4] = add_f44_f44 (M2, M3);
	    assert (equalWithAbsErr_f44 (A, B, 0));
	}

	{
	    float A[4][4] = invert_f44 (M1);
	    assert (equalWithAbsErr_f44 (A, M1, 0));
	}

	{
	    const float B[4][4] = {{0.5, 0.0, 0.0, 0.0},
				   {0.0, 0.5, 0.0, 0.0},
				   {0.0, 0.0, 0.5, 0.0},
				   {0.0, 0.0, 0.0, 0.5}};

	    float A[4][4] = invert_f44 (M2);
	    assert (equalWithAbsErr_f44 (A, B, 0));
	}

	{
	    float A[4][4] = transpose_f44 (M1);
	    assert (equalWithAbsErr_f44 (A, M1, 0));
	}

	{
	    const float B[4][4] = {{1, 1, 1, 1},
				   {2, 2, 2, 2},
				   {3, 3, 3, 3},
				   {4, 4, 4, 2}};

	    float A[4][4] = transpose_f44 (M3);
	    assert (equalWithAbsErr_f44 (A, B, 0));
	}

	const float v[3] = {1, 2, 3};

	{
	    float a[3] = mult_f3_f44 (v, M1);
	    assert (equalWithAbsErr_f3 (a, v, 0));
	}

	{
	    float a[3] = mult_f3_f44 (v, M2);
	    assert (equalWithAbsErr_f3 (a, v, 0.00001));
	}

	{
	    const float b[3] = {0.269231, 0.538462, 0.807692};
	    float a[3] = mult_f3_f44 (v, M3);
	    assert (equalWithAbsErr_f3 (a, b, 0.00001));
	}
    }

    {
	const float v1[3] = {0, 0, 0};
	const float v2[3] = {1, 2, 3};
	const float v3[3] = {7, 8, 9};

	{
	    float a[3] = mult_f_f3 (3, v1);
	    assert (equalWithAbsErr_f3 (a, v1, 0));
	}

	{
	    float a[3] = mult_f_f3 (3, v2);
	    float b[3] = {3, 6, 9};
	    assert (equalWithAbsErr_f3 (a, b, 0));
	}

	{
	    float a[3] = add_f3_f3 (v1, v2);
	    assert (equalWithAbsErr_f3 (a, v2, 0));
	}

	{
	    float a[3] = add_f3_f3 (v2, v3);
	    float b[3] = {8, 10, 12};
	    assert (equalWithAbsErr_f3 (a, b, 0));
	}

	{
	    float a[3] = sub_f3_f3 (v2, v1);
	    assert (equalWithAbsErr_f3 (a, v2, 0));
	}

	{
	    float a[3] = sub_f3_f3 (v2, v3);
	    float b[3] = {-6, -6, -6};
	    assert (equalWithAbsErr_f3 (a, b, 0));
	}

	{
	    float a[3] = cross_f3_f3 (v2, v2);
	    assert (equalWithAbsErr_f3 (a, v1, 0));
	}

	{
	    float a[3] = cross_f3_f3 (v2, v3);
	    float b[3] = {-6, 12, -6};
	    assert (equalWithAbsErr_f3 (a, b, 0));
	}

	{
	    float a = dot_f3_f3 (v2, v2);
	    assert (equalWithAbsErr (a, 14, 0));
	}

	{
	    float a = dot_f3_f3 (v2, v3);
	    assert (equalWithAbsErr (a, 50, 0));
	}

	{
	    float a = length_f3 (v2);
	    assert (equalWithAbsErr (a, 3.74166, 0.0001));
	}

	{
	    float a = length_f3 (v3);
	    assert (equalWithAbsErr (a, 13.9284, 0.001));
	}
    }

    print ("ok\n");
}


void
testRgbXyzFunctions()
{
    print ("Testing conversion between RGB and CIE XYZ\n");

    float R2X[4][4] =
	{{ 4.123908e-01,  2.126390e-01,  1.933082e-02,  0.000000e+00},
	 { 3.575844e-01,  7.151688e-01,  1.191948e-01,  0.000000e+00},
	 { 1.804807e-01,  7.219230e-02,  9.505320e-01,  0.000000e+00},
	 { 0.000000e+00,  0.000000e+00,  0.000000e+00,  1.000000e+00}};

    float X2R[4][4] =
	{{ 3.240970e+00, -9.692435e-01,  5.563005e-02,  0.000000e+00},
	 {-1.537383e+00,  1.875967e+00, -2.039769e-01,  0.000000e+00},
	 {-4.986107e-01,  4.155506e-02,  1.056972e+00,  0.000000e+00},
	 { 0.000000e+00,  0.000000e+00,  0.000000e+00,  1.000000e+00}};

    Chromaticities c =
	{{0.6400, 0.3300},
	 {0.3000, 0.6000},
	 {0.1500, 0.0600},
	 {0.3127, 0.3290}};
    
    float A[4][4] = RGBtoXYZ (c, 1);
    float B[4][4] = XYZtoRGB (c, 1);

    assert (equalWithAbsErr_f44 (A, R2X, 0.001));
    assert (equalWithAbsErr_f44 (B, X2R, 0.001));

    print ("ok\n");
}


float[3]
color3 (float a, float b, float c)
{
    float c[3] = {a, b, c};
    return c;
}


void
testXyzLuvFunctions()
{
    print ("Testing conversion between CIE XYZ and CIE L*u*v*\n");

    float XYZn[3] = {95.0456, 100.000, 108.906};

    {
	float XYZ[3] = {41.2391, 21.2639, 1.93308};	// Rec. 709 red
	float Luv[3] = XYZtoLuv (XYZ, XYZn);
	float XYZ1[3] = LuvtoXYZ (Luv, XYZn);

	assert (equalWithAbsErr_f3 (XYZ, XYZ1, 0.01));
	assert (equalWithAbsErr (Luv[0], 53.2371, 0.01));
	assert (equalWithAbsErr (Luv[1], 175.01, 0.01));
	assert (equalWithAbsErr (Luv[2], 37.7652, 0.01));
    }

    {
	float XYZ[3] = {35.7584, 71.5169, 11.9195};	// Rec. 709 green
	float Luv[3] = XYZtoLuv (XYZ, XYZn);
	float XYZ1[3] = LuvtoXYZ (Luv, XYZn);

	assert (equalWithAbsErr_f3 (XYZ, XYZ1, 0.01));
	assert (equalWithAbsErr (Luv[0], 87.7355, 0.01));
	assert (equalWithAbsErr (Luv[1], -83.0673, 0.01));
	assert (equalWithAbsErr (Luv[2], 107.418, 0.01));
    }

    {
	float XYZ[3] = {18.0481, 7.21923, 95.0532};	// Rec. 709 blue
	float Luv[3] = XYZtoLuv (XYZ, XYZn);
	float XYZ1[3] = LuvtoXYZ (Luv, XYZn);

	assert (equalWithAbsErr_f3 (XYZ, XYZ1, 0.01));
	assert (equalWithAbsErr (Luv[0], 32.3009, 0.01));
	assert (equalWithAbsErr (Luv[1], -9.40229, 0.01));
	assert (equalWithAbsErr (Luv[2], -130.351, 0.01));
    }

    {
	float XYZ[3] = {95.0456, 100.000, 108.906};	// Rec. 709 white
	float Luv[3] = XYZtoLuv (XYZ, XYZn);
	float XYZ1[3] = LuvtoXYZ (Luv, XYZn);

	assert (equalWithAbsErr_f3 (XYZ, XYZ1, 0.01));
	assert (equalWithAbsErr (Luv[0], 100.0, 0.01));
	assert (equalWithAbsErr (Luv[1], 0.0, 0.01));
	assert (equalWithAbsErr (Luv[2], 0.0, 0.01));
    }

    {
	float XYZ[3] = {0.475228, 0.500, 0.54453};	// 0.5% gray
	float Luv[3] = XYZtoLuv (XYZ, XYZn);
	float XYZ1[3] = LuvtoXYZ (Luv, XYZn);

	assert (equalWithAbsErr_f3 (XYZ, XYZ1, 0.01));
	assert (equalWithAbsErr (Luv[0], 4.51646, 0.01));
	assert (equalWithAbsErr (Luv[1], 0.0, 0.01));
	assert (equalWithAbsErr (Luv[2], 0.0, 0.01));
    }

    print ("ok\n");
}


void
testXyzLabFunctions()
{
    print ("Testing conversion between CIE XYZ and CIE L*a*b*\n");

    float XYZn[3] = {95.0456, 100.000, 108.906};

    {
	float XYZ[3] = {41.2391, 21.2639, 1.93308};	// Rec. 709 red
	float Lab[3] = XYZtoLab (XYZ, XYZn);
	float XYZ1[3] = LabtoXYZ (Lab, XYZn);

	assert (equalWithAbsErr_f3 (XYZ, XYZ1, 0.01));
	assert (equalWithAbsErr (Lab[0], 53.2371, 0.01));
	assert (equalWithAbsErr (Lab[1], 80.0902, 0.01));
	assert (equalWithAbsErr (Lab[2], 67.2033, 0.01));
    }

    {
	float XYZ[3] = {35.7584, 71.5169, 11.9195};	// Rec. 709 green
	float Lab[3] = XYZtoLab (XYZ, XYZn);
	float XYZ1[3] = LabtoXYZ (Lab, XYZn);

	assert (equalWithAbsErr_f3 (XYZ, XYZ1, 0.01));
	assert (equalWithAbsErr (Lab[0], 87.7355, 0.01));
	assert (equalWithAbsErr (Lab[1], -86.1818, 0.01));
	assert (equalWithAbsErr (Lab[2], 83.1867, 0.01));
    }

    {
	float XYZ[3] = {18.0481, 7.21923, 95.0532};	// Rec. 709 blue
	float Lab[3] = XYZtoLab (XYZ, XYZn);
	float XYZ1[3] = LabtoXYZ (Lab, XYZn);

	assert (equalWithAbsErr_f3 (XYZ, XYZ1, 0.01));
	assert (equalWithAbsErr (Lab[0], 32.3009, 0.01));
	assert (equalWithAbsErr (Lab[1], 79.1954, 0.01));
	assert (equalWithAbsErr (Lab[2], -107.855, 0.01));
    }

    {
	float XYZ[3] = {95.0456, 100.000, 108.906};	// Rec. 709 white
	float Lab[3] = XYZtoLab (XYZ, XYZn);
	float XYZ1[3] = LabtoXYZ (Lab, XYZn);

	assert (equalWithAbsErr_f3 (XYZ, XYZ1, 0.01));
	assert (equalWithAbsErr (Lab[0], 100.0, 0.01));
	assert (equalWithAbsErr (Lab[1], 0.0, 0.01));
	assert (equalWithAbsErr (Lab[2], 0.0, 0.01));
    }

    {
	float XYZ[3] = {0.475228, 0.500, 0.54453};	// 0.5% gray
	float Lab[3] = XYZtoLab (XYZ, XYZn);
	float XYZ1[3] = LabtoXYZ (Lab, XYZn);

	assert (equalWithAbsErr_f3 (XYZ, XYZ1, 0.01));
	assert (equalWithAbsErr (Lab[0], 4.51646, 0.01));
	assert (equalWithAbsErr (Lab[1], 0.0, 0.01));
	assert (equalWithAbsErr (Lab[2], 0.0, 0.01));
    }

    {
	float XYZ[3] = {0.000, 0.000, 0.000};		// black
	float Lab[3] = XYZtoLab (XYZ, XYZn);
	float XYZ1[3] = LabtoXYZ (Lab, XYZn);

	assert (equalWithAbsErr_f3 (XYZ, XYZ1, 0.01));
	assert (equalWithAbsErr (Lab[0], 0.0, 0.01));
	assert (equalWithAbsErr (Lab[1], 0.0, 0.01));
	assert (equalWithAbsErr (Lab[2], 0.0, 0.01));
    }

    print ("ok\n");
}


int
testStdLibrary ()
{
    testEqualWithAbsErr();
    testChangeBuiltinConstants();
    testBuiltinConstants();
    testClassification ();
    testMathFunctions();
    testVecMatrixFunctions();
    testRgbXyzFunctions();
    testXyzLuvFunctions();
    testXyzLabFunctions();
    return 0;
}


const int testStdLibraryDummy = testStdLibrary();



} // namespace testStdLibrary
