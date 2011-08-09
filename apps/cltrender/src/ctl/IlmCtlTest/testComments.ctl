// Test // and /* ... */ comments

/*
float
*/
int
testComments ()
{
    /*
     * comment comment comment comment
     * comment comment comment comment
     * comment comment comment comment
     * comment comment comment comment
     */

    print ("Testing comments\n");

    //
    // comment comment comment comment
    // comment comment comment comment
    // comment comment comment comment
    // comment comment comment comment
    //

    // */ comment comment
    int a = 1; // comment // comment
    assert (a == 1); // comment /* */

    int /***/ b /*comment* /*/ = 2 + a; /*comment*/
    assert (b == 3); /* comment */ // comment

    int /* "*/ c = b + a;
    assert (c /* comment
    comment comment
    comment */ == /* comment
    comment
    comment */// 5
    /* // comment comment
    * /*/ 4);

    int /*"*/ d /*"*/ = c /* comment *// /* comment *///* 4;
    2;

    assert (d == 2);

    print ("ok\n");
    return 0;
}

const int testCommentsDummy = testComments();
