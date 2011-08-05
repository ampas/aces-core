void
function2
    (output uniform float a3,		// to outHeader
     output uniform float b3,		// to outHeader
     output uniform float c3,		// to outHeader
     output varying half d3,		// to outFb
     output varying half e3,		// to outFb
     output uniform float f3,		// to outHeader
     output uniform float g3,		// to outHeader
     output varying half h3,		// to outFb
     output uniform float i3,		// to outHeader
     output varying half j3,		// to outFb
     input uniform float a2,		// from function1
     input uniform float b2,		// from function1
     input uniform float c2,		// from function1
     input varying half d2,		// from function1
     input varying half e2,		// from function1
     input uniform float f2,		// from inHeader
     input uniform float g2,		// from envHeader
     input varying half h2,		// from inFb
     input uniform float i2 = 1.0,	// use default value
     input varying half j2 = 2.0,	// use default value
     input varying half k2 = 0.0,	// from function1
     input uniform float m2 = 0.0)	// from function1
{
    a3 = a2;
    b3 = b2;
    c3 = c2;
    d3 = d2;
    e3 = e2;
    f3 = f2;
    g3 = g2;
    h3 = h2;
    i3 = i2;
    j3 = j2;

    assert (k2 == 123.0);
    assert (m2 == 345.0);
}
