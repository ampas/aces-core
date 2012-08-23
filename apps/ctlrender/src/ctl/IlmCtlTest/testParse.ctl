namespace testParse
{

@error42 int x = 3;
@error42 float y[2] = {sqrt(2), sqrt(3)};
const int w = 4;
@error42 bool z;

void
variableNames()
{
    return;
           int test_name_with_underscores;
           int really_long_variable_name_really_long_variable_name_really_long_variable_name_really_long_variable_name_really_long_variable_name_;
@error4    int aa~;
@error4    int ab&;
@error4    int ac*;
@error4    int ad%;
@error4    int ae$;
@error4    int af`;
@error4    int ag/;
@error4    int ah';
@error4    int ';
@error4 @error28 int ah"";

           float f = .01;
           float f2 = 0000000.01; 
@error4    float f = .A1;
@error4    float f3 = .01.0; 


}


void
junk()
{
    return;
    some_undefined_junk
    @error29 ;

    int variableNames;
}





}  // namespace testParse
