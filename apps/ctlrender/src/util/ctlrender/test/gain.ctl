void main
    (output varying half ROut,
     output varying half GOut,
     output varying half BOut,
     input varying half R,
     input varying half G,
     input varying half B,
     input uniform float gain=1.5)
{
    ROut=R*gain;
	GOut=G*gain;
	BOut=B*gain;
}
