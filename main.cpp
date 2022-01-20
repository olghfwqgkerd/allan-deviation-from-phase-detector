#include<iostream>
#include<math.h>
#include<fstream>
#include<cstdlib>
#include<tgmath.h>

using namespace std;

fstream input;
fstream output;

void ConsoleClear(int a)
{
  switch(a)
  {
    case 1: system("cls"); //Windows
      break;
    case 2: system("clear"); //GNU/LINUX
      break;
    default: cout << endl << "==================================================" << endl << endl;
      break;
  }
}

//function that counts the sum from the Allan deviation formula
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
    int system = 0;
    char choose = 'a';
    int samples = 90000; //N
    int constComp = 50000; //Constant component
    int interval = 10000000; //t0

    cout << "     (1) Windows" << endl;
    cout << "     (2) GNU/Linux" << endl;
    cout << "     Choose operating system: ";
    cin >> system;

    while(choose != 's' && choose != 'S')
    {
      ConsoleClear(system);

      cout << " (0) Operating system ............ ";
      switch(system)
      {
        case 1: cout << "Windows" << endl;
          break;
        case 2: cout << "GNU/Linux" << endl;
          break;
        default: cout << "unknown" << endl;
          break;
      }
      cout << "     File path ................... " << "/putty_example_1.log" << endl;
      cout << " (2) Number of samples (N) ....... " << samples << endl;
      cout << " (3) Constant component .......... "<< constComp << endl;
      cout << "     Type of wave ................ " << "Sawtooth wave" << endl;
      cout << " (5) Interval (t0) ............... " << interval << endl;
      cout << endl;
      cout << "     Choose number to edit or (S)tart (E)xit ";

      cin >> choose;

      if(choose != 's' && choose != 'S')
      {
        ConsoleClear(9);
        switch(choose)
        {
          case '0':
          cout << "     (1) Windows" << endl;
          cout << "     (2) GNU/Linux" << endl;
          cout << "     Choose operating system: ";
          cin >> system;
            break;
          case '2':
              cout << "Number of samples: ";
              cin >> samples;
            break;
          case '3':
              cout << "Constant component: ";
              cin >> constComp;
            break;
          case '5':
              cout << "Interval: ";
              cin >> interval;
            break;
          default: break;
        }
      }
    }

    ConsoleClear(system);

    long long int* container = new long long int[samples];
    long long int x = 0;
    bool check = false;

    input.open( "putty_example_1.log", ios::in);
    output.open("output.txt", ios::out);

    for (int i = 0; i < samples; i++)
    {
        //cout << container[i] << endl;
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


    int onePercent;
    int detect = 0;
    int loadingStatus = 0;
    long double allanTemp;

    cout << "--------Start of the calculation--------" << endl << endl;

    for (int n = 3; n < samples / 4; n++)
    {
        allanTemp = Allan(container, n, interval, samples);
        onePercent = ( ( samples / 4 ) - 1 )/ 100;

        if(detect == onePercent)
        {
          cout << loadingStatus << " %";
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
