import "common";

namespace testArray
{

int [3]
returnIntArray1()
{
    int i[3] = {1,2,3};
    return i;
}

int [1]
returnIntArray2(int a[1])
{
    return a;
}

int [20]
returnIntArray3(int a[20])
{
    int b[20];
    return b;
}

bool [20][1][2][3][1][1][3]
returnIntArray4()
{
    bool b[20][1][2][3][1][1][3];
    return b;
}


void
simple()
{
    int ia1[] = {1,2,3,4};
    int ia2[3];
    ia2[0] = ia1[0];
    assert(ia2[0] == 1);
    assert(ia2[0] == ia1[0]);

    int ia3[1] = {0};
    int ia3b[] = ia3;

    assert(ia3[0] == ia3b[0]);
}


void
funcCalls()
{
                     return;
                     int ia1[3] = returnIntArray1();
                     ia1 = returnIntArray1();
                    
                     int ia2[1];
                     int ia3[1] = returnIntArray2(ia2);

                     int ia4[] = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0};
                     int ia5[] = returnIntArray3(ia4);

@error21             ia2 = returnIntArray1();
@error9              int ia6[4] =  returnIntArray1();
    
                     int ia7[] = {1,2,3};
@error27 @error10    int ia8[] = returnIntArray3(ia7);


@error18             bool b = !returnIntArray2(ia2);
}


void
indexing()
{
            int ia1[] = {1,2,3,4};
@error14    ia1[.5];
@error14    ia1[1 + .5];
            assert(ia1[true] == 2);
            assert(ia1[false] == 1);
            assert(ia1[1+2-2] == 2);
	    assert(ia1[ia1[ia1[0]]] == 3);
}


void
assignments()
{

           int i = returnIntArray1()[1];  
           bool bb[3] = returnIntArray4()[19][0][1][2][0][0];
@error9    bool bb2[2] = returnIntArray4()[19][0][1][2][0][0];
@error17   bool bb3[] = returnIntArray4()[19][0][1][2][0];
           bool bb4[][] = returnIntArray4()[19][0][1][2][0];

    assert(i == 2);

    int a[10] = {1,2,3,4,5,6,7,8,9,10};
    int b[10];
    int c[10];
    int d[10];

    b = a;
    c = b;
    d = c;
    
    assert(d[0] == a[0]);
    assert(d[3] == a[3]);
    assert(d[4] == a[4]);
    assert(d[9] == a[9]);
}

void
multidimensional()
{
    int oned[] = {1,2,3,4,5};
    assert (oned[0] == 1);
    assert (oned[1] == 2);
    assert (oned[2] == 3);
    assert (oned[3] == 4);
    assert (oned[4] == 5);

    int twod1[][] = {{1,2,3},{4,5,6}};
    int twod2[2][3] = twod1;

    int oned1[3] = twod1[0];

    assert(twod1[0][0] == 1 );
    assert(twod2[0][0] == 1 );
    assert(oned1[0] == 1);

    assert(twod1[0][1] == 2 );
    assert(twod2[0][1] == 2 );
    assert(oned1[1] == 2);

    assert(twod1[0][2] == 3 );
    assert(twod2[0][2] == 3 );
    assert(oned1[2] == 3);

    oned1 = twod1[1];

    assert(twod1[1][0] == 4 );
    assert(twod2[1][0] == 4 );
    assert(oned1[0] == 4);

    assert(twod1[1][1] == 5 );
    assert(twod2[1][1] == 5 );
    assert(oned1[1] == 5);

    assert(twod1[1][2] == 6 );
    assert(twod2[1][2] == 6 );
    assert(oned1[2] == 6);

    float fourD[][][][] = {{{{.1,.2,.3},{.4,.5,.6}},
                            {{.01,.02,.03},{.04,.05,.06}}}};
    float threeD[][][] = fourD[0];
    float twoD[][] = threeD[0];
    float oneD[] = twoD[0];
    float f = oneD[0];

    assert(equalWithAbsErr(threeD[0][0][0], .1, .0001));
    assert(equalWithAbsErr(twoD[0][0], .1, .0001));
    assert(equalWithAbsErr(oneD[0], .1, .0001));
    assert(equalWithAbsErr(f, .1, .0001));
}

void
multidimensional2()
{
    bool t = true;
    bool f = false;

    bool b1[] =          {t, f, true, false};
    bool b2[][] =      { {b1[0], b1[1], true, false},{b1[0], b1[1], f, t}};
    bool b3[][][] =   {{ {b1[0], b1[1], true, false},{b1[0], b1[1], f, t}},
                       { {b1[1], b1[0], false, true},{b1[0], b1[1], f, t}}};

    bool bb1[4] = b1;
    bool bb2[2][4] = b2;
    bool bb3[2][2][4] = b3;

    assert(b1[0] == t);

    assert(b2[0][0] == b1[0]);
    assert(b2[0][1] == b1[1]);
    assert(b2[0][2] == true);
    assert(b2[0][3] == false);

    assert(b2[1][0] == b1[0]);
    assert(b2[1][1] == b1[1]);
    assert(b2[1][2] == f);
    assert(b2[1][3] == t);

    assert(b3[0][0][0] == b1[0]);
    assert(b3[0][0][1] == b1[1]);
    assert(b3[0][0][2] == true);
    assert(b3[0][0][3] == false);

    assert(b3[0][1][0] == b1[0]);
    assert(b3[0][1][1] == b1[1]);
    assert(b3[0][1][2] == f);
    assert(b3[0][1][3] == t);

    assert(b3[1][0][0] == b1[1]);
    assert(b3[1][0][1] == b1[0]);
    assert(b3[1][0][2] == f);
    assert(b3[1][0][3] == t);

    assert(b3[1][1][0] == b1[0]);
    assert(b3[1][1][1] == b1[1]);
    assert(b3[1][1][2] == f);
    assert(b3[1][1][3] == t);



}

void
multidimensional3()
{
    bool f = false;
    bool t = true;

    bool b3[2][2][4] =   {{ {t, f, t, f},{t, f, f, t}},
                          { {t, f, t, f},{t, f, f, t}}};


    return;
@error36    bool b3a[2][2][3] =   {{ {t, f, t, f},{t, f, f, t}},
@error36                           { {t, f, t, f},{t, f, f, t}}};
@error36    bool b3b[2][2][4] =   {{ {t, f, t   },{t, f, f, t}},
                                   { {t, f, t, f},{t, f, f, t}}};

}

void
multidimensional4()
{
     const int size = 4;

    struct Assorted
    {
 	int i;
 	float f;
 	bool b;
 	int ai[2];
 	float af[10];
 	bool ab[1+3];
 	bool ab2[size];
    };
    struct Nested
    {
 	Assorted a1;
 	Assorted a2;
	Assorted a3[4];
    };

    bool t = true;
    bool f = false;

    Nested nInit= 
    { 
      {1,.1,false,{0,1},{1,1,1,1,1,1,1,1,1,1},{t,t,t,t},{t,t,t,t}},
      {1,.1,false,{0,1},{1,1,1,1,1,1,1,1,1,1},{t,t,t,t},{t,t,t,t}},
      {
         {1,.1,false,{0,1},{1,1,1,1,1,1,1,1,1,1},{t,t,t,t},{t,t,t,t}},
         {1,.1,false,{0,1},{1,1,1,1,1,1,1,1,1,1},{t,t,t,t},{t,t,t,t}},
         {1,.1,false,{0,1},{1,1,1,1,1,1,1,1,1,1},{t,t,t,t},{t,t,t,t}},
         {1,.1,false,{0,1},{1,1,1,1,1,1,1,1,1,1},{t,t,t,t},{t,t,t,t}}
      }
    };

    Assorted a;
    a.i = 4;
    a.f = .2;
    a.ai[1] = 3;
    a.af[0] = 3.1;

    assert(a.i == 4);
    assert(a.f == .2);
    assert(a.ai[1] == 3);
    assert(a.af[0] == 3.1);

    Nested n;
    n.a1 = a;
    n.a2 = n.a1;
    assert(n.a1.af[3] == n.a2.af[3]);
    n.a1.af[3] = 1.033;
    assert(n.a1.af[3] != n.a2.af[3]);

    Nested an[10];
    an[4].a2.ab[1] = false ;
    assert( an[4].a2.ab[1] == false );

    an[4].a3[2].ab2[1] = true;
    assert( an[4].a3[2].ab2[1]  == true);

    // test array boundaries

    int ai[2];
    ai[0];
    ai[1];

    return;
    
@error14    ai[-1];
@error14    ai[2];
@error14    ai[1000];

    // these would only be caught at run-time (if above return statement)
    int i = -1;
 an[i].a2.ab[1];
 an[4].a2.ab[i];



@error14    n.a1.af[10];
 n.a1.af[i];


@error14 an[10].a2.ab[1];
@error14 an[4].a2.ab[4];

}


void
nested()
{
    struct Simple
    {
	int i;
    };

    struct Nest
    {
	Simple as[2][2];
    };

    Nest     n        = {{{{1},{2}},{{3},{4}}}};
    assert(n.as[0][0].i == 1);
    assert(n.as[0][1].i == 2);
    assert(n.as[1][0].i == 3);
    assert(n.as[1][1].i == 4);


    struct N1
    {
	Simple as[2][1];
    };

    Simple as[2][1] = {{{1}},{{1}}};
    N1     n1       = {{{{1}},{{1}}}};
    int   ai1[4][2] = { {3, 3},
                        {3, 3},
                        {3, 3},
                        {3, 3} };
    N1    an2[1]    = {{{{{1}},{{1}}}}};
    N1    an3[2][1] = {{{{{{1}},{{1}}}}},
		       {{{{{1}},{{1}}}}}};

    N1    an4[4][2] = { {{{{{1}}, {{2}}}},  {{{{3}}, {{4}}}}},
                        {{{{{5}}, {{6}}}},  {{{{7}}, {{8}}}}},
                        {{{{{9}}, {{10}}}}, {{{{11}},{{12}}}}},
                        {{{{{13}},{{14}}}}, {{{{15}},{{16}}}}} };


    assert(an4[0][0].as[0][0].i == 1);
    assert(an4[0][0].as[1][0].i == 2);
    assert(an4[0][1].as[0][0].i == 3);
    assert(an4[0][1].as[1][0].i == 4);

    assert(an4[1][0].as[0][0].i == 5);
    assert(an4[1][0].as[1][0].i == 6);
    assert(an4[1][1].as[0][0].i == 7);
    assert(an4[1][1].as[1][0].i == 8);

    N1    an5[][] = an4;

    assert(an5[0][0].as[0][0].i == 1);
    assert(an5[0][0].as[1][0].i == 2);
    assert(an5[0][1].as[0][0].i == 3);
    assert(an5[0][1].as[1][0].i == 4);

    assert(an5[1][0].as[0][0].i == 5);
    assert(an5[1][0].as[1][0].i == 6);
    assert(an5[1][1].as[0][0].i == 7);
    assert(an5[1][1].as[1][0].i == 8);

    struct N2
    {
	N1 n1;
    };

    N2 n2 = {{{{{1}},{{2}}}}};
    assert (n2.n1.as[0][0].i == 1);
    assert (n2.n1.as[1][0].i == 2);

}



void
parseErrors()
{
    return;     // don't generate code for syntax errors below

@error10            int ia[]; 
 int k; 
@error10            int ia0[1][]; 
 int M; 
@error10            int ia01[1][]; 
@error10            int ia02[][]; 
@error10            int ia03[][][][][]; 
@error10            int ia04[][3][9][10][]; 
@error12 @error10   int ia05[0][0][1];

@error12 @error10   int ia5[-12];
@error12 @error10   int ia6[0];

@error10            int ia1[] = 1;
@error36            int ia2[1] = {1,2};
@error29 @error4    ia2 = {1,2};

                    int ia3[1];
                    int ia4[2];
@error21            ia4 = ia3;

                    int i = ia3[0]; 
                    float f = ia3[0];
                    bool b = ia3[0];

		    int ia7;
@error4             ia7 = {1,2};
}


void
parseErrors2()
{
    return;     // don't generate code for syntax errors below

    int ia1[3]; 
    float fa1[3];
    bool ba1[3];

@error17     float fa2[] = ia1;
@error17     bool ba2[] = ia1;

@error17     bool ba3[] = fa1;
@error17     int  ia3[] = fa1;

@error17     int ia4[] = ba1;
@error17     float fa4[] = ba1;

    int a[2];
    int b[3];
@error21    a = b;

}


int
initY (output int y[][])
{
    for (int i = 0; i < y.size; i = i+1)
	for (int j = 0; j < y[i].size; j = j+1)
	    y[i][j] = i + j;

    return 0;
}


void
initZ (output int z[][])
{
    for (int i = 0; i < z.size; i = i+1)
	for (int j = 0; j < z[i].size; j = j+1)
	    z[i][j] = i + j * 2;
}


void
sideEffectInit1 ()
{
    int x[3], 3+5;		// doesn't actually initialize
    int y[3][4], initY (y);
    const int z[4][3], initZ (z);

    for (int i = 0; i < y.size; i = i+1)
	for (int j = 0; j < y[i].size; j = j+1)
	    assert (y[i][j] == i + j);

    for (int i = 0; i < z.size; i = i+1)
	for (int j = 0; j < z[i].size; j = j+1)
	    assert (z[i][j] == i + j * 2);
}


const int x[3][3], initY (x);


void
sideEffectInit2 ()
{
    for (int i = 0; i < x.size; i = i+1)
	for (int j = 0; j < x[i].size; j = j+1)
	    assert (x[i][j] == i + j);
}

void
testArrayCommas ()
{
    int ix[3]  = {1, 2, 3};
@error4    int ix2[3] = {1, 2 3};
@error4    int ix3[3] = {1, 2, 3,};
@error4    int ix4[3] = {, 1, 2, 3};

}



int
testArrays()
{
    simple();
    funcCalls();
    indexing();
    parseErrors();
    parseErrors2();
    assignments();
    multidimensional();
    multidimensional2();
    multidimensional3();
    multidimensional4();
    sideEffectInit1();
    sideEffectInit2();
    nested();
    return 1;
}

const int t = testArrays();



}  // namespace testArray
