
// <ACEStransformID>ACESlib.Utilities.a1.0.0</ACEStransformID>

//
// Generic functions that may be useful for writing CTL programs
//

float min( float a, float b)
{
  if (a < b)
    return a;
  else
    return b;
}

float max( float a, float b)
{
  if (a > b)
    return a;
  else
    return b;
}

float min_f3( float a[3])
{
  return min( a[0], min( a[1], a[2]));
}

float max_f3( float a[3])
{
  return max( a[0], max( a[1], a[2]));
}

float clip( float v)
{
  return min(v, 1.0);
}

float[3] clip_f3( float in[3])
{
  float out[3];
  out[0] = clip( in[0]);
  out[1] = clip( in[1]);
  out[2] = clip( in[2]);

  return out;
}

float clamp( float in, float clampMin, float clampMax)
{
  // Note: Numeric constants can be used in place of a min or max value (i.e. 
  // use HALF_NEG_INF in place of clampMin or HALF_POS_INF in place of clampMax)
  
  return max( clampMin, min(in, clampMax));
}

float[3] clamp_f3( float in[3], float clampMin, float clampMax)
{
  // Note: Numeric constants can be used in place of a min or max value (i.e. 
  // use HALF_NEG_INF in place of clampMin or HALF_POS_INF in place of clampMax)

  float out[3];
  out[0] = clamp( in[0], clampMin, clampMax);
  out[1] = clamp( in[1], clampMin, clampMax);
  out[2] = clamp( in[2], clampMin, clampMax);
      
  return out;
}

float[3] add_f_f3( float a, float b[3])
{
  float out[3];
  out[0] = a + b[0];
  out[1] = a + b[1];
  out[2] = a + b[2];
  return out;
}

float[3] pow_f3( float a[3], float b)
{
  float out[3];
  out[0] = pow(a[0], b);
  out[1] = pow(a[1], b);
  out[2] = pow(a[2], b);
  return out;
}

float[3] pow10_f3( float a[3])
{
  float out[3];
  out[0] = pow10(a[0]);
  out[1] = pow10(a[1]);
  out[2] = pow10(a[2]);
  return out;
}

float[3] log10_f3( float a[3])
{
  float out[3];
  out[0] = log10(a[0]);
  out[1] = log10(a[1]);
  out[2] = log10(a[2]);
  return out;
}

float round(float x)
{
  float x1;

  if (x < 0.0)
    x1 = floor( x - 0.5);
  else
    x1 = floor( x + 0.5);

  return x1;
}

float log2(float x)
{
  return log(x) / log(2.);
}

int sign( float x)
{
	// Signum function:
  //  sign(X) returns 1 if the element is greater than zero, 0 if it equals zero 
  //  and -1 if it is less than zero

	int y;
	if (x < 0) { 
		y = -1;
	} else if (x > 0) {
		y = 1;
	} else {
		y = 0;
	}

	return y;	
}

void print_f3( float m[ 3])
{
   print( m[ 0], ",\t", m[ 1], ",\t", m[ 2], "\n");
}

void print_f33( float m[ 3][ 3])
{
   print( "{ {", m[ 0][ 0], ",\t", m[ 0][ 1], ",\t", m[ 0][ 2], "},\n");
   print( "  {", m[ 1][ 0], ",\t", m[ 1][ 1], ",\t", m[ 1][ 2], "},\n");
   print( "  {", m[ 2][ 0], ",\t", m[ 2][ 1], ",\t", m[ 2][ 2], "} };\n");
}

void print_f44( float m[ 4][ 4])
{
   print( "{ { ", m[ 0][ 0], ",\t", m[ 0][ 1], ",\t", m[ 0][ 2], ",\t", m[ 0][ 3], "},\n");
   print( "  { ", m[ 1][ 0], ",\t", m[ 1][ 1], ",\t", m[ 1][ 2], ",\t", m[ 1][ 3], "},\n");
   print( "  { ", m[ 2][ 0], ",\t", m[ 2][ 1], ",\t", m[ 2][ 2], ",\t", m[ 2][ 3], "},\n");
   print( "  { ", m[ 3][ 0], ",\t", m[ 3][ 1], ",\t", m[ 3][ 2], ",\t", m[ 3][ 3], "} };\n");
}
