void gain3
    (output varying half ROut,
     output varying half GOut,
     output varying half BOut,
     input varying half R,
     input varying half G,
     input varying half B,
     input uniform half gain3[3])
{
	ROut=R*gain3[0];
	GOut=G*gain3[1];
	BOut=B*gain3[2];
}
