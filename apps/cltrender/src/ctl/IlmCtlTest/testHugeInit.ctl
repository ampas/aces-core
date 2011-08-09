namespace testHugeInit
{
//
//  to be loaded by testHugeInit.cpp, this file assumes that 
//  a ctl file has been created...
//



void
testVLA()
{
    assert(vla.size == vlaSize);
    print("very large array : size = ", vla.size, ".\n");


    assert(vla[1000] == 1000);

    assert(vla2D.size == 2);
    assert(vla2D[0].size == vlaSize);

    assert(vla2D[0][1000] == 1000);
    assert(vla2D[1][1000] == -1000);

}



int 
test()
{
    testVLA();
    return 1;
}

const int i = test();








} // namespace "testHugeInit"
