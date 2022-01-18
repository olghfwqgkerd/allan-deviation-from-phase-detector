#include<iostream>
#include<math.h>
#include<fstream>
#include<cstdlib>
#include<tgmath.h>
using namespace std;

fstream data1;
fstream data2;
fstream result1;
fstream result2;

const int samples = 90000; //N

//function that counts the sum from the Allan deviation formula
long double allan(long long int* arr, int n, const int interval)
{
    long double sum = 0;
    for (int i = 0; i < samples - 2 * n; i++)
    {
        long double auxSum = 0;
        auxSum += arr[i + 2 * n] - 2 * arr[i + n] + arr[i];
        auxSum *= auxSum;
        auxSum /= 2 * n * n;
        auxSum /= interval;
        auxSum /= interval;
        auxSum /= (samples - 2 * n);
        sum += auxSum;
    }
    return sqrt(sum);
}

int main()
{
    long long int* container1 = new long long int[samples];
    long long int* container2 = new long long int[samples];
    data1.open("putty1.log", ios::in);
    data2.open("putty2.log", ios::in);
    result1.open("output1.txt", ios::out);
    result2.open("output2.log", ios::out);

    long long int x1 = 0, x2 = 0;
    bool check1 = false, check2 = false;

    for (int i = 0; i < samples; i++)
    {
        data1 >> container1[i];
        data2 >> container2[i];

        container1[i] -= 50000; //removal of constant component
        container2[i] -= 50000;

        if (container1[i] < 0)
        {
            if (!check1)
            {
                check1 = true;
                x1++;
            }
        }
        else if (check1)
        {
            check1 = false;
        }

        if (container2[i] < 0)
        {
            if (!check2)
            {
                check2 = true;
                x2++;
            }
        }
        else if (check2)
        {
            check2 = false;
            x2++;
        }

        container1[i] += 100000 * x1;
        container2[i] += 100000 * x2;

        container1[i] *= 10; //[ns]
        container2[i] *= 10; //[ns]

    }

    const int interval = 10000000; //t0
    /*
    //calculation starts with n=3, ends with n=N/4
    for (int n = 3; n < samples / 4; n++) {
        long double  ADEV1 = allan(container1, n, 10000000);                    //calculate the sum for the first set of data
        long double  ADEV2 = allan(container2, n, 10000000);

        result1 << ADEV1 << endl;                                               //save the results of the calculations for n to a file...
        result2 << ADEV2 << endl;
    }
    */
    for (int i = 0; i < samples; i++)
    {
        result1 << container1[i] << endl;                                               //save the results of the calculations for n to a file...
        result2 << container1[i] << endl;
    }
    data1.close();
    data2.close();
    result1.close();
    result2.close();
    cout << "...opus magnum, its done.";
    return 0;
}
