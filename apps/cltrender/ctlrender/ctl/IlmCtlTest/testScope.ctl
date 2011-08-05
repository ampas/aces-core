import "testName";

// void
// testScope1()
// {
// @error20 @error29    i = 2;
//     int i;
// }

void
testScope2()
{
    int i = 0;
    {
        assert( i == 0 );
        i = 2;
        assert( i == 2 );
        int i = 3;
        assert( i == 3 );
    }
    assert(i == 2);

}


const int i = 10;

void
testScope3()
{
    int i = 0;

    {
        int i = 1;

        {
            int i = 2;
            {
                assert (i == 2);
                assert (::i == 10);
                assert (testName::i == 20);
            }
            assert (i == 2);
        }
        assert (i == 1);
    }
    assert (i == 0);
}



void
testCall1(output int i)
{
    i = 1;
}


// void
// testScope3()
// {
//     int i = 0;
//     testCall1(i);
//     assert(i == 1);
// @error29    testCall2(i);
// }


void
testCall2(output int i)
{
    i = 2;
}




int 
testScope()
{
    print ("Testing variable scope\n");
    testScope2();
    testScope3();

    print ("ok\n");
    return 0;
}

const int testScopeDummy = testScope();


