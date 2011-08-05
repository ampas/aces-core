//  Test variable scoping & namespaces from global namespace
//

import "testName";
import "testNoName";




int
test()
{
    assert(returnInt() == 1);
    assert(testName::returnInt() == 2);

	
    return 1;
}






const int t = test();
