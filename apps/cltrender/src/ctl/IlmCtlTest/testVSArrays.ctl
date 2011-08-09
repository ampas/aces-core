namespace testVSArrays
{

int
testArgs(output int a[], int b[][])
{

    a[1] = 1;

    return 2;
@error21    a = b[0];
}


void
testVSA1D(int a[])
{
    assert(a[0] == 2);
}

void
testVSA2D(int a[][])
{
    assert(a[1][2] == 6);
}

int
testVSA2D2(int a[][])
{
    return a[1][2];
}


void
testVSA2D3(output int a[][])
{
    a[1][2] = 5;
}


void
testVSA4D(output int a[][][][])
{
    assert(a[0][2][0][1] == 8);
    a[0][2][0][1] = 100;
    
    int i;
    i = 2;
}


const int a[2] = {1,2};
const int s = a.size;


void
empty(int a[][][])
{

}

void
testVSA()
{




    int ai[10];
    int b[1][2];
    testArgs(ai, b);

    int a[1] = {2};
    testVSA1D(a);

    int b2[2][3] = {{1,2,3},{4,5,6}};
    testVSA2D(b2);
    
    int b3[2][3] = {{1,2,3},{4,5,6}};
    assert(testVSA2D2(b3) == 6);
    assert(2*testVSA2D2(b3)-1 == 11);
    
    int b4[2][3] = {{1,2,3},{4,5,6}};
    testVSA2D3(b4);
    assert(b4[1][2] == 5);
    assert(b4[0][0] == 1);
    
    
    int b5[2][4][1][3] = { { {{1,2,3}}, {{4,5,6}}, {{7,8,9}}, {{10,11,12}} }, 
			  { {{1,1,1}}, {{1,1,1}}, {{1,1,1}}, {{1,1,1}}} };
    
    testVSA4D(b5);
    assert(b5[0][2][0][1] == 100);

    int emptyA[1][2][3];
    empty(emptyA);

}

void
retSize(output int a1, 
	output int a2, 
	int a[][][][], 
	output int a3,
	output int a4)
{
    a1 = a.size;
    a2 = a[0].size;
    a3 = a[0][0].size;
    a4 = a[0][0][0].size;
}

// Test size op for varying and non-varying args
//
void
testArgSize(int a[][], int b[1][2][3])
{
    assert(b.size == 1);
    assert(b[0].size == 2);
    assert(b[0][0].size == 3);

    assert(a.size == 3);
    assert(a[0].size == 2);
    assert(a[1].size == 2);
    assert(a[2].size == 2);

}

// Test size op with args where only some dimensions are varying
//
void
testArgSize2(int a[3][], int b[1][2][3])
{
    assert(b.size == 1);
    assert(b[0].size == 2);
    assert(b[0][0].size == 3);

    assert(a.size == 3);
    assert(a[0].size == 2);
    assert(a[1].size == 2);
    assert(a[2].size == 2);

}

// Test size op with args where only some dimensions are varying
//
void
testArgSize3(int a[][2], int b[1][2][3])
{
    assert(b.size == 1);
    assert(b[0].size == 2);
    assert(b[0][0].size == 3);

    assert(a.size == 3);
    assert(a[0].size == 2);
    assert(a[1].size == 2);
    assert(a[2].size == 2);
}

void
testSize()
{
    int a[3][2];
    int b[1][2][3];
    testArgSize(a, b);
    testArgSize2(a, b);
    testArgSize3(a, b);
    
    int a1;
    int a2; 
    int a3;
    int a4;

    int rsa1[1][2][3][4];
    retSize(a1, a2, rsa1, a3, a4);
    assert(a1 == 1);
    assert(a2 == 2);
    assert(a3 == 3);
    assert(a4 == 4);
    
    int rsa2[2][5][2][1];
    retSize(a1, a2, rsa2, a3, a4);
    assert(a1 == 2);
    assert(a2 == 5);
    assert(a3 == 2);
    assert(a4 == 1);
    
    return;
            struct size
            {
@error33	int size[3];
            };
            size s;
@error38 @error16 @error20    s.size[0] = 2;
@error38 @error16    assert(s.size[0] == 2);

}



int[]
@error10  checkReturn(int a[])
{
@error5    return a;
}

int[2]
checkReturn2(int a[])
{
@error5    return a;
}

int[2]
returnLen2()
{
    int a[2];
    return a;
}



void
checkNotAllowed(int a[])
{
    return;

    int aLocal[2];
@error21    aLocal = a;

 int aLocalWrong[3]; 
@error21    aLocalWrong = a;
@error10 @error9    int aLocalWrong2[3] = a;

@error10    int aLocalUnk[] = a;

@error10    int bLocalUnk[];

@error8    const int cLocalUni[3];


}


@error8 const int staticA[];


void
testReturns()
{

    int ai1[2] = {-2,3};
    checkNotAllowed(ai1);

    int ai3[3] = {5,6,7};
    @error9 int ai3b[3] = returnLen2();

    assert( ai3[2] == 7);


}

void
testLoopsSub(int iin[], bool bin[], half hin[], float fin[],

	     output int iout[], 
	     output bool bout[], 
	     output half hout[], 
	     output float fout[],

	     half hin2[],
	     output half hout2[])
{

    assert(iin.size == iout.size);
    int i = iin.size-1;
    while( i > 0 )
    {
	iout[i] = iin[i];
	i = i - 1;
    }

    assert(bin.size == bout.size);
    i = bin.size-1;
    while( i > 0 )
    {
	bout[i] = bin[i];
	i = i - 1;
    }

    assert(hin.size == hout.size);
    i = hin.size-1;
    while( i > 0 )
    {
	hout[i] = hin[i];
	i = i - 1;
    }

    assert(fin.size == fout.size);
    i = fin.size-1;
    while( i > 0 )
    {
	fout[i] = fin[i];
	i = i - 1;
    }

    return;
@error21    hout2 = hin2;
}


void
testLoops()
{
    const int size = 20;

    int iin[] = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0};
    int iout[size];

    bool bin[size];
    bool bout[size];

    float fin[size];
    float fout[size];

    half hin[size];
    half hout[size];

    half hin2[size];
    half hout2[size];


    int i = bin.size - 1;
    float f = -4.2;
    bool b = true;
    while( i > 0 )
    {
	fin[i] = f;
	hin[i] = f;
	bin[i] = b;
	
	f = f + 0.0129399;
	b = !b;
	i = i - 1;
    }
    testLoopsSub(iin, bin, hin, fin, iout, bout, hout, fout, hin2, hout2);

    i = size -1; 
    while( i > 0 )
    {
	
	assert(iout[i] == iin[i]);
	assert(bout[i] == bin[i]);
	assert(fout[i] == fin[i]);
	assert(hout[i] == hin[i]);
	i = i -1;
    }
}


// Test nested variable-size args in function calls
//
void
testPassVSA1 (float x[], int sz)
{
    assert (x.size == sz);
}


void
testPassVSA2 (float x[], int sz)
{
    assert (x.size == sz);

    testPassVSA1 (x, sz);
    testPassVSA1 (x, x.size);

    const float z[] = {1, 2, 3};
    testPassVSA1 (z, 3);
    testPassVSA1 (z, z.size);
}


void
testPassVSA ()
{
    const float x[] = {1, 2, 3, 4};
    testPassVSA2 (x, 4);
    testPassVSA2 (x, x.size);
}

// Test multi-dimensional nested variable-size args in function 
// calls
void
testPassMVSA1 (half x[][], int sz0, int sz1)
{
    assert (x.size == sz0);
    assert (x[0].size == sz1);

}


void
testPassMVSA2 (half x[1][], int sz0, int sz1)
{
    assert (x.size == sz0);
    assert (x[0].size == sz1);

    testPassMVSA1 (x, sz0, sz1);
    testPassMVSA1 (x, x.size, x[0].size);

    const half z[][] = {{1, 2, 3},{4,5,6}};
    testPassMVSA1 (z, 2, 3);
    testPassMVSA1 (z, z.size, z[0].size);
}


void
testPassMVSA ()
{
    const half x[][] = {{1, 2, 3, 4}};
    testPassMVSA2 (x, 1, 4);
    testPassMVSA2 (x, x.size, x[0].size);
}


int
all()
{
    testVSA();
    testSize();
    testReturns();
    testLoops();
    testPassVSA();
    testPassMVSA();
    return 100000000;
}

const int i = all();


} // namespace VSArrays
