namespace arrayTest
{


void
i2b()
{
    int i = -3;
    bool b = -3;
    assert(b);
    b = i;
    assert(b);
    assert(i);
}

void
b2i()
{
    bool b1 = false;
    bool b2 = true;
    int i1 = b1;
    int i2 = b2;
    assert(i1 == 0);
    assert(i2 == 1);
    assert(b1 == 0);
    assert(b2 == 1);
}

void
f2b()
{
    float f = 200.3;
    bool b2 = f;
    assert(b2);
    assert(f);
    assert(0.00001);
}

void
b2f()
{
    bool b1 = false;
    bool b2 = true;
    float f1 = b1;
    float f2 = b2;
    assert(f1 == 0.0);
    assert(f2 == 1.0);
    assert(b1 == 0.0);
    assert(b2 == 1.0);
}

void
f2i()
{
    int i1 = -0.0004;
    int i2 = .8;
    int i3 = 1.4;
    assert(i3 == 1);
    assert(1 == i3);
    assert(i2 != i3);
    assert(i1 == i2);
    assert(i2 == 0);
    assert(!i2);

}

void
i2f()
{
    int i1 = 1;
    int i2 = 0;
    float f1 = i1;
    float f2 = i2;
    assert(f1 == 1.0);
    assert(f2 == 0.0);

    assert(-1.4 != -1);
    assert(0.0 == 0);
    assert(1.0 == true);
    assert(true == 1.0);
    assert(0.0 == false);
    assert(false == 0.0);
}





void
a2x()
{
    int i1 = 1;
    int b1 = true;
    float f1 = false;

@error10    int ia1[3] = i1;
@error10    int ia2[]  = b1;
@error10    int ia3[1] = f1;

}

void
x2a()
{
}


int
testCasts()
{
    i2b();
    b2i();
	
    f2b();
    b2f();
    
    f2i();
    i2f();

    a2x();
    x2a();
    return 1;
}


const int tc = testCasts();


}  // namespace arrayTest
