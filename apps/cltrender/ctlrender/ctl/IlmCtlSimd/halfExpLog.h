//
// This is an automatically generated file.
// Do not edit.
//


extern const unsigned int log10Table[];
extern const unsigned int logTable[];
extern const unsigned short expTable[];


inline float
log10_h (half x)
{
    return *(float *)(&log10Table[x.bits()]);
}


inline float
log_h (half x)
{
    return *(float *)(&logTable[x.bits()]);
}


inline half
exp_h (float x)
{
    if (x >= -16.6355323f)
    {
        if (x <= 11.0898665f)
        {
            //
            // If we arrive here, then x is finite and between
            // log(HALF_MIN) and log(HALF_MAX); indexing into
            // expTable will not fail.
            //

            return *(half *)(&expTable[int (x * 4094.98169f + 68122.7031f)]);
        }
        else
        {
            return half::posInf();
        }
    }
    else if (x < -16.6355323f)
    {
        return 0;
    }
    else
    {
        //
        // If we arrive here, then x is a NaN.
        //

        return half::qNan();
    }
}


inline half
pow10_h (float x)
{
    return exp_h (x * 2.30258509f);
}


inline half
pow_h (half x, float y)
{
    return exp_h (y * log_h (x));
}

