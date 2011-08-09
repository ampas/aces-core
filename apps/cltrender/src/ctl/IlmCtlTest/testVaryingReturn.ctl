namespace testVaryingReturn
{

void
testIf(input varying bool in, output varying int out)
{
    if(in)
    {
	out = 1;
	return;
	assert(false);
    }
    out = 2;
}


void
testIf2(input varying bool in)
{
    if(in)
    {
	return;
    }
    else
    {
	return;
    }
    assert(false);
}


void
testWhile(input varying int in, output varying int out)
{
    while(in > 0)
    {
	out = out + 1;
	return;
    }

    out = -1;
}


void
testFor(input varying int in, output varying int out)
{
     for(int cond = in ; cond > 0; cond = cond - 1)
     {
 	out = out + 1;
 	return;
     }
     out = -1;
}

void
testNested(input varying int in1,
	    input varying int in2,
	    output varying int out1,
	    output varying int out2,
	    output varying int out3,
	    output varying int out4,
	    output varying int out5)
{
    int cond = in1;

    int i = 0;
    while (i < in2)
    {
	if(in1 > 0)
	{
	    out1 = out1 + 1;
	    return;
	}
	else
	{
	    out2 = out2 +1;
	}

	cond = cond -1;
	out3 = out3 + 1;
	return;
	out4 = out4 + 1;
	i= i+1;
    }
    out5 = out5 + 1;
}


void
testReturn(input varying bool ifIn,
	   output varying int ifOut,

	   input varying int whileIn,
	   output varying int whileOut,
	   
	   input varying int forIn,
	   output varying int forOut,
	   
	   input varying int  nestIn1,
	   input varying int  nestIn2,
	   output varying int nestOut1,
	   output varying int nestOut2,
	   output varying int nestOut3,
	   output varying int nestOut4,
	   output varying int nestOut5 )
{
    ifOut = 0;
    whileOut = 0;
    forOut = 0;
    nestOut1 = 0;
    nestOut2 = 0;
    nestOut3 = 0;
    nestOut4 = 0;
    nestOut5 = 0;

    testIf2(ifIn);
    testIf(ifIn, ifOut);
    if(ifIn)
    {
	assert(ifOut == 1);
    }
    else
    {
	assert(ifOut == 2);
    }


    assert(whileOut == 0);
    testWhile(whileIn, whileOut);
    if(whileIn > 0)
    {
	assert(whileOut == 1);
    }
    else
    {
	assert(whileOut == -1);
    }


    testFor(forIn, forOut);
    if(forIn > 0)
    {
 	// assumes forOut was originally 0
 	assert(forOut > 0);
    }
    else
    {
 	assert(forOut == -1);
    }
 
    testNested(nestIn1, nestIn2, nestOut1, 
 	      nestOut2, nestOut3, nestOut4, nestOut5);
    
}
    

} // namespace testVarying
