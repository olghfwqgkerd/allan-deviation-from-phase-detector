#include<iostream>
#include<math.h>
#include<fstream>
#include<cstdlib>
#include<tgmath.h>

using namespace std;

fstream input;
fstream output;

long double Allan(long long int* arr, int n, const int interval, int samples)
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
    int samples = 90000; //N
    int constComp = 50000; //Constant component
    int interval = 10000000; //t0

    long long int* container = new long long int[samples];
    long long int x = 0;
    bool check = false;

    input.open( "putty_example_1.log", ios::in);
    output.open("output.txt", ios::out);

    for (int i = 0; i < samples; i++)
    {
        input >> container[i];
        container[i] -= constComp; //removal of constant component

        if (container[i] < 0)
        {
            if (!check)
            {
                check = true;
                x++;
            }
        }
        else if (check)
        {
            check = false;
        }

        container[i] += 100000 * x;
        container[i] *= 10; //[ns]
    }

    cout << "Start..." << endl;

    for (int n = 3; n < samples / 4; n++)
    {
        if( n == samples / 16 ) cout << "25%" << endl;
        if( n == samples / 12 ) cout << "50%" << endl;
        if( n == samples / 8 ) cout << "75%" << endl;
        if( n == samples / 4 ) cout << "100%" << endl;

        output << n << "\t" << Allan(container, n, interval, samples) << endl;                                               //save the outputs of the calculations for n to a file...
    }

    input.close();
    output.close();

    cout << endl << "...opus magnum, its done.";
    return 0;
}
