void
function1
    (output uniform float a2,		// to outHeader, used in function2
     output uniform float b2,		// used in function2
     output uniform float c2,		// used in function2
     output varying half d2,		// to outFb, used in function2
     output varying half e2,		// used in function2
     output varying half k2Out,		// used in function2
     output varying half l2Out,		// to outFb
     output uniform float m2Out,	// used in function2
     output uniform float n2Out,	// to outHeader
     output uniform half x,		// discarded
     output varying half y,		// discarded
     input uniform float a1,		// from inHeader
     input uniform float b1 = 0,	// from envHeader
     input uniform float c1 = 23.5,	// use default value
     input varying half d1 = 0,		// from inFb
     input varying half e1 = 12)	// use default value
{
    a2 = a1;
    b2 = b1;
    c2 = c1;
    d2 = d1;
    e2 = e1;
    k2Out = 123.0;
    l2Out = 234.0;
    m2Out = 345.0;
    n2Out = 456.0;
    x = a1;
    y = d1;
}
