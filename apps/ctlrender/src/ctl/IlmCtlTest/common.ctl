float
absolute (float a)
{
    if (a >= 0)
	return a;
    else
	return -a;
}


bool
equalWithAbsErr (float a, float b, float e)
{
    return absolute (a - b) <= e;
}


