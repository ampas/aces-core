namespace cppCall
{

int
funcRETi()
{
    return 5;
}


float
funcINifbRETf(int i, float f, bool b)
{
    return f;
}

float
funcINifbRETfD(int i, bool b, float f = 2.0)
{
    return f;
}

varying float
funcINifbRETfD2(int i, bool b, varying float f = 2.0)
{
    return f + .1;
}

void
funcWVaryiable1(int ai[])
{
    int i = ai[0];
}

void
funcWVaryiable2(int ai[1], output int oai[1][2][])
{
    oai[0][0][0] = 1;
}



} // namespace cppFunc
