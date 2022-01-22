////////////////////////////////////////////////////////////////////////////////
//or.oyeaiu...22-01-2022......................................................//
////////////////////////////////////////////////////////////////////////////////
#include<iostream>
#include<fstream>
#include<math.h> // sqrt
#include<cstdlib> // exit

using namespace std;

void TerminalClear(int a)
{
	switch (a)
	{
    case 1:
		cout << endl;
		system("cls"); // Windows
        break;
    case 2:
		cout << endl;
		system("clear"); // Unix
        break;
    default: cout << endl << "===========================================================" << endl << endl; //unknown
        break;
    }
}


long double AllanDeviation(long long int* arr, unsigned int n, unsigned int interval, unsigned int samples)
{
    long double sum = 0;
    for (unsigned int i = 0; i < samples - 2 * n; i++)
    {
        long double tempSum = 0;
        tempSum += arr[i + 2 * n] - 2 * arr[i + n] + arr[i];
        tempSum *= tempSum;
        tempSum /= 2 * n * n;
        tempSum /= interval;
        tempSum /= interval;
        tempSum /= (samples - 2 * n);
        sum += tempSum;
    }
    return sqrt(sum);
}

int SystemSelection()
{
	int a = 3;
    cout << "(1) Windows" << endl;
    cout << "(2) GNU/Linux or MacOS" << endl;
	cout << "(3) Unknown" << endl;
    cout << "    Select operating system: ";
    cin >> a;
	return a;
}

int main()
{
    fstream input;
    fstream output;

    int system = 0;
    char choose = 'x';

    unsigned int samples = 90000; // number of samples, records in logs
    float offset = 50000; // offset
    unsigned int interval = 10000000; // t0
    unsigned int measRangVal = 100000; // measuring range value

	system = SystemSelection();

//##################### MENU SECTION #########################

    while (choose != 's' && choose != 'S')
    {
        TerminalClear(system);

		cout << "====================[PARAMETER SETTING]====================" << endl;
        cout << "(0) Operating system ............... ";
        switch (system)
        {
        case 1: cout << "Windows" << endl;
            break;
        case 2: cout << "GNU/Linux or MacOS" << endl;
            break;
        default: cout << "unknown" << endl;
            break;
        }
        cout << "    Input file path ................ " << "./input.log" << endl;
        cout << "    Output file path ............... " << "./output.txt" << endl;
		cout << "-----------------------------------------------------------" << endl;
		cout << "    Type of wave ................... " << "Sawtooth wave" << endl;
        cout << "(4) Number of samples [N] .......... " << samples << endl;
        cout << "(5) Offset ......................... " << offset << endl;
        cout << "(6) Interval [t0] .................. " << interval << endl;
        cout << "(7) Measuring range value .......... " << measRangVal << endl;
        cout << endl;
        cout << "    Choose number to edit, (S)tart or (E)xit ";

        cin >> choose;

        if (choose == 'e' || choose == 'E')
		{
			TerminalClear(system);
			cout << "Program ended...";
			exit(0);
		}

        if (choose != 's' && choose != 'S')
        {
            TerminalClear(0);
            switch (choose)
            {
            case '0':
				system = SystemSelection();
                break;
            case '4':
                cout << "Number of samples: ";
                cin >> samples;
                break;
            case '5':
                cout << "Offset: ";
                cin >> offset;
                break;
            case '6':
                cout << "Interval: ";
                cin >> interval;
                break;
            case '7':
                cout << "Measuring range value: ";
                cin >> measRangVal;
                break;
            default:
				break;
            }
        }
    }

    TerminalClear(system);

    long long int* container = new long long int[samples];
	long long int x = 0;
    bool check = false;

    input.open("putty.log", ios::in);
    output.open("output.txt", ios::out);

    for (unsigned int i = 0; i < samples; i++)
    {
        input >> container[i];
        container[i] -= offset;
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
        container[i] += measRangVal * x;
        container[i] *= 10;
    }

    int onePercent;
    int detect = 0;
    int loadingStatus = 0;
    long double allanTemp = 0;

    cout << "=================[Start of the calculation]================" << endl << endl;

    for (unsigned int n = 3; n < samples / 4; n++)
    {
        allanTemp = AllanDeviation(container, n, interval, samples);
        onePercent = ((samples / 4) - 1) / 100;

        if (detect == onePercent)
        {
            cout << loadingStatus << "%";
            if (loadingStatus % 10 == 0)
            {
                cout << "........random checks: " << allanTemp << endl;
            }
            else cout << endl;
            detect = 3;
            loadingStatus++;
        }
        else detect++;

        output << n << "\t" << allanTemp << endl;
    }

    input.close();
    output.close();

    cout << endl << "...opus magnum, its done. Program ended...";
    return 0;
}
