#include<iostream>
#include<math.h>
#include<fstream>
#include<cstdlib>
#include<tgmath.h>
using namespace std;

fstream input;
fstream output;

const int samples = 90000; //N

//function that counts the sum from the Allan deviation formula
long double Allan(long long int* arr, int n, const int interval)
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
    long long int* container = new long long int[samples];
    input.open( "putty_example_1.log", ios::in);
    output.open("output.txt", ios::out);

    long long int x = 0;
    bool check = false;

    for (int i = 0; i < samples; i++)
    {
        //cout << container[i] << endl;
        input >> container[i];
        container[i] -= 50000; //removal of constant component

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

    const int interval = 10000000; //t0
    int onePercent;
    int detect = 0;
    int loadingStatus = 0;
    long double allanTemp;

    for (int n = 3; n < samples / 4; n++)
    {
        allanTemp = Allan(container, n, interval);
        onePercent = ( ( samples / 4 ) - 1 )/ 100;

        if(detect == onePercent)
        {
          cout << loadingStatus << "%";
          if( loadingStatus % 10 == 0)
          {
            cout << "\t ...random checks: " << allanTemp << endl;
          }
          else cout << endl;
          detect = 3;
          loadingStatus ++;
        }
        else detect++;

        output << n << "\t" << allanTemp << endl;                                               //save the outputs of the calculations for n to a file...
    }

    input.close();
    output.close();

    cout << endl << "...opus magnum, its done.";
    return 0;
}
