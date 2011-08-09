// Test while and for loops

namespace testLoops {


int
whileLoopWithReturn ()
{
    int i = 1;
    int j = 0;

    while (true)
    {
	j = j + i;
	i = i + 1;

	if (i > 10)
	    return j;
    }
}


int
forLoopWithReturn ()
{
    int j = 0;

    for (int i = 0; true; i = i + 1)
    {
	j = j + i;

	if (i >= 10)
	    return j;
    }
}


void
increment (output int i)
{
    i = i + 1;
}


void
whileLoops ()
{
    {
	//
	// 10 iterations
	//

	int i = 1;
	int j = 0;

	while (i <= 10)
	{
	    j = j + i;
	    i = i + 1;
	}

	assert (j == 55);
    }

    {
	//
	// zero iterations
	//

	int i = 1;
	int j = 0;

	while (i <= -10)
	{
	    j = j + i;
	    i = i + 1;
	}

	assert (j == 0);
    }

    {
	//
	// zero iterations, found at compile tme
	//

	int i = 1;
	int j = 0;

	while (false)
	{
	    j = j + i;
	    i = i + 1;
	}

	assert (j == 0);
    }

    {
	int j = whileLoopWithReturn();
	assert (j == 55);
    }
}


void
forLoops ()
{
    {
	//
	// 10 iterations, update is assignment
	//

	int i = 1;
	int j = 0;

	for (i = 1; i <= 10; i = i+1)
	    j = j + i;

	assert (i == 11);
	assert (j == 55);
    }

    {
	//
	// 10 iterations, update is simpleExprStatement
	//

	int i = 1;
	int j = 0;

	for (i = 1; i <= 10; increment (i))
	    j = j + i;

	assert (i == 11);
	assert (j == 55);
    }

    {
	//
	// 10 iterations, with local scopes
	//

	int i = 1;
	int j = 0;
	int k = 0;

	for (int i = 1; i <= 10; i = i+1)	// i shadows variable
	{					// outside loop
	    j = j + i;

	    int i = 0;				// i shadows i in loop
	    					// header
	    k = k + i;
	}

	assert (i == 1);
	assert (j == 55);
	assert (k == 0);
    }

    {
	//
	// zero iterations
	//

	int j = 0;

	for (int i = 0; i <= -1; i = i+1)
	    j = j + i;

	assert (j == 0);
    }

    {
	//
	// zero iterations, found at compile time
	//

	int j = 0;

	for (int i = 0; false; i = i+1)
	    j = j + i;

	assert (j == 0);
    }

    {
	int j = forLoopWithReturn();
	assert (j == 55);
    }
}


int
testLoops ()
{
    print ("Testing while and for loops\n");
    whileLoops();
    forLoops();
    print ("ok\n");
    return 0;
}


const int testLoopsDummy = testLoops();

} // namespace
