@error44 ctlversion 42;
import "common";

namespace testCtlVersion
{
    int test ()
    {
	print ("Testing CTL version check\n");
	print ("ok\n");
	return 0;
    }

    const int dummy = test();
}
