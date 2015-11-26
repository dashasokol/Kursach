#include <iostream>
#include <kursbdclass.h>

using namespace std;

int abs(int f);
int abs(float f);

int abs(int f)
{
    cout << f << endl;
    return 0;
}

int abs(float f)
{
    cout << f << endl;
    return 0;
}

int main()
{
    float sd = 3;

    if (abs(sd))
        cout << '\n';

    return 0;
}

