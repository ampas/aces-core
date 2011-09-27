// Test 1D and 3D interpolated table lookups

import "common";

float[3]
f3 (float x, float y, float z)
{
    float tmp[3] = {x, y, z};
    return tmp;
}


bool
equal (float x[3], float y[3])
{
    return x[0] == y[0] &&
	   x[1] == y[1] &&
	   x[2] == y[2];
}


void
testLookup1D ()
{
    print ("Testing 1D table lookups: linear, regular spacіng\n");

    float f[1] = {1};

    assert (lookup1D (f, 2.0, 4.0, 0.0) == 1.0);
    assert (lookup1D (f, 2.0, 4.0, 2.0) == 1.0);
    assert (lookup1D (f, 2.0, 4.0, 3.0) == 1.0);
    assert (lookup1D (f, 2.0, 4.0, 4.0) == 1.0);
    assert (lookup1D (f, 2.0, 4.0, 5.0) == 1.0);

    float g[2] = {1, 2};

    assert (lookup1D (g, 2.0, 4.0, 0.0) == 1.0);
    assert (lookup1D (g, 2.0, 4.0, 2.0) == 1.0);
    assert (lookup1D (g, 2.0, 4.0, 3.0) == 1.5);
    assert (lookup1D (g, 2.0, 4.0, 4.0) == 2.0);
    assert (lookup1D (g, 2.0, 4.0, 5.0) == 2.0);

    float h[4] = {2, 6, 1, 7};

    assert (lookup1D (h, 10.0, 40.0, 0.0) == 2);
    assert (lookup1D (h, 10.0, 40.0, 10.0) == 2);
    assert (lookup1D (h, 0.0, 3.0, 0.25) == 3);
    assert (lookup1D (h, 0.0, 3.0, 0.5) == 4);
    assert (lookup1D (h, 0.0, 3.0, 0.75) == 5);
    assert (lookup1D (h, 0.0, 3.0, 1.00) == 6);
    assert (lookup1D (h, 10.0, 40.0, 15.0) == 4);
    assert (lookup1D (h, 10.0, 40.0, 35.0) == 4);
    assert (lookup1D (h, 10.0, 40.0, 40.0) == 7);
    assert (lookup1D (h, 10.0, 40.0, 50.0) == 7);

    print ("ok\n");
}


void
testLookupCubic1D ()
{
    print ("Testing 1D table lookups: cubic, regular spacing\n");

    float f[1] = {1};

    assert (lookupCubic1D (f, 2.0, 4.0, 0.0) == 1.0);
    assert (lookupCubic1D (f, 2.0, 4.0, 2.0) == 1.0);
    assert (lookupCubic1D (f, 2.0, 4.0, 3.0) == 1.0);
    assert (lookupCubic1D (f, 2.0, 4.0, 4.0) == 1.0);
    assert (lookupCubic1D (f, 2.0, 4.0, 5.0) == 1.0);

    float g[2] = {1, 2};

    assert (lookupCubic1D (g, 2.0, 4.0, 0.0) == 1.0);
    assert (lookupCubic1D (g, 2.0, 4.0, 2.0) == 1.0);
    assert (lookupCubic1D (g, 2.0, 4.0, 3.0) == 1.5);
    assert (lookupCubic1D (g, 2.0, 4.0, 4.0) == 2.0);
    assert (lookupCubic1D (g, 2.0, 4.0, 5.0) == 2.0);

    float h[7] = {2, 6, 1, 1, 7, 1, 1};

    assert (equalWithAbsErr (lookupCubic1D (h, 10, 40,  0), 2.00000, 0.00002));
    assert (equalWithAbsErr (lookupCubic1D (h, 10, 40, 10), 2.00000, 0.00002));
    assert (equalWithAbsErr (lookupCubic1D (h, 10, 40, 11), 3.23200, 0.00002));
    assert (equalWithAbsErr (lookupCubic1D (h, 10, 40, 15), 6.00000, 0.00002));
    assert (equalWithAbsErr (lookupCubic1D (h, 10, 40, 16), 5.49600, 0.00002));
    assert (equalWithAbsErr (lookupCubic1D (h, 10, 40, 25), 1.00000, 0.00002));
    assert (equalWithAbsErr (lookupCubic1D (h, 10, 40, 30), 7.00000, 0.00002));
    assert (equalWithAbsErr (lookupCubic1D (h, 10, 40, 31), 6.47200, 0.00002));
    assert (equalWithAbsErr (lookupCubic1D (h, 10, 40, 35), 1.00000, 0.00002));
    assert (equalWithAbsErr (lookupCubic1D (h, 10, 40, 39), 0.71200, 0.00002));
    assert (equalWithAbsErr (lookupCubic1D (h, 10, 40, 40), 1.00000, 0.00002));
    assert (equalWithAbsErr (lookupCubic1D (h, 10, 40, 41), 1.00000, 0.00002));

    print ("ok\n");
}


void
testLookup3D ()
{
    print ("Testing 3D table lookups, trilinear, regular spacing\n");

    assert ( equal (f3 (1, 2, 3), f3 (1, 2, 3)));
    assert (!equal (f3 (0, 2, 3), f3 (1, 2, 3)));
    assert (!equal (f3 (1, 0, 3), f3 (1, 2, 3)));
    assert (!equal (f3 (1, 2, 0), f3 (1, 2, 3)));

    float f[2][2][2][3];

    f[0][0][0] = f3 (2, 2, 2);
    f[0][0][1] = f3 (2, 2, 6);
    f[0][1][0] = f3 (2, 6, 2);
    f[0][1][1] = f3 (2, 6, 6);
    f[1][0][0] = f3 (6, 2, 2);
    f[1][0][1] = f3 (6, 2, 6);
    f[1][1][0] = f3 (6, 6, 2);
    f[1][1][1] = f3 (6, 6, 6);

    float fMin[3] = {1, 1, 1};
    float fMax[3] = {5, 5, 5};
    int  fSize[3] = {2, 2, 2};

    assert (equal (lookup3D_f3 (f, fMin, fMax, f3 (1, 1, 1)), f3 (2, 2, 2)));
    assert (equal (lookup3D_f3 (f, fMin, fMax, f3 (5, 1, 1)), f3 (6, 2, 2)));
    assert (equal (lookup3D_f3 (f, fMin, fMax, f3 (1, 5, 1)), f3 (2, 6, 2)));
    assert (equal (lookup3D_f3 (f, fMin, fMax, f3 (5, 5, 1)), f3 (6, 6, 2)));
    assert (equal (lookup3D_f3 (f, fMin, fMax, f3 (1, 1, 5)), f3 (2, 2, 6)));
    assert (equal (lookup3D_f3 (f, fMin, fMax, f3 (5, 1, 5)), f3 (6, 2, 6)));
    assert (equal (lookup3D_f3 (f, fMin, fMax, f3 (1, 5, 5)), f3 (2, 6, 6)));
    assert (equal (lookup3D_f3 (f, fMin, fMax, f3 (5, 5, 5)), f3 (6, 6, 6)));

    assert (equal (lookup3D_f3 (f, fMin, fMax, f3 (2, 2, 2)), f3 (3, 3, 3)));
    assert (equal (lookup3D_f3 (f, fMin, fMax, f3 (4, 2, 2)), f3 (5, 3, 3)));
    assert (equal (lookup3D_f3 (f, fMin, fMax, f3 (2, 4, 2)), f3 (3, 5, 3)));
    assert (equal (lookup3D_f3 (f, fMin, fMax, f3 (4, 4, 2)), f3 (5, 5, 3)));
    assert (equal (lookup3D_f3 (f, fMin, fMax, f3 (2, 2, 4)), f3 (3, 3, 5)));
    assert (equal (lookup3D_f3 (f, fMin, fMax, f3 (4, 2, 4)), f3 (5, 3, 5)));
    assert (equal (lookup3D_f3 (f, fMin, fMax, f3 (2, 4, 4)), f3 (3, 5, 5)));
    assert (equal (lookup3D_f3 (f, fMin, fMax, f3 (4, 4, 4)), f3 (5, 5, 5)));

    assert (equal (lookup3D_f3 (f, fMin, fMax, f3 (0, 0, 0)), f3 (2, 2, 2)));
    assert (equal (lookup3D_f3 (f, fMin, fMax, f3 (6, 0, 0)), f3 (6, 2, 2)));
    assert (equal (lookup3D_f3 (f, fMin, fMax, f3 (0, 6, 0)), f3 (2, 6, 2)));
    assert (equal (lookup3D_f3 (f, fMin, fMax, f3 (6, 6, 0)), f3 (6, 6, 2)));
    assert (equal (lookup3D_f3 (f, fMin, fMax, f3 (0, 0, 6)), f3 (2, 2, 6)));
    assert (equal (lookup3D_f3 (f, fMin, fMax, f3 (6, 0, 6)), f3 (6, 2, 6)));
    assert (equal (lookup3D_f3 (f, fMin, fMax, f3 (0, 6, 6)), f3 (2, 6, 6)));
    assert (equal (lookup3D_f3 (f, fMin, fMax, f3 (6, 6, 6)), f3 (6, 6, 6)));

    float xf;
    float yf;
    float zf;

    half xh;
    half yh;
    half zh;

    float g[2][3][4][3];
    float gMin[3];
    float gMax[3];

    gMin = f3 (0, 0, 0);
    gMax = f3 (1, 2, 3);

    for (int i = 0; i < 2; i = i + 1)
	for (int j = 0; j < 3; j = j + 1)
	    for (int k = 0; k < 4; k = k + 1)
		g[i][j][k] = f3 (i, j, k);

    assert (equal (lookup3D_f3 (g, gMin, gMax, f3 (.25, 1.5, 2.75)),
		   f3 (.25, 1.5, 2.75)));

    lookup3D_f (g, gMin, gMax, .25, 1.5, 2.75, xf, yf, zf);
    assert (xf == .25 && yf == 1.5 && zf == 2.75);

    lookup3D_h (g, gMin, gMax, .25, 1.5, 2.75, xh, yh, zh);
    assert (xh == .25 && yh == 1.5 && zh == 2.75);

    for (int i = 0; i < 2; i = i + 1)
	for (int j = 0; j < 3; j = j + 1)
	    for (int k = 0; k < 4; k = k + 1)
	    {
		assert (equal (lookup3D_f3 (g, gMin, gMax, f3 (i, j, k)),
			       f3 (i, j, k)));

		lookup3D_f (g, gMin, gMax, i, j, k, xf, yf, zf);
		assert (xf == i && yf == j && zf == k);

		lookup3D_h (g, gMin, gMax, i, j, k, xh, yh, zh);
		assert (xh == i && yh == j && zh == k);
	    }

    gMin = f3 (2, 4, 6);
    gMax = f3 (3, 6, 9);
    int n = 0;

    for (int i = 0; i < 2; i = i + 1)
	for (int j = 0; j < 3; j = j + 1)
	    for (int k = 0; k < 4; k = k + 1)
	    {
		assert (equal (lookup3D_f3 (g, gMin, gMax,
					    f3 (i+2, j+4, k+6)),
			       f3 (i, j, k)));

		lookup3D_f (g, gMin, gMax, i+2, j+4, k+6, xf, yf, zf);
		assert (xf == i && yf == j && zf == k);

		lookup3D_h (g, gMin, gMax, i+2, j+4, k+6, xh, yh, zh);
		assert (xh == i && yh == j && zh == k);

		n = n + 1;
	    }

    assert (n == 2 * 3 * 4);
    print ("ok\n");
}


void
testInterpolate1D ()
{
    print ("Testing 1D table lookups: linear, random spacing\n");

    float f[][2] = {{1, 2}};

    assert (interpolate1D (f, 0.0) == 2.0);
    assert (interpolate1D (f, 1.0) == 2.0);
    assert (interpolate1D (f, 2.0) == 2.0);

    float g[][2] = {{1, 2}, {3, 4}};

    assert (interpolate1D (g, 0.0) == 2.0);
    assert (interpolate1D (g, 1.0) == 2.0);
    assert (interpolate1D (g, 2.0) == 3.0);
    assert (interpolate1D (g, 3.0) == 4.0);
    assert (interpolate1D (g, 4.0) == 4.0);

    float h[][2] = {{1, 2}, {3, 4}, {4, 5}, {4, 5}, {8, 7}, {9, 1}};

    assert (interpolate1D (h, 0.0) == 2.0);
    assert (interpolate1D (h, 1.0) == 2.0);
    assert (interpolate1D (h, 1.5) == 2.5);
    assert (interpolate1D (h, 2.0) == 3.0);
    assert (interpolate1D (h, 2.5) == 3.5);
    assert (interpolate1D (h, 3.0) == 4.0);
    assert (interpolate1D (h, 4.0) == 5.0);
    assert (interpolate1D (h, 5.0) == 5.5);
    assert (interpolate1D (h, 6.0) == 6.0);
    assert (interpolate1D (h, 7.0) == 6.5);
    assert (interpolate1D (h, 8.0) == 7.0);
    assert (interpolate1D (h, 9.0) == 1.0);
    assert (interpolate1D (h, 10.0) == 1.0);

    print ("ok\n");
}


void
testInterpolateCubic1D ()
{
    print ("Testing 1D table lookups: cubic, random spacing\n");

    float f[][2] = {{1, 2}};

    assert (interpolateCubic1D (f, 0.0) == 2.0);
    assert (interpolateCubic1D (f, 1.0) == 2.0);
    assert (interpolateCubic1D (f, 2.0) == 2.0);

    float g[][2] = {{1, 2}, {3, 4}};

    assert (interpolateCubic1D (g, 0.0) == 2.0);
    assert (interpolateCubic1D (g, 1.0) == 2.0);
    assert (equalWithAbsErr (interpolateCubic1D (g, 2.0), 3.00000, 0.00002));
    assert (interpolateCubic1D (g, 3.0) == 4.0);
    assert (interpolateCubic1D (g, 4.0) == 4.0);

    float h[][2] = {{1, 2}, {3, 4}, {4, 5}, {8, 7}, {9, 1}};

    assert (interpolateCubic1D (h, 0.0) == 2.0);
    assert (interpolateCubic1D (h, 1.0) == 2.0);
    assert (equalWithAbsErr (interpolateCubic1D (h, 1.5), 2.50000, 0.00002));
    assert (equalWithAbsErr (interpolateCubic1D (h, 2.0), 3.00000, 0.00002));
    assert (equalWithAbsErr (interpolateCubic1D (h, 2.5), 3.50000, 0.00002));
    assert (interpolateCubic1D (h, 3.0) == 4.0);
    assert (interpolateCubic1D (h, 4.0) == 5.0);
    assert (equalWithAbsErr (interpolateCubic1D (h, 5.0), 6.25000, 0.00002));
    assert (equalWithAbsErr (interpolateCubic1D (h, 6.0), 7.75000, 0.00002));
    assert (equalWithAbsErr (interpolateCubic1D (h, 7.0), 8.37500, 0.00002));
    assert (interpolateCubic1D (h, 8.0) == 7.0);
    assert (interpolateCubic1D (h, 9.0) == 1.0);
    assert (interpolateCubic1D (h, 10.0) == 1.0);

    print ("ok\n");
}


int
testLookupTables ()
{
    testLookup1D();
    testLookupCubic1D();
    testLookup3D();
    testInterpolate1D();
    testInterpolateCubic1D();
    return 0;
}


const int testLookupTablesDummy = testLookupTables();
