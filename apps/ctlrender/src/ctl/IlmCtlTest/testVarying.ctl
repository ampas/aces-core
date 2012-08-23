import "common";


namespace testVarying
{

void
boolOps(input varying bool a,
	input varying bool b,
	output varying bool equals,
	output varying bool not,
  	output varying bool bitAnd,
  	output varying bool and,
  	output varying bool bitOr,
  	output varying bool or,
  	output varying bool exOr,
  	output varying bool greater,
  	output varying bool greaterEquals,
  	output varying bool less,
  	output varying bool lessEquals,
  	output varying bool notEquals)
{
    equals = (a == b);
    not = !a;

    bitAnd = (a & b);
    and = (a && b);
    
    bitOr = (a | b);
    or = (a || b);
    
    exOr = (a ^ b);
   
    greater = (a > b);
    greaterEquals = (a >= b);
    less = (a < b);
    lessEquals = (a <= b);
    notEquals = (a != b);
}

void
floatOps(input varying float a,
	input varying float b,
	output varying bool equals,
	output varying bool not,
  	output varying bool and,
  	output varying bool or,
  	output varying bool greater,
  	output varying bool greaterEquals,
  	output varying bool less,
  	output varying bool lessEquals,
  	output varying bool notEquals)
{
    equals = (a == b);
    not = !a;
    and = (a && b);
    or = (a || b);
   
    greater = (a > b);
    greaterEquals = (a >= b);
    less = (a < b);
    lessEquals = (a <= b);
    notEquals = (a != b);


}

void
halfOps(input varying half a,
	input varying half b,
	output varying bool equals,
	output varying bool not,
  	output varying bool and,
  	output varying bool or,
  	output varying bool greater,
  	output varying bool greaterEquals,
  	output varying bool less,
  	output varying bool lessEquals,
  	output varying bool notEquals)
{
    equals = (a == b);
    not = !a;
    and = (a && b);
    or = (a || b);
   
    greater = (a > b);
    greaterEquals = (a >= b);
    less = (a < b);
    lessEquals = (a <= b);
    notEquals = (a != b);


}


void
intOps(input varying int a,
	input varying int b,
	output varying bool equals,
	output varying bool not,
  	output varying int bitAnd,
  	output varying bool and,
  	output varying int bitOr,
  	output varying bool or,
  	output varying int exOr,
  	output varying bool greater,
  	output varying bool greaterEquals,
  	output varying bool less,
  	output varying bool lessEquals,
  	output varying bool notEquals)
{
    equals = (a == b);
    not = !a;
    bitAnd = (a & b);
    and = (a && b);
    bitOr = (a | b);
    or = (a || b);
    exOr = (a ^ b);
   
    greater = (a > b);
    greaterEquals = (a >= b);
    less = (a < b);
    lessEquals = (a <= b);
    notEquals = (a != b);
}


void
unsignedOps(input varying unsigned a,
	input varying unsigned b,
	output varying bool equals,
	output varying bool not,
  	output varying unsigned bitAnd,
  	output varying bool and,
  	output varying unsigned bitOr,
  	output varying bool or,
  	output varying unsigned exOr,
  	output varying bool greater,
  	output varying bool greaterEquals,
  	output varying bool less,
  	output varying bool lessEquals,
  	output varying bool notEquals)
{
    equals = (a == b);
    not = !a;
    bitAnd = (a & b);
    and = (a && b);
    bitOr = (a | b);
    or = (a || b);
    exOr = (a ^ b);
   
    greater = (a > b);
    greaterEquals = (a >= b);
    less = (a < b);
    lessEquals = (a <= b);
    notEquals = (a != b);
}




float
f (float x)
{
    return x * x;
}


float
g (float x, float y)
{
    return pow (x, y);
}


void
functionCalls (varying float x,
	       varying float y,
	       varying float z,
	       varying float w,
	       output varying float a,
	       output varying float b,
	       output varying float c)
{
    a = sqrt (x);
    b = testVarying::f (y);
    c = testVarying::g (z, w);
}


//
//  an object for which the aligned object size is different from
//  the object size
struct Hodgepodge
{
    float f;
    int i;
    bool b;
    bool ab[2];
    half h;
};



void
structVary(input varying Hodgepodge hoAIn,
	   input varying Hodgepodge hoBIn,
	   output varying Hodgepodge hoOut,
	   output varying float f,
	   output varying int i,
	   output varying bool b,
	   output varying half h
	  )
{
    Hodgepodge hLocal = hoAIn;
    hoOut = hLocal;
//    hoOut = hoAIn;
    
    f     = hoAIn.f;
    i     = hoAIn.i;
    b     = hoAIn.b;
    h     = hoAIn.h;

    assert(equalWithAbsErr(hoOut.f, hoAIn.f, .0001));
    assert(hoOut.i == hoAIn.i);
    assert(hoOut.b == hoAIn.b);
    assert(equalWithAbsErr(hoOut.h, hoAIn.h, .0001));

}

const Hodgepodge hodGlobal = {-2.0, 3, false, {true, false}, .001};
const Hodgepodge ahodGlobal[] = {{1.0, -3, false, {false, false}, .4},
                                 {1.3, -2, true, {true, true}, .1}};

void
varyingDefaults(output varying bool  b1Out,
		output varying bool  b2Out,
		output varying float f1Out,
		output varying float f2Out,
		output varying half  h1Out,
		output varying half  h2Out,
		output varying int   i1Out,
		output varying int   i2Out,

		output varying Hodgepodge hod1Out,
		output varying Hodgepodge hod2Out,
		output varying Hodgepodge ahod1Out[2],
		output varying Hodgepodge ahod2Out[2],

		input varying  bool  b1In = true,
		input varying  bool  b2In = hodGlobal.ab[1], //  false,
		input varying  float f1In = 1.022,
		input varying  float f2In = -12399999,
		input varying  half  h1In = -.00001,
		input varying  half  h2In = 23,
		input varying  int   i1In = -700,
		input varying  int   i2In = 46,

		input varying  Hodgepodge hod1In = {1.2, 1, false, {false, true}, .342},
		input varying  Hodgepodge ahod1In[2] = {{-1.2, -1, true, {true, true}, -.342},{-1.2, -1, true, {true, true}, -.342}},
		input varying  Hodgepodge hod2In = ahodGlobal[1],
		input varying  Hodgepodge ahod2In[2] = {{-1.2, -1, true, {true, false}, -.342},{1.2, 1, false, {false, true}, .342}})
{
    b1Out = b1In;
    b2Out = b2In;
    f1Out = f1In;
    f2Out = f2In;
    h1Out = h1In;
    h2Out = h2In;
    i1Out = i1In;
    i2Out = i2In;

    hod1Out = hod1In;
    hod2Out = hod2In;

    ahod1Out = ahod1In;
    ahod2Out = ahod2In;
}


int
testDefaults()
{
    bool b1Out = false;
    bool b2Out = true;
    bool b1In  = true;
    bool b2In  = false;

    float f1Out = .1;
    float f2Out = .2;
    float f1In = .3;
    float f2In = .4;

    half h1Out = .5;
    half h2Out = .6;
    half h1In = .7;
    half h2In = .8;

    int i1Out = 1;
    int i2Out = 2;
    int i1In = 3;
    int i2In = 4;

    Hodgepodge hod1Out;
    Hodgepodge hod2Out;
    Hodgepodge ahod1Out[2];
    Hodgepodge ahod2Out[2];

    Hodgepodge hod1In = {-1.2, -1, true, {true, true}, -.342};
    Hodgepodge hod2In = {-1.2, -1, true, {true, true}, -.342};;
    Hodgepodge ahod1In[2] = {{1.2, 1, false, {false, true}, .342},{1.3, 2, true, {true, true}, .1}};
    Hodgepodge ahod2In[2] = {{1.2, 1, false, {false, true}, .342},{1.3, 2, true, {true, true}, .1}};

    varyingDefaults( b1Out, b2Out, f1Out, f2Out, h1Out, h2Out, i1Out, i2Out,
		     hod1Out, hod2Out, ahod1Out, ahod2Out,
		     b1In,  b2In,  f1In,  f2In,  h1In,  h2In, i1In, i2In,
		     hod1In, ahod1In); //params hod2In, ahod2In use defaults


    assert(b1Out == b1In);
    assert(b2Out == b2In);

    assert(equalWithAbsErr(f1Out, f1In, .0001));
    assert(equalWithAbsErr(f2Out, f2In, .0001));

    assert(equalWithAbsErr(h1Out, h1In, .0001));
    assert(equalWithAbsErr(h2Out, h2In, .0001));

    assert(i1Out == i1In);
    assert(i2Out == i2In);

    assert(equalWithAbsErr(hod1Out.f, hod1In.f, .0001));
    assert(hod1Out.i == hod1In.i);
    assert(hod1Out.b == hod1In.b);
    assert(equalWithAbsErr(hod1Out.h, hod1In.h, .0001));

    assert(equalWithAbsErr(hod2Out.f, 1.3, .0001));
    assert(hod2Out.i == -2);
    assert(hod2Out.b == true);
    assert(hod2Out.ab[0] == true);
    assert(hod2Out.ab[1] == true);
    assert(equalWithAbsErr(hod2Out.h, .1, .0001));
    return 1;
}
const int i = testDefaults();



} // namespace testVarying
