
// <ACEStransformID>ACESlib.Adjust_Exposure.a1.0.0</ACEStransformID>

// 
// Adjust Exposure
//
// Adjusts the linear exposure value by scaling the input by the values in 
// expFactor. expFactor consists of 3 values that adjust the R,G,B channels 
// accordingly expFactor is specified at runtime with ctlrender using the 
// -param3 flag.
//
// Example:
//    To half the exposure (i.e. a 1 stop decrease):
// ctlrender -ctl adjust_exp.ctl in.exr out.exr -param3 expFactor 0.5 0.5 0.5
// 



void main
(   
  input varying float rIn,
  input varying float gIn,
  input varying float bIn,
  input varying float aIn,
  output varying float rOut,
  output varying float gOut,
  output varying float bOut,
  output varying float aOut,
  input float expFactor[3] = {1.0, 1.0, 1.0}
)
{
	rOut = rIn * expFactor[0];
	gOut = gIn * expFactor[1];
	bOut = bIn * expFactor[2];
	aOut = aIn;
}