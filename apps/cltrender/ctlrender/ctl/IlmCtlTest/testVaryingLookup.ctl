// Test 1D and 3D interpolated table lookups with varying input data.
// The functions below are called by C++ code in testVaryingLookup.C


void
varyingLookup1D
    (input varying float pMin,
     input varying float pMax,
     input varying float p,
     output varying float q)
{
    float h[4] = {2, 6, 1, 7};
    q = lookup1D (h, pMin, pMax, p);
}


void
varyingLookupCubic1D
    (input varying float pMin,
     input varying float pMax,
     input varying float p,
     output varying float q)
{
    float h[2] = {2, 4};
    q = lookupCubic1D (h, pMin, pMax, p);
}


void
varyingLookup3D
    (input varying float pMin[3],
     input varying float pMax[3],
     input varying float p1[3],
     output varying float q1[3],
     input varying float p20,
     input varying float p21,
     input varying float p22,
     output varying float q20,
     output varying float q21,
     output varying float q22,
     input varying half p30,
     input varying half p31,
     input varying half p32,
     output varying half q30,
     output varying half q31,
     output varying half q32)
{
    float g[2][3][4][3];

    for (int i = 0; i < 2; i = i + 1)
    {
	for (int j = 0; j < 3; j = j + 1)
	{
	    for (int k = 0; k < 4; k = k + 1)
	    {
		g[i][j][k][0] = i;
		g[i][j][k][1] = j;
		g[i][j][k][2] = k;
	    }
	}
    }

    q1 = lookup3D_f3 (g, pMin, pMax, p1);
    lookup3D_f (g, pMin, pMax, p20, p21, p22, q20, q21, q22);
    lookup3D_h (g, pMin, pMax, p30, p31, p32, q30, q31, q32);
}


void
varyingInterpolate1D
    (input varying float p,
     output varying float q)
{
    float h[][2] = {{1, 2}, {3, 4}};
    q = interpolate1D (h, p);
}


void
varyingInterpolateCubic1D
    (input varying float p,
     output varying float q)
{
    float h[][2] = {{1, 3}, {3, 5}};
    q = interpolateCubic1D (h, p);
}
