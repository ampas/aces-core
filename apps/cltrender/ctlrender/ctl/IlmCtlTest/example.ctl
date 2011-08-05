
namespace ilm
{
    // weighted average of two values

    float
    mix (float a = 0,
	 float b = 0,
	 float m = 0.5)
    {
	return a * m + b * (1.0 - m);
    }


    void
    mixRedGreen (output float rOut,
		 output float gOut,
		 output float bOut,
		 float r = 0,
		 float g = 0,
		 float b = 0,
		 float m = 0.5)
    {
	if (m == 0)
	{
	    rOut = r;  // no mixing required
	    gOut = g;
	    bOut = b;
	    return;
	}

	rOut = mix (r, g, m);
	gOut = ilm::mix (g, r, m);
	bOut = b;
    }

    int[3]
    foo (int b[2])
    {
	bool a = false;
	int x = a + b[0];
	const int y = 6 / 2;
	int z[] = {0, 0, 0};
	float w[y] = {1.0, 2.0, 3.0};
	float v = w[2] + z[2];

	if (b[0] != b[1])
	{
	    z[0] = 3;
	    z[1] = y;
	    z[2] = a;

	    return z;
	}

	z[2] = w[1] + 3;
	return z;
    }

    float
    rec709gammaToLinear (float x)
    {
	if (x <= 0.08125)
	    return x / 4.5;
	else
	    return pow ((x + 0.099) / 1.099, 1 / 0.45);
    }

    void
    rec709YCbCrToLinear (float Y,
			 float Cb,
			 float Cr,
			 output float R,
			 output float G,
			 output float B,
			 output float chromaticities[8])
    {
	float R1 = (1.5748 * Cr) + Y;
	float B1 = (1.8556 * Cb) + Y;
	float G1 = (Y - 0.0722 * B1 - 0.2126 * R1) / 0.7152;
	
	R = rec709gammaToLinear (R1);
	G = rec709gammaToLinear (G1);
	B = rec709gammaToLinear (B1);

	chromaticities[0] = 0.640;	// red
	chromaticities[1] = 0.330;
	chromaticities[2] = 0.300;	// green
	chromaticities[3] = 0.600;
	chromaticities[4] = 0.150;	// blue
	chromaticities[5] = 0.060;
	chromaticities[6] = 0.3127;	// white point
	chromaticities[7] = 0.3290;
    }

    void
    foo1 ()
    {
	float x = 1 + 2 + 3;
	float y = 1 * 2 * 3;
	int z = !~--4;
    }

    int
    factorial1 (int i)
    {
	int k = i;
	int j = 1;

	while (i > 1)
	{
	    j = j * k;
	    k = k - 1;
	}

	return j;
    }

    int
    factorial2 (int i)
    {
	if (i <= 1)
	    return 1;
	else
	    return i * factorial2 (i - (3 - 8/4));
    }

    const int one = 6/2 - 2;
    const int two = sqrt (4.0);
    const int x[2 * one] = {1, 2};
    const int y[2 * 2 * one] = {1, 2, 3, 4};
    const int z[4] = {1, 2, 3, 4};
}
