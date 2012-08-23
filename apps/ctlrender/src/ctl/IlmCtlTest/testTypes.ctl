//
// Test if operations on basic types such as bool or int work as
// expected.  We need to test compile-time evaluation where the
// CTL interpreter pre-computes the values of constant arithmetic
// expressions, and run-time evaluation, where the interpreter
// generates code to compute the values of expressions.
//

void
testBoolRunTimeEvaluation ()
{
    print ("Testing operations on type bool, run-time\n");

    bool a = false;
    bool b = true;

    assert (~a == true);
    assert (~b == false);

    assert (!a == true);
    assert (!b == false);

    assert ((a & a) == false);
    assert ((a & b) == false);
    assert ((b & b) == true);

    assert ((a && a) == false);
    assert ((a && b) == false);
    assert ((b && b) == true);

    assert ((a | a) == false);
    assert ((a | b) == true);
    assert ((b | b) == true);

    assert ((a || a) == false);
    assert ((a || b) == true);
    assert ((b || b) == true);

    assert ((a ^ a) == false);
    assert ((a ^ b) == true);
    assert ((b ^ b) == false);

    assert ((a == a) == true);
    assert ((a == b) == false);
    assert ((b == a) == false);
    assert ((b == b) == true);

    assert ((a > a) == false);
    assert ((a > b) == false);
    assert ((b > a) == true);
    assert ((b > b) == false);

    assert ((a >= a) == true);
    assert ((a >= b) == false);
    assert ((b >= a) == true);
    assert ((b >= b) == true);

    assert ((a < a) == false);
    assert ((a < b) == true);
    assert ((b < a) == false);
    assert ((b < b) == false);

    assert ((a <= a) == true);
    assert ((a <= b) == true);
    assert ((b <= a) == false);
    assert ((b <= b) == true);

    assert ((a != a) == false);
    assert ((a != b) == true);
    assert ((b != a) == true);
    assert ((b != b) == false);

    print ("ok\n");
}


void
testBoolCompileTimeEvaluation ()
{
    print ("Testing operations on type bool, compile-time\n");

    const bool a = false;
    const bool b = true;

    assert (~a == true);
    assert (~b == false);

    assert (!a == true);
    assert (!b == false);

    assert ((a & a) == false);
    assert ((a & b) == false);
    assert ((b & b) == true);

    assert ((a && a) == false);
    assert ((a && b) == false);
    assert ((b && b) == true);

    assert ((a | a) == false);
    assert ((a | b) == true);
    assert ((b | b) == true);

    assert ((a || a) == false);
    assert ((a || b) == true);
    assert ((b || b) == true);

    assert ((a ^ a) == false);
    assert ((a ^ b) == true);
    assert ((b ^ b) == false);

    assert ((a == a) == true);
    assert ((a == b) == false);
    assert ((b == a) == false);
    assert ((b == b) == true);

    assert ((a > a) == false);
    assert ((a > b) == false);
    assert ((b > a) == true);
    assert ((b > b) == false);

    assert ((a >= a) == true);
    assert ((a >= b) == false);
    assert ((b >= a) == true);
    assert ((b >= b) == true);

    assert ((a < a) == false);
    assert ((a < b) == true);
    assert ((b < a) == false);
    assert ((b < b) == false);

    assert ((a <= a) == true);
    assert ((a <= b) == true);
    assert ((b <= a) == false);
    assert ((b <= b) == true);

    assert ((a != a) == false);
    assert ((a != b) == true);
    assert ((b != a) == true);
    assert ((b != b) == false);

    print ("ok\n");
}


void
testIntRunTimeEvaluation ()
{
    print ("Testing operations on type int, run-time\n");

    int a = 0;
    int b = 3;
    int c = -5;
    int d = 2;
    int e = 4;

    assert (~a == -1);
    assert (~b == -4);

    assert (-a == 0);
    assert (-b == -3);
    assert (-c == 5);

    assert (!a == true);
    assert (!b == false);
    assert (!c == false);

    assert ((a & a) == 0);
    assert ((a & b) == 0);
    assert ((b & b) == 3);
    assert ((b & d) == 2);

    assert ((a && a) == false);
    assert ((a && b) == false);
    assert ((b && b) == true);

    assert ((a | a) == 0);
    assert ((a | b) == 3);
    assert ((b | b) == 3);
    assert ((b | d) == 3);

    assert ((a || a) == false);
    assert ((a || b) == true);
    assert ((b || b) == true);

    assert ((a ^ a) == 0);
    assert ((a ^ b) == 3);
    assert ((b ^ b) == 0);
    assert ((b ^ d) == 1);

    assert ((a / d) == 0);
    assert ((b / d) == 1);
    assert ((e / d) == 2);

    assert ((a == a) == true);
    assert ((a == b) == false);
    assert ((b == a) == false);
    assert ((b == b) == true);

    assert ((a > a) == false);
    assert ((a > b) == false);
    assert ((b > a) == true);
    assert ((b > b) == false);

    assert ((a >= a) == true);
    assert ((a >= b) == false);
    assert ((b >= a) == true);
    assert ((b >= b) == true);

    assert ((a << a) == 0);
    assert ((a << b) == 0);
    assert ((b << a) == 3);
    assert ((e << d) == 16);

    assert ((a < a) == false);
    assert ((a < b) == true);
    assert ((b < a) == false);
    assert ((b < b) == false);

    assert ((a <= a) == true);
    assert ((a <= b) == true);
    assert ((b <= a) == false);
    assert ((b <= b) == true);

    assert ((a - a) == 0);
    assert ((a - b) == -3);
    assert ((b - a) == 3);
    assert ((c - b) == -8);

    assert ((a % d) == 0);
    assert ((b % d) == 1);
    assert ((e % b) == 1);
    assert ((e % d) == 0);

    assert ((a >> a) == 0);
    assert ((a >> b) == 0);
    assert ((b >> a) == 3);
    assert ((e >> d) == 1);

    assert ((a != a) == false);
    assert ((a != b) == true);
    assert ((b != a) == true);
    assert ((b != b) == false);

    assert ((a + a) == 0);
    assert ((a + b) == 3);
    assert ((b + a) == 3);
    assert ((c + b) == -2);

    assert ((a * a) == 0);
    assert ((a * b) == 0);
    assert ((b * c) == -15);
    assert ((d * e) == 8);

    print ("ok\n");
}


void
testIntCompileTimeEvaluation ()
{
    print ("Testing operations on type int, compile-time\n");

    const int a = 0;
    const int b = 3;
    const int c = -5;
    const int d = 2;
    const int e = 4;

    assert (~a == -1);
    assert (~b == -4);

    assert (-a == 0);
    assert (-b == -3);
    assert (-c == 5);

    assert (!a == true);
    assert (!b == false);
    assert (!c == false);

    assert ((a & a) == 0);
    assert ((a & b) == 0);
    assert ((b & b) == 3);
    assert ((b & d) == 2);

    assert ((a && a) == false);
    assert ((a && b) == false);
    assert ((b && b) == true);

    assert ((a | a) == 0);
    assert ((a | b) == 3);
    assert ((b | b) == 3);
    assert ((b | d) == 3);

    assert ((a || a) == false);
    assert ((a || b) == true);
    assert ((b || b) == true);

    assert ((a ^ a) == 0);
    assert ((a ^ b) == 3);
    assert ((b ^ b) == 0);
    assert ((b ^ d) == 1);

    assert ((a / d) == 0);
    assert ((b / d) == 1);
    assert ((e / d) == 2);

    assert ((a == a) == true);
    assert ((a == b) == false);
    assert ((b == a) == false);
    assert ((b == b) == true);

    assert ((a > a) == false);
    assert ((a > b) == false);
    assert ((b > a) == true);
    assert ((b > b) == false);

    assert ((a >= a) == true);
    assert ((a >= b) == false);
    assert ((b >= a) == true);
    assert ((b >= b) == true);

    assert ((a << a) == 0);
    assert ((a << b) == 0);
    assert ((b << a) == 3);
    assert ((e << d) == 16);

    assert ((a < a) == false);
    assert ((a < b) == true);
    assert ((b < a) == false);
    assert ((b < b) == false);

    assert ((a <= a) == true);
    assert ((a <= b) == true);
    assert ((b <= a) == false);
    assert ((b <= b) == true);

    assert ((a - a) == 0);
    assert ((a - b) == -3);
    assert ((b - a) == 3);
    assert ((c - b) == -8);

    assert ((a % d) == 0);
    assert ((b % d) == 1);
    assert ((e % b) == 1);
    assert ((e % d) == 0);

    assert ((a >> a) == 0);
    assert ((a >> b) == 0);
    assert ((b >> a) == 3);
    assert ((e >> d) == 1);

    assert ((a != a) == false);
    assert ((a != b) == true);
    assert ((b != a) == true);
    assert ((b != b) == false);

    assert ((a + a) == 0);
    assert ((a + b) == 3);
    assert ((b + a) == 3);
    assert ((c + b) == -2);

    assert ((a * a) == 0);
    assert ((a * b) == 0);
    assert ((b * c) == -15);
    assert ((d * e) == 8);

    print ("ok\n");
}


void
testUnsignedIntRunTimeEvaluation ()
{
    print ("Testing operations on type unsigned int, run-time\n");

    unsigned int a = 0;
    unsigned int b = 3;
    unsigned int c = 5;
    unsigned d = 2;
    unsigned e = 4;

    assert (~a == ~0);
    assert (~b == ~0 - 3);

    assert (-a == 0);
    assert (-b == -3);
    assert (-c == -5);

    assert (!a == true);
    assert (!b == false);
    assert (!c == false);

    assert ((a & a) == 0);
    assert ((a & b) == 0);
    assert ((b & b) == 3);
    assert ((b & d) == 2);

    assert ((a && a) == false);
    assert ((a && b) == false);
    assert ((b && b) == true);

    assert ((a | a) == 0);
    assert ((a | b) == 3);
    assert ((b | b) == 3);
    assert ((b | d) == 3);

    assert ((a || a) == false);
    assert ((a || b) == true);
    assert ((b || b) == true);

    assert ((a ^ a) == 0);
    assert ((a ^ b) == 3);
    assert ((b ^ b) == 0);
    assert ((b ^ d) == 1);

    assert ((a / d) == 0);
    assert ((b / d) == 1);
    assert ((e / d) == 2);

    assert ((a == a) == true);
    assert ((a == b) == false);
    assert ((b == a) == false);
    assert ((b == b) == true);

    assert ((a > a) == false);
    assert ((a > b) == false);
    assert ((b > a) == true);
    assert ((b > b) == false);

    assert ((a >= a) == true);
    assert ((a >= b) == false);
    assert ((b >= a) == true);
    assert ((b >= b) == true);

    assert ((a << a) == 0);
    assert ((a << b) == 0);
    assert ((b << a) == 3);
    assert ((e << d) == 16);

    assert ((a < a) == false);
    assert ((a < b) == true);
    assert ((b < a) == false);
    assert ((b < b) == false);

    assert ((a <= a) == true);
    assert ((a <= b) == true);
    assert ((b <= a) == false);
    assert ((b <= b) == true);

    assert ((a - a) == 0);
    assert ((a - b) == -3);
    assert ((b - a) == 3);
    assert ((c - b) == 2);

    assert ((a % d) == 0);
    assert ((b % d) == 1);
    assert ((e % b) == 1);
    assert ((e % d) == 0);

    assert ((a >> a) == 0);
    assert ((a >> b) == 0);
    assert ((b >> a) == 3);
    assert ((e >> d) == 1);

    assert ((a != a) == false);
    assert ((a != b) == true);
    assert ((b != a) == true);
    assert ((b != b) == false);

    assert ((a + a) == 0);
    assert ((a + b) == 3);
    assert ((b + a) == 3);
    assert ((c + b) == 8);

    assert ((a * a) == 0);
    assert ((a * b) == 0);
    assert ((b * c) == 15);
    assert ((d * e) == 8);

    print ("ok\n");
}


void
testUnsignedIntCompileTimeEvaluation ()
{
    print ("Testing operations on type unsigned int, compile-time\n");

    const unsigned int a = 0;
    const unsigned int b = 3;
    const unsigned int c = 5;
    const unsigned d = 2;
    const unsigned e = 4;

    assert (~a == ~0);
    assert (~b == ~0 - 3);

    assert (-a == 0);
    assert (-b == -3);
    assert (-c == -5);

    assert (!a == true);
    assert (!b == false);
    assert (!c == false);

    assert ((a & a) == 0);
    assert ((a & b) == 0);
    assert ((b & b) == 3);
    assert ((b & d) == 2);

    assert ((a && a) == false);
    assert ((a && b) == false);
    assert ((b && b) == true);

    assert ((a | a) == 0);
    assert ((a | b) == 3);
    assert ((b | b) == 3);
    assert ((b | d) == 3);

    assert ((a || a) == false);
    assert ((a || b) == true);
    assert ((b || b) == true);

    assert ((a ^ a) == 0);
    assert ((a ^ b) == 3);
    assert ((b ^ b) == 0);
    assert ((b ^ d) == 1);

    assert ((a / d) == 0);
    assert ((b / d) == 1);
    assert ((e / d) == 2);

    assert ((a == a) == true);
    assert ((a == b) == false);
    assert ((b == a) == false);
    assert ((b == b) == true);

    assert ((a > a) == false);
    assert ((a > b) == false);
    assert ((b > a) == true);
    assert ((b > b) == false);

    assert ((a >= a) == true);
    assert ((a >= b) == false);
    assert ((b >= a) == true);
    assert ((b >= b) == true);

    assert ((a << a) == 0);
    assert ((a << b) == 0);
    assert ((b << a) == 3);
    assert ((e << d) == 16);

    assert ((a < a) == false);
    assert ((a < b) == true);
    assert ((b < a) == false);
    assert ((b < b) == false);

    assert ((a <= a) == true);
    assert ((a <= b) == true);
    assert ((b <= a) == false);
    assert ((b <= b) == true);

    assert ((a - a) == 0);
    assert ((a - b) == -3);
    assert ((b - a) == 3);
    assert ((c - b) == 2);

    assert ((a % d) == 0);
    assert ((b % d) == 1);
    assert ((e % b) == 1);
    assert ((e % d) == 0);

    assert ((a >> a) == 0);
    assert ((a >> b) == 0);
    assert ((b >> a) == 3);
    assert ((e >> d) == 1);

    assert ((a != a) == false);
    assert ((a != b) == true);
    assert ((b != a) == true);
    assert ((b != b) == false);

    assert ((a + a) == 0);
    assert ((a + b) == 3);
    assert ((b + a) == 3);
    assert ((c + b) == 8);

    assert ((a * a) == 0);
    assert ((a * b) == 0);
    assert ((b * c) == 15);
    assert ((d * e) == 8);

    print ("ok\n");
}


void
testHalfRunTimeEvaluation ()
{
    print ("Testing operations on type half, run-time\n");

    half a = 0;
    half b = 3.5;
    half c = -5;
    half d = 2;
    half e = 4;

    assert (-a == 0);
    assert (-b == -3.5);
    assert (-c == 5);

    assert (!a == true);
    assert (!b == false);
    assert (!c == false);

    assert ((a && a) == false);
    assert ((a && b) == false);
    assert ((b && b) == true);

    assert ((a || a) == false);
    assert ((a || b) == true);
    assert ((b || b) == true);

    assert ((a / d) == 0);
    assert ((b / d) == 1.75);
    assert ((e / d) == 2);

    assert ((a == a) == true);
    assert ((a == b) == false);
    assert ((b == a) == false);
    assert ((b == b) == true);

    assert ((a > a) == false);
    assert ((a > b) == false);
    assert ((b > a) == true);
    assert ((b > b) == false);

    assert ((a >= a) == true);
    assert ((a >= b) == false);
    assert ((b >= a) == true);
    assert ((b >= b) == true);

    assert ((a < a) == false);
    assert ((a < b) == true);
    assert ((b < a) == false);
    assert ((b < b) == false);

    assert ((a <= a) == true);
    assert ((a <= b) == true);
    assert ((b <= a) == false);
    assert ((b <= b) == true);

    assert ((a - a) == 0);
    assert ((a - b) == -3.5);
    assert ((b - a) == 3.5);
    assert ((c - b) == -8.5);

    assert ((a != a) == false);
    assert ((a != b) == true);
    assert ((b != a) == true);
    assert ((b != b) == false);

    assert ((a + a) == 0);
    assert ((a + b) == 3.5);
    assert ((b + a) == 3.5);
    assert ((c + b) == -1.5);

    assert ((a * a) == 0);
    assert ((a * b) == 0);
    assert ((b * c) == -17.5);
    assert ((d * e) == 8);

    print ("ok\n");
}


void
testHalfCompileTimeEvaluation ()
{
    print ("Testing operations on type half, compile-time\n");

    const half a = 0;
    const half b = 3.5;
    const half c = -5;
    const half d = 2;
    const half e = 4;

    assert (-a == 0);
    assert (-b == -3.5);
    assert (-c == 5);

    assert (!a == true);
    assert (!b == false);
    assert (!c == false);

    assert ((a && a) == false);
    assert ((a && b) == false);
    assert ((b && b) == true);

    assert ((a || a) == false);
    assert ((a || b) == true);
    assert ((b || b) == true);

    assert ((a / d) == 0);
    assert ((b / d) == 1.75);
    assert ((e / d) == 2);

    assert ((a == a) == true);
    assert ((a == b) == false);
    assert ((b == a) == false);
    assert ((b == b) == true);

    assert ((a > a) == false);
    assert ((a > b) == false);
    assert ((b > a) == true);
    assert ((b > b) == false);

    assert ((a >= a) == true);
    assert ((a >= b) == false);
    assert ((b >= a) == true);
    assert ((b >= b) == true);

    assert ((a < a) == false);
    assert ((a < b) == true);
    assert ((b < a) == false);
    assert ((b < b) == false);

    assert ((a <= a) == true);
    assert ((a <= b) == true);
    assert ((b <= a) == false);
    assert ((b <= b) == true);

    assert ((a - a) == 0);
    assert ((a - b) == -3.5);
    assert ((b - a) == 3.5);
    assert ((c - b) == -8.5);

    assert ((a != a) == false);
    assert ((a != b) == true);
    assert ((b != a) == true);
    assert ((b != b) == false);

    assert ((a + a) == 0);
    assert ((a + b) == 3.5);
    assert ((b + a) == 3.5);
    assert ((c + b) == -1.5);

    assert ((a * a) == 0);
    assert ((a * b) == 0);
    assert ((b * c) == -17.5);
    assert ((d * e) == 8);

    print ("ok\n");
}


void
testFloatRunTimeEvaluation ()
{
    print ("Testing operations on type float, run-time\n");

    float a = 0;
    float b = 3.5;
    float c = -5;
    float d = 2;
    float e = 4;

    assert (-a == 0);
    assert (-b == -3.5);
    assert (-c == 5);

    assert (!a == true);
    assert (!b == false);
    assert (!c == false);

    assert ((a && a) == false);
    assert ((a && b) == false);
    assert ((b && b) == true);

    assert ((a || a) == false);
    assert ((a || b) == true);
    assert ((b || b) == true);

    assert ((a / d) == 0);
    assert ((b / d) == 1.75);
    assert ((e / d) == 2);

    assert ((a == a) == true);
    assert ((a == b) == false);
    assert ((b == a) == false);
    assert ((b == b) == true);

    assert ((a > a) == false);
    assert ((a > b) == false);
    assert ((b > a) == true);
    assert ((b > b) == false);

    assert ((a >= a) == true);
    assert ((a >= b) == false);
    assert ((b >= a) == true);
    assert ((b >= b) == true);

    assert ((a < a) == false);
    assert ((a < b) == true);
    assert ((b < a) == false);
    assert ((b < b) == false);

    assert ((a <= a) == true);
    assert ((a <= b) == true);
    assert ((b <= a) == false);
    assert ((b <= b) == true);

    assert ((a - a) == 0);
    assert ((a - b) == -3.5);
    assert ((b - a) == 3.5);
    assert ((c - b) == -8.5);

    assert ((a != a) == false);
    assert ((a != b) == true);
    assert ((b != a) == true);
    assert ((b != b) == false);

    assert ((a + a) == 0);
    assert ((a + b) == 3.5);
    assert ((b + a) == 3.5);
    assert ((c + b) == -1.5);

    assert ((a * a) == 0);
    assert ((a * b) == 0);
    assert ((b * c) == -17.5);
    assert ((d * e) == 8);

    print ("ok\n");
}


void
testFloatCompileTimeEvaluation ()
{
    print ("Testing operations on type float, compile-time\n");

    const float a = 0;
    const float b = 3.5;
    const float c = -5;
    const float d = 2;
    const float e = 4;

    assert (-a == 0);
    assert (-b == -3.5);
    assert (-c == 5);

    assert (!a == true);
    assert (!b == false);
    assert (!c == false);

    assert ((a && a) == false);
    assert ((a && b) == false);
    assert ((b && b) == true);

    assert ((a || a) == false);
    assert ((a || b) == true);
    assert ((b || b) == true);

    assert ((a / d) == 0);
    assert ((b / d) == 1.75);
    assert ((e / d) == 2);

    assert ((a == a) == true);
    assert ((a == b) == false);
    assert ((b == a) == false);
    assert ((b == b) == true);

    assert ((a > a) == false);
    assert ((a > b) == false);
    assert ((b > a) == true);
    assert ((b > b) == false);

    assert ((a >= a) == true);
    assert ((a >= b) == false);
    assert ((b >= a) == true);
    assert ((b >= b) == true);

    assert ((a < a) == false);
    assert ((a < b) == true);
    assert ((b < a) == false);
    assert ((b < b) == false);

    assert ((a <= a) == true);
    assert ((a <= b) == true);
    assert ((b <= a) == false);
    assert ((b <= b) == true);

    assert ((a - a) == 0);
    assert ((a - b) == -3.5);
    assert ((b - a) == 3.5);
    assert ((c - b) == -8.5);

    assert ((a != a) == false);
    assert ((a != b) == true);
    assert ((b != a) == true);
    assert ((b != b) == false);

    assert ((a + a) == 0);
    assert ((a + b) == 3.5);
    assert ((b + a) == 3.5);
    assert ((c + b) == -1.5);

    assert ((a * a) == 0);
    assert ((a * b) == 0);
    assert ((b * c) == -17.5);
    assert ((d * e) == 8);

    print ("ok\n");
}


int
testTypes ()
{
    testBoolRunTimeEvaluation();
    testBoolCompileTimeEvaluation();
    testIntRunTimeEvaluation();
    testIntCompileTimeEvaluation();
    testUnsignedIntRunTimeEvaluation();
    testUnsignedIntCompileTimeEvaluation();
    testHalfRunTimeEvaluation();
    testHalfCompileTimeEvaluation();
    testFloatRunTimeEvaluation();
    testFloatCompileTimeEvaluation();
    return 0;
}

const int testTypesDummy = testTypes();
