import "common";

namespace testStruct
{

struct OneInt
{
    int i;
};

struct Sins
{
    OneInt oi;
};


struct TwoInts
{
    int i;
    int j;
};

const int size = 10;
struct Assorted
{
    int i;
    float f;
    bool b;
    int ai[100];
    float af[10];
    bool ab[1+3];
    bool ab2[size];
    OneInt oi;
};


struct UndeterminedSize
{
@error29 @error13 @error30    int nope[k];
@error30                      int i[];
@error29                      undeterminedSize us1;
@error29                      undeterminedSize us2[1];
};




void
parseStruct1()
{
    struct OneInt
    {
	int i;
    };

    struct Nothing
    {
    };
    Nothing n;

    struct Sins
    {
	float f;
    };
    
    Sins s1;
    Sins s2;
    s1 = s2;


    return;

    testStruct::Sins outside;
    Sins inside;
    @error21    outside = inside;


    OneInt oi;
    testStruct::OneInt oiGlobal;
}

struct Simple
{
    int i;
};

int
returnMember(Simple s)
{
    return s.i;
}

void
memberAccess()
{
    Simple s1;
    s1.i = 2;
    assert(returnMember(s1) == 2);
}

void
invalidMemberAccess()
{
@error29 @error20	a.b = 1;
    			float c;
@error34		c.d = 2;
@error34		c.d.e = 3;
}

void
alignment()
{
    half h = .3;
    assert(equalWithAbsErr(h, .3, .0001));

    struct ATest
    {
	int i;
	half h;
    };

    ATest aat[3] = {{1,.5},{1,.5},{1,.5}};
    assert(equalWithAbsErr(aat[0].h, .5, .0001));
    assert(equalWithAbsErr(aat[1].h, .5, .0001));
}


void
testConst1()
{
    struct myStruct
    {
        int i;
    };
    const myStruct s = {1};
@error20   s.i = 2; 
}



void
testStructCommas()
{
@error4    const Chromaticities c1 = { {0,0},{0,0},{0,0}{0,0}};
@error4    const Chromaticities c1b = { {0,0},{0,0},{0,0},,{0,0}};
@error4    const Chromaticities c1b = { {0,0},{0,0},{0,0},{0,0},};

    struct C2
    {
        float t;
        float u;
        float r[2];
        float g[2];
        float b[2];
        float a[2];
    };

@error4    const C2 c2 = { 1 3 {0 0} {0,0}{0,0}{1,0}};
@error4    const C2 c2b = { 1, 3, {0 0}, {0,0},{0,0},{1,0}};
@error4    const C2 c2c = { 1, 3, {0, 0}, {0,0}{0,0},{1,0}};
@error4    const C2 c2d = { 1, 3, {0, 0}, {0,0},{0,0},{1,0},};
@error4    const C2 c2e = { 1, 3, {0, 0,}, {0,0},{0,0},{1,0}};
@error4    const C2 c2f = {, 1, 3, {0, 0}, {0,0},{0,0},{1,0}};
}


int
testArrays()
{
    memberAccess();
    parseStruct1();
    alignment();
    return 1;
}

const int t = testArrays();



}  // namespace testArray
