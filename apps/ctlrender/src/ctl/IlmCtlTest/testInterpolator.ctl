import "common";

namespace testInterpolator
{

void
test1 ()
{
    float data[][2][3] =
    {
	{{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
	{{1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}},
	{{0.0, 3.0, 0.0}, {0.0, 3.0, 0.0}},
	{{0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}},
	{{2.0, 2.0, 2.0}, {2.0, 2.0, 2.0}}
    };

    float pMin[3] = {-2.0, -1.0, 0.0};
    float pMax[3] = {2.0, 3.0, 4.0};

    const int sx = 8;
    const int sy = 12;
    const int sz = 16;

    const float grid[sx][sy][sz][3],
	scatteredDataToGrid3D (data, pMin, pMax, grid);

    float s;
    float t;
    float p[3];

    for (int i = 0; i < sx; i = i + 1)
    {
	s = i / (sx - 1.0);
	t = 1 - s;
	p[0] = t * pMin[0] + s * pMax[0];

	for (int j = 0; j < sx; j = j + 1)
	{
	    s = j / (sy - 1.0);
	    t = 1 - s;
	    p[1] = t * pMin[1] + s * pMax[1];

	    for (int k = 0; k < sx; k = k + 1)
	    {
		s = k / (sz - 1.0);
		t = 1 - s;
		p[2] = t * pMin[2] + s * pMax[2];

		float q[3] = grid[i][j][k];

		assert (equalWithAbsErr (p[0], q[0], 0.0001));
		assert (equalWithAbsErr (p[1], q[1], 0.0001));
		assert (equalWithAbsErr (p[2], q[2], 0.0001));
	    }
	}
    }
}


void
test2 ()
{
    float data[27][2][3];

    for (int i = 0; i < 3; i = i + 1)
    {
	for (int j = 0; j < 3; j = j + 1)
	{
	    for (int k = 0; k < 3; k = k + 1)
	    {
		data[i * 9 + j * 3 + k][0][0] = i;
		data[i * 9 + j * 3 + k][0][1] = j;
		data[i * 9 + j * 3 + k][0][2] = k;

		if (i == 1 && j == 1 && k == 1)
		{
		    data[i * 9 + j * 3 + k][1][0] = 1;
		    data[i * 9 + j * 3 + k][1][1] = 2;
		    data[i * 9 + j * 3 + k][1][2] = 3;
		}
		else
		{
		    data[i * 9 + j * 3 + k][1][0] = 0;
		    data[i * 9 + j * 3 + k][1][1] = 0;
		    data[i * 9 + j * 3 + k][1][2] = 0;
		}
	    }
	}
    }

    float grid[3][3][3][3];
    float pMin[3] = {0, 0, 0};
    float pMax[3] = {2, 2, 2};

    scatteredDataToGrid3D (data, pMin, pMax, grid);

    for (int i = 0; i < 3; i = i + 1)
    {
	for (int j = 0; j < 3; j = j + 1)
	{
	    for (int k = 0; k < 3; k = k + 1)
	    {
		if (i == 1 && j == 1 && k == 1)
		{
		    assert (equalWithAbsErr (grid[i][j][k][0], 1, 0.0001));
		    assert (equalWithAbsErr (grid[i][j][k][1], 2, 0.0001));
		    assert (equalWithAbsErr (grid[i][j][k][2], 3, 0.0001));
		}
		else
		{
		    assert (equalWithAbsErr (grid[i][j][k][0], 0, 0.0001));
		    assert (equalWithAbsErr (grid[i][j][k][1], 0, 0.0001));
		    assert (equalWithAbsErr (grid[i][j][k][2], 0, 0.0001));
		}
	    }
	}
    }
}


int
runTest ()
{
    print ("Testing 3D scattered data interpolation\n");
    test1();
    test2();
    print ("ok\n");
    return 0;
}

const int t = runTest();

}  // namespace testInterpolator
