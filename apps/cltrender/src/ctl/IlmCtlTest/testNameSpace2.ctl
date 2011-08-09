//  Test variable scoping & namespaces from within a namespace 
//


import "testNameSpace2";  // test recursion checking

import "testName";
import "testNoName";

namespace testNameSpace2
{

int
returnInt2()
{
    return 3;
}    


int
test()
{
    assert(returnInt() == 1);
    assert(testName::returnInt() == 2);

    assert(::returnInt2() == 1);
    assert(returnInt2() == 3);
    assert(testName::returnInt2() == 2);
    

	
    return 1;
}






const int t = test();



} // namespace testNameSpace2
