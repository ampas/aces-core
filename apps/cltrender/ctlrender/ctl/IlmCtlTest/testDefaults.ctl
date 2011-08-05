import "common";

namespace testDefaults
{


int
defaults1(int i = 2)
{
    return i;
}

int
defaults2(float f = 2.3, int i = 10)
{
    return i;
}

float
defaults3(int i = 2, bool b = false, float f = .001 )
{
    return f;
}


const int iaStatic[] = {1};


float
@error7 defaults4(int i = iaStatic, bool b = iaStatic, float f = .001 )
{
    return f;
}


const int staticAI[2] = {1,2};

int[2]
defaults5(int ai[2] = staticAI)
{
    return ai;
} 

int[2]
defaults6(int ai[2] = {1, 2})
{
    return ai;
} 



void
defaults7(int ai[] = {1, 2}, int i = 0)
{
    if(i == 0)
    {
	assert(ai.size == 2);
	assert(ai[0] == 1);
	assert(ai[1] == 2);
    }
    else
    {
	assert(ai.size == i);
	assert(ai[0] == 1);
	assert(ai[1] == 2);
    }
}

int[2]
@error40 defaults8(int ai[2] = {1, 2}, int j)
{
    return ai;
} 


void
@error41 defaults9(output varying int i = 2)
{
    i = 4;
} 




void
defaultValues()
{
    assert(defaults1() == 2);
    assert( 2 == defaults1( 2));
    assert( 1 == defaults1( 1));

    assert( defaults2() == 10);
    assert( defaults2(1.2) == 10);
    assert( defaults2(1.2,5) == 5);

    assert( defaults3() == .001);
    assert( defaults3(1, false) == .001);
    assert( defaults3(1, false, .2) == .2);

    assert( defaults5().size == 2 );
    assert( defaults5()[0] == 1 );
    assert( defaults5()[1] == 2 );

    int ai[] = {-20,5000};
    assert( defaults5(ai)[0] == -20 );
    assert( defaults5(ai)[1] == 5000 );
    
    assert( defaults6().size == 2 );
    assert( defaults6()[0] == 1 );
    assert( defaults6()[1] == 2 );

    int ai2[] = {1,2,3};
    defaults7();
    defaults7(ai2, 3);
    defaults7(ai2, (12/4));
    defaults7(ai2, ai2.size);

}


struct Simple
{
    int i;
};

struct Nest
{
    Simple as[2][2];
};


Simple[2][1]
defaultComplex(Simple as[2][1] = {{{2}},{{3}}})
{
    return as;
}



void
defaultComplex2( output Simple oas[][], Simple as[][] = {{{2}},{{3}}})
{
    assert(oas.size == as.size);
    int i = oas.size - 1;
    while( i >= 0)
    {
	assert(oas.size == as.size);
	int j = oas[i].size - 1;
	while( j >= 0)
	{
	    oas[i][j] = as[i][j];
	    j = j - 1;
	}
	i = i - 1;
    }
}

void
defaultComplex3( output Nest out, Nest in = {{{{1},{2}},{{3},{4}}}} )
{
    out = in;
}



const  
Nest staticIn[] = { {{{{1},{2}},{{3},{4}}}},{{{{5},{6}},{{7},{8}}}} };


void
defaultComplex4( output Nest out[], Nest in[] = staticIn )
{
    assert(out.size == in.size);

    int i = out.size - 1;
    while( i >= 0)
    {
	out[i] = in[i];
	i = i - 1;
    }
}

void
defaultComplex5( output Nest out[], 
		 Nest in[] = {{{{{10},{20}},{{3},{4}}}},{{{{5},{6}},{{7},{8}}}}})
{
    assert(out.size == in.size);

    int i = out.size - 1;
    while( i >= 0)
    {
	out[i] = in[i];
	i = i - 1;
    }
}

struct Hodgepodge
{
    float f;
    int i;
    bool b;
    bool ab[2];
    half h;
};


void
defaultComplex6(Hodgepodge i1 = {-1, -1, -1, {-1, -1}, -1},
		Hodgepodge i2 = {50, 50, 50, {50, 50}, 50},

		Hodgepodge f1 = {234.20, 234.20, 234.20, {234.20, 234.20}, 234.20},
		Hodgepodge f2 = {-23.5, -23.5, -23.5, {-23.5, -23.5}, -23.5},

		Hodgepodge b1 = {true, true, true, {true, true}, true},
		Hodgepodge b2 = {false, false, false, {false, false}, false},

		Hodgepodge op1 = {3*31.2-200, 3*31.2-200, 3*31.2-200, {3*31.2-200, 3*31.2-200}, 3*31.2-200},
		Hodgepodge op2 = {(false*36)/true, (false*36)/true, (false*36)/true, {(false*36)/true, (false*36)/true}, (false*36)/true})
{
    float f = -1;
    int i = -1;
    bool b = -1;
    half h = -1;

    assert( equalWithAbsErr(i1.f, f, .0001));
    assert( i1.i == i);
    assert( i1.b == b);
    assert( i1.ab[0] == b);
    assert( i1.ab[1] == b);
    assert( equalWithAbsErr(i1.h, h, .0001));

    f = 50;
    i = 50;
    b = 50;
    h = 50;

    assert( equalWithAbsErr(i2.f, f, .0001));
    assert( i2.i == i);
    assert( i2.b == b);
    assert( i2.ab[0] == b);
    assert( i2.ab[1] == b);
    assert( equalWithAbsErr(i2.h, h, .0001));


    f = 234.20;
    i = 234.20;
    b = 234.20;
    h = 234.20;

    assert( equalWithAbsErr(f1.f, f, .0001));
    assert( f1.i == i);
    assert( f1.b == b);
    assert( f1.ab[0] == b);
    assert( f1.ab[1] == b);
    assert( equalWithAbsErr(f1.h, h, .0001));


    f = -23.5;
    i = -23.5;
    b = -23.5;
    h = -23.5;

    assert( equalWithAbsErr(f2.f, f, .0001));
    assert( f2.i == i);
    assert( f2.b == b);
    assert( f2.ab[0] == b);
    assert( f2.ab[1] == b);
    assert( equalWithAbsErr(f2.h, h, .0001));


    f = true;
    i = true;
    b = true;
    h = true;

    assert( equalWithAbsErr(b1.f, f, .0001));
    assert( b1.i == i);
    assert( b1.b == b);
    assert( b1.ab[0] == b);
    assert( b1.ab[1] == b);
    assert( equalWithAbsErr(b1.h, h, .0001));


    f = false;
    i = false;
    b = false;
    h = false;

    assert( equalWithAbsErr(b2.f, f, .0001));
    assert( b2.i == i);
    assert( b2.b == b);
    assert( b2.ab[0] == b);
    assert( b2.ab[1] == b);
    assert( equalWithAbsErr(b2.h, h, .0001));

    f = 3*31.2-200;
    i = 3*31.2-200;
    b = 3*31.2-200;
    h = 3*31.2-200;

    assert( equalWithAbsErr(op1.f, f, .0001));
    assert( op1.i == i);
    assert( op1.b == b);
    assert( op1.ab[0] == b);
    assert( op1.ab[1] == b);
    assert( equalWithAbsErr(op1.h, h, .0001));

    f = (false*36)/true;
    i = (false*36)/true;
    b = (false*36)/true;
    h = (false*36)/true;

    assert( equalWithAbsErr(op2.f, f, .0001));
    assert( op2.i == i);
    assert( op2.b == b);
    assert( op2.ab[0] == b);
    assert( op2.ab[1] == b);
    assert( equalWithAbsErr(op2.h, h, .0001));
}




void
complexDefaultValues()
{

    Simple as1[2][1] = {{{10}},{{4}}};

    Simple as2[][] = defaultComplex(as1);
    assert(as2[0][0].i == 10);
    assert(as2[1][0].i == 4);

    Simple as3[][] = defaultComplex();
    assert(as3[0][0].i == 2);
    assert(as3[1][0].i == 3);
    
    Simple oas[2][1];

    defaultComplex2(oas);
    assert(oas[0][0].i == 2);
    assert(oas[1][0].i == 3);

    Simple as4[][] = {{{-2}},{{-3}}};
    defaultComplex2(oas, as4);
    assert(oas[0][0].i == -2);
    assert(oas[1][0].i == -3);


    Nest n = {{{{-1},{-2}},{{-3},{-4}}}};
    Nest nout;
    defaultComplex3(nout);
    assert(nout.as[0][0].i == 1);
    assert(nout.as[0][1].i == 2);
    assert(nout.as[1][0].i == 3);
    assert(nout.as[1][1].i == 4);

    defaultComplex3(nout, n);
    assert(nout.as[0][0].i == -1);
    assert(nout.as[0][1].i == -2);
    assert(nout.as[1][0].i == -3);
    assert(nout.as[1][1].i == -4);


    Nest an[] = {{{{{-1},{-2}},{{-3},{-4}}}},
                 {{{{-10},{-20}},{{-30},{-40}}}}};
    Nest anout2[2];
    defaultComplex4(anout2, an);
    assert(anout2[0].as[0][0].i == -1);
    assert(anout2[0].as[0][1].i == -2);
    assert(anout2[0].as[1][0].i == -3);
    assert(anout2[0].as[1][1].i == -4);
    assert(anout2[1].as[0][0].i == -10);
    assert(anout2[1].as[0][1].i == -20);
    assert(anout2[1].as[1][0].i == -30);
    assert(anout2[1].as[1][1].i == -40);


    Nest aout[2];
    defaultComplex4(aout);

    assert(aout.size == staticIn.size);
    assert(aout[0].as[0][0].i == 1);
    assert(aout[0].as[0][1].i == 2);
    assert(aout[0].as[1][0].i == 3);
    assert(aout[0].as[1][1].i == 4);
	
    assert(aout[1].as[0][0].i == 5);
    assert(aout[1].as[0][1].i == 6);
    assert(aout[1].as[1][0].i == 7);
    assert(aout[1].as[1][1].i == 8);
	

    Nest aout2[2];
    defaultComplex5(aout2);

    assert(aout2.size == 2);
    assert(aout2[0].as[0][0].i == 10);
    assert(aout2[0].as[0][1].i == 20);
    assert(aout2[0].as[1][0].i == 3);
    assert(aout2[0].as[1][1].i == 4);
	
    assert(aout2[1].as[0][0].i == 5);
    assert(aout2[1].as[0][1].i == 6);
    assert(aout2[1].as[1][0].i == 7);
    assert(aout2[1].as[1][1].i == 8);

    defaultComplex6();
}

int
defaultTest()
{
    defaultValues();
    complexDefaultValues();
    return 1;
}


const int rt = defaultTest();


}  // namespace testFunc
