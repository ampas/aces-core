// Check that variables in an imported file with the same namespace 
// declaration can be referred to with or without namespace
// specifiers.  (And that variables can not be redefined.)


ctlversion 1;

import "testName";

namespace testName
{


int
testName()
{
    assert (i == 20);
    assert (testName::i == 20);
    return 1;
}

const int dummyTestName2 = testName();

@error28 const int i = 0;  // one is already defined in testName.ctl


}  // namespace testName
