namespace testExpr
{

float zero_f () {return 0;}
int zero_i () {return 0;}

int
testExpr()
{
    print ("Testing divisions by zero\n");
    
    // compile-time

    @error43 assert (1/0 == 0);
    @error43 assert (1/0.0 == FLT_POS_INF);
    @error43 assert (-1/0.0 == FLT_NEG_INF);
    @error43 assert (isnan_f (0/0.0));

    // run-time

    assert (1/zero_i() == 0);
    assert (1/zero_f() == FLT_POS_INF);
    assert (-1/zero_f() == FLT_NEG_INF);
    assert (isnan_f (0/zero_f()));

    print ("ok\n");
    return 0;
}

const int t = testExpr();

}  // namespace testExpr
