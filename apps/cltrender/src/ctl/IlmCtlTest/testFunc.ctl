namespace testFunc
{


// Test various return types, function passing and locals
// ** ints  **
int
returnIntNoArgs()
{
    return 1;
}

int
returnIntWArgs(int a, float b, bool c)
{
    return a;
}


int
returnIntWArgsLocals(int a, float b, bool c)
{
    int i;
    int j = 2;
    int k = a;
    assert(k == a);
    int l = 2 -1;
    int m = 2 - a/2;
    return a;
}

void
setIntWArgs(output int a, input int b)
{
    a = b;
}

// ** floats  **
float
returnFloatNoArgs()
{
    return 1.0;
}

float
returnFloatWArgs(int a, float b, bool c)
{
    return b;
}

float
returnFloatWArgsLocals(int a, float b, bool c)
{
    float f;
    float g = 2.0;
    g = 2;
    float h = 2;
    float i = true;
    return b*g;
}


void
setFloatWArgs(output float a, input float b)
{
    a = b;
}


// ** bools  **
bool
returnBoolNoArgs()
{
    return false;
}

bool
returnBoolWArgs(int a, float b, bool c)
{
    return c;
}


void
setBoolWArgs(output bool a, input bool b)
{
    a = b;
}



// functions without return values

int
noReturnNoBody() @error5 
{
}

int
noReturnNoBody2() @error5 
{
    int i;
}

int
noReturnWBody() @error5 
{
    int i = 2;
}



void
returns()
{

    const int RINA  = returnIntNoArgs();

    int arg = 5;
    const int RIWA  = returnIntWArgs(arg,4.0,true);
    const int RIWAL = returnIntWArgsLocals(2,4.0,true);
    assert(RINA == 1);
    assert(RIWA == 5);
    assert(RIWAL == 2);

    assert(returnIntNoArgs() == 1);
    assert(returnIntWArgs(2,2.0,false) == 2);


    const float RFNA  = returnFloatNoArgs();
    const float RFWA  = returnFloatWArgs(5,4.0,true);
    const float RFWAL = returnFloatWArgsLocals(5,4.0,true);
    assert(RFNA == 1.0);    
    assert(RFWA == 4.0);
    assert(RFWAL == 8);
    return;

    const bool RBNA = returnBoolNoArgs();
    const bool RBWA = returnBoolWArgs(5,4.0,true);
    assert(RBNA == false);    
    assert(RBWA == true);

    assert(returnIntNoArgs() == false);
    assert(returnIntWArgs(2,2.0,false) == false);
}


// Casting arguments to functions
void
castedArgs()
{
    // Cast to Int
    {
	float in = 1.0002;
	int out = returnIntWArgs(in, in, in);
	int out2 = in;
	assert(out2 == out);
    }
    {
	bool in = false;
	int out = returnIntWArgs(in, in, in);
	int out2 = in;
	assert(out2 == out);
    }
    {
	bool in = true;
	int out = returnIntWArgs(in, in, in);
	int out2 = in;
	assert(out2 == out);
    }
    {
	half in = 1.002;
	int out = returnIntWArgs(in, in, in);
	int out2 = in;
	assert(out2 == out);
    }


    // Cast to float
    {
	int in = -4000;
	float out = returnFloatWArgs(in, in, in);
	float out2 = in;
	assert(out2 == out);
    }
    {
	bool in = false;
	float out = returnFloatWArgs(in, in, in);
	float out2 = in;
	assert(out2 == out);
    }
    {
	bool in = true;
	float out = returnFloatWArgs(in, in, in);
	float out2 = in;
	assert(out2 == out);
    }
    {
	half in = -1.999;
	float out = returnFloatWArgs(in, in, in);
	float out2 = in;
	assert(out2 == out);
    }


    // Cast to bool
    {
	int in = -4000;
	bool out = returnBoolWArgs(in, in, in);
	bool out2 = in;
	assert(out2 == out);
    }
    {
	float in = -999999.99999;
	bool out = returnBoolWArgs(in, in, in);
	bool out2 = in;
	assert(out2 == out);
    }
    {
	half in = -1.999;
	bool out = returnBoolWArgs(in, in, in);
	bool out2 = in;
	assert(out2 == out);
    }



    return;

    // try to pass in output arguments of the wrong type
    {
	half in = 1.002;
	half out = 3.4;
@error27	setIntWArgs(out, in);
    }

    {
	float in = -2200;
	float out = 3.4;
@error27	setIntWArgs(out, in);
    }

    {
	bool in = true;
	bool out = false;
@error27	setIntWArgs(out, in);
    }
}




void
oneOut(output int out)
{
    out = 3;
}
void
oneIn(input int in)
{
@error20    in = 3;
}

void
oneOutOneIn(output int out, input int in)
{

}


const int staticInt = 2;

const int staticAI[] = {1};

struct Simple
{
    int i;
};


void
funcStatics()
{
    return;
    const int staticInt2 = 2;
    const Simple s = {2};

@error26   oneOut(staticInt);
@error26   oneOut(staticInt2);
@error26   oneOut(staticAI[0]);
@error26   oneOut(s.i);
}

void
@error4 testFuncWNoCommaParams (int a int b int c)
{
    
}

void
testFuncCommas ()
{
@error4    returnIntWArgs(1 2 3);
@error4    returnIntWArgs(1, 2, 3,);
@error4    returnIntWArgs(1, 2 3);
    testFuncWNoCommaParams (1,2,3);
}



int
funcTest ()
{
    testFuncCommas ();
    funcStatics ();
    castedArgs ();
    returns ();
    return 1;
}


const int rt = funcTest();


}  // namespace testFunc
