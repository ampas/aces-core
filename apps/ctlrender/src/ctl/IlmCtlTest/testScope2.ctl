import "testName";


namespace testScope2
{


const int i = 10;

void
testScope1()
{
    assert (i == 10);
    int i = 0;

    {
        int i = 1;

        {
            int i = 2;
            {
                assert (i == 2);
                assert (testScope2::i == 10);
                assert (testName::i == 20);
            }
            assert (i == 2);
        }
        assert (i == 1);
    }
    assert (i == 0);
}

int
testScope()
{
    print ("Testing more variable scope\n");
    testScope1();

    print ("ok\n");
    return 0;
}

const int testScope2Dummy = testScope();


} // namespace testScope
