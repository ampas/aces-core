namespace MyLib {

const int i1 = 0;
const int i2 = 0;

void myFunc()
{
    const int i1 = 1;

    {
        int i = i1;           assert(i == 1);
        int j = MyLib::i1;    assert(j == 0);
        int k = i2;           assert(k == 0);
    }
}
} // namespace MyLib
