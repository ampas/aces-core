import "testExamplesNamespace";

//
// examples from the CTL manual

const int i1 = 3;

void myFunc()
{
    int i = ::i1;        assert(i == 3);
    int j = MyLib::i1;   assert(j == 0);
}


struct Mixed
{
   int i;
   float f;
   bool b;
   half ah[2];
};


bool someBooleanFunc(output float fArg, half hArg)
{
    //  hArg = 2.0;  // error, half is an input parameter

    if(hArg > fArg)
        return false;
    else
    {
        fArg = hArg;
        return true;
    }
}


void variableDefinitions()
{
    int i = 0;
    half h = 1.201e2;
    float f = 2.1*h;
    bool b = someBooleanFunc(f,h);

    Mixed m1;
    Mixed m2 = {1, 1.0, false, {1,2}};
    //@error4    Mixed m3 = {1, 1.0} // error in ctl (ok in C++)

    Mixed am1[2] =     {{1, 1.0, false, {1,2}}, 
                        {2, -1.0, true, {-1,-2}}};
    Mixed am2[2][1] = {{{1, 1.0, false, {1,2}}}, 
                       {{2, -1.0, true, {-1,-2}}}};
    Mixed am3[1][2] = {{{1, 1.0, false, {1,2}}, 
                        {2, -1.0, true, {-1,-2}}}};

    Mixed am3b[][] = {{{1, 1.0, false, {1,2}}, 
                       {2, -1.0, true, {-1,-2}}}};
}

void
copyWDefaults(output int aiOut[2], int aiIn[2] = {1, 2})
{
    for(int i = 0; i < 2; i=i+1)
    {
        aiOut[i] = aiIn[i];
    }
}

void
copyVarying(output int aiOut[], int aiIn[] = {1, 2})
{
    if( aiOut.size != aiIn.size)
        return;

    for(int i = 0; i < aiOut.size; i=i+1)
    {
        aiOut[i] = aiIn[i];
    }
}

void
parameterDefaults()
{
    int ai[2];
    int ai2[] = {1,2};
    copyWDefaults(ai, ai2);
    assert(ai[0] == 1);
    assert(ai[1] == 2);

}

void
varyingBody(output int ai1[][2], output int ai2[][3], output int ai3[][2])
{
@error21    ai1 = ai2;       // int [][2] != int [][3] - error
@error21    ai1 = ai3;       // int [][2] != int [][2] - error
    ai1[0] = ai3[0]; // int [2] == int [2] - ok
@error10    int aiLocal1[][2] = ai1;

@error10 @error13    int aiLocal[ai1.size][2];  // size can not be determined at compile time
}


void
testArraySize(int aiArg[1][][2])
{
    assert(aiArg.size == 1);
    assert(aiArg[0].size == 3);
    assert(aiArg[0][0].size == 2);
}    

void
variableSizeArrays()
{
    int ai[2];
    int ai2[] = {3,4};
    copyVarying(ai, ai2);
    assert(ai[0] == 3 && ai[1] == 4);

    int ai3[] = {-1};
    int ai4[] = {-2};
    copyVarying(ai3);
    assert(ai3[0] == -1);
    copyVarying(ai3, ai4);
    assert(ai3[0] == -2);

    int ai3d [1][3][2];
    testArraySize(ai3d);

    int ai3d2 [1][2][2];
    //    testArraySize(ai3d2);  // assert fails
}

void
statements()
{

    // while statement
    int i;
    while(i < 5)
    {
        i = i+1;
        print(i, " ");
    }
    print ("\n");
    
    // print statement
    bool b = false;
    float f = -.00000012;
    print("b = ", b, ", f = ", f);
    print(", random literals: ", 4, ", ", 3.0001h,"\n");
}

void
printExample(input varying float f)
{
    print("all f: ", f, "\n");
    if(f > 1)
        print ("f > 1: ", f, "\n");
}

void
sampleExpressions()
{
    struct Inside  { int i[2]; };
    struct Outside { Inside ain[2][3];  };

    Inside in = {{1,2}};
    Outside out = { {{{{0,1}},{{2,3}},{{4,5}}},{{{6,7}},{{8,9}},{{10,11}}}} };
    in = out.ain[0][1];
    assert(in.i[0] == out.ain[0][1].i[0]);
}

void
sampleAssignments()
{

    int ai1[1][2];
    int ai2[2] = {1,2};
    ai1[0] = ai2;
}

void
typeConversion()
{
    int i = 2.7;
    float f = 2.3;
    if(f > i)
        i = 1.7;
    assert(i == 1);

    // ~f;
}


bool 
testExamples()
{
    variableDefinitions();
    variableSizeArrays();
    parameterDefaults();
    statements();
    sampleExpressions();
    sampleAssignments();
    myFunc();
    typeConversion();
    return true;
}

const bool testExamplesDummy = testExamples();
