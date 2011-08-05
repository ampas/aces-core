// Test if integer and floating-point literals are parsed correctly

int
testLiterals ()
{
    print ("Testing integer and floating-point literals\n");

    assert (!0);
    assert (1);
    assert (1 != 2);
    assert (1 != 10);
    assert (0x1 != 0x2);
    assert (0x1 != 0x10);
    assert (1.0 != 2.0);
    assert (1.0 != 1.1);
    assert (1e0 != 2e0);
    assert (1e0 != 1e1);
    assert (1e-1 != 1e1);
    assert (123 == 100 + 20 + 3);
    assert (-123 == -100 - 20 - 3);
    assert (0x1ae == 0x100 + 0xa0 + 0xe);
    assert (-0x1ae == -0x100 - 0xa0 - 0xe);
    assert (123 == 0X7B);
    assert (0X1AE == 430);
    assert (0.25 > 0 && 0.25 < 1);
    assert (0.25 * 100 == 25);
    assert (2.25 * 100 == 225);
    assert (.0 == 0);
    assert (0.0 == 0);
    assert (0. == 0);
    assert (2. == 2);
    assert (2.0 == 2);
    assert (.25 == 0.25);
    assert (0.25e0 == 0.25);
    assert (0.25e00 == 0.25);
    assert (0.25e2 == 25);
    assert (2.25e2 == 225);
    assert (2.e2 == 200);
    assert (25e-2 == 0.25);
    assert (25e+2 == 2500);
    assert (25E+03 == 25000);
    assert (0.25h > 0 && 0.25h < 1);
    assert (0.25H * 100 == 25);
    assert (2.25h * 100 == 225);
    assert (.0h == 0);
    assert (0.0h == 0);
    assert (0.h == 0);
    assert (2.H == 2);
    assert (2.0h == 2);
    assert (.25h == 0.25);
    assert (0.25e0h == 0.25);
    assert (0.25e00H == 0.25h);
    assert (0.25e2h == 25);
    assert (2.25e2h == 225);
    assert (2.e2h == 200);
    assert (25e-2h == 0.25);
    assert (25e+2H == 2500);
    assert (25E+03h == 25000);
    assert (010 == 8);


    half h = 2.0h;

@error4    float f = 2.0f;   
@error4    float f1 = 2.0f;  
@error4    float f2 = 2.0l;  
@error4    float f3 = 2.0l;  

   
    print ("\"\''\t\n");
    print (" line 1 \n"
           " line 2\n");

    print ("string " /* comment */);
    print ("str" /* comment */ "ing ");

    print ("one "
           // comment in the middle
           "two "
                                    "three "
           """""""");
    print ("one "
           /* one
              two */
           "two "
                                    "three"
           "\n""""""");

    bool b1 = true;
    bool b2 = false;

    print ("ok\n");
    return 0;


}


const int testLiteralsDummy = testLiterals();
