////////////////////////////////////
//..........olg rog v0.8..........//
////////////////////////////////////

#include<iostream>
#include<fstream>
#include<math.h>

using namespace std;

fstream input;
fstream output;

void TerminalClear(int a) // czysczenie terminalu
{
    switch (a)
    {
    case 1: system("cls"); // Windows
        break;
    case 2: system("clear"); // GNU/LINUX
        break;
    default: cout << endl << "==================================================" << endl << endl; //unknown
        break;
    }
}

// funkcja obliczajaca dewiacje Allana
long double Allan(long long int* arr, int n, const int interval, int samples)
{
    long double sum = 0;
    for (int i = 0; i < samples - 2 * n; i++)
    {   // postanowilismy rozpisac wzzor tak aby kazda czesc sumy byla od razu dzielona, aby zmniejszyc ryzyko przpelnienia
        long double auxSum = 0; // tymcasowa zmienna przechowuajca wynik
        auxSum += arr[i + 2 * n] - 2 * arr[i + n] + arr[i]; // aktualnie obliczana czesc sumy
        auxSum *= auxSum;
        auxSum /= 2 * n * n;
        auxSum /= interval;
        auxSum /= interval;
        auxSum /= (samples - 2 * n);
        sum += auxSum;
    }
    return sqrt(sum); // zwracanie calej sumy przez pierwiastek
}

int main()
{
    int system = 0;
    char choose = 'x';
    int samples = 90000; // liczba probek
    int constComp = 50000; // składowa stała
    int interval = 10000000; // t0
    int measRangVal = 100000; // wartosc zakresu pomiarowego

    cout << endl;
    cout << "     (1) Windows" << endl;
    cout << "     (2) GNU/Linux" << endl;
    cout << "     Choose operating system: ";
    cin >> system;

    while (choose != 's' && choose != 'S')
    {
        TerminalClear(system);
        cout << endl;

//######################### MENU #########################

        cout << " (0) Operating system ............ ";
        switch (system)
        {
        case 1: cout << "Windows" << endl;
            break;
        case 2: cout << "GNU/Linux" << endl;
            break;
        default: cout << "unknown" << endl;
            break;
        }
        cout << "     Input file path ............. " << "./putty_example_1.log" << endl;
        cout << "     Output file path ............ " << "./output.txt" << endl;
        cout << " (3) Number of samples (N) ....... " << samples << endl;
        cout << " (4) Constant component .......... " << constComp << endl;
        cout << "     Type of wave ................ " << "Sawtooth wave" << endl;
        cout << " (6) Interval (t0) ............... " << interval << endl;
        cout << " (7) Measuring range value ....... " << measRangVal << endl;

        cout << endl;
        cout << "     Choose number to edit or (S)tart (E)xit ";

        cin >> choose;

        if (choose == 'e' || choose == 'E') exit(0);

        if (choose != 's' && choose != 'S')
        {
            TerminalClear(0);
            switch (choose)
            {
            case '0':
                cout << "     (1) Windows" << endl;
                cout << "     (2) GNU/Linux" << endl;
                cout << "     Choose operating system: ";
                cin >> system;
                break;
            case '3':
                cout << "Number of samples: ";
                cin >> samples;
                break;
            case '4':
                cout << "Constant component: ";
                cin >> constComp;
                break;
            case '6':
                cout << "Interval: ";
                cin >> interval;
                break;
            case '7':
                cout << "Measuring range value: ";
                cin >> measRangVal;
                break;
            default: break;
            }
        }
    }

    TerminalClear(system);

    long long int* container = new long long int[samples]; // tworzenie tabeli do przechowywania zmiennych z pliku
    long long int x = 0;
    bool check = false;

    input.open("putty_example_1.log", ios::in);
    output.open("output.txt", ios::out);

    for (int i = 0; i < samples; i++)
    {
        input >> container[i];
        container[i] -= constComp; // usuniecie skladowej zmiennej

        if (container[i] < 0) // wykrywanie przeskoku
        {
            if (!check)
            {
                check = true;
                x++; // zliczanie kolejnych przeskokow
            }
        }
        else if (check)
        {
            check = false;
        }

        container[i] += measRangVal * x; // dodanie wartości zakresu pomiaroweg
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
        onePercent = ((samples / 4) - 1) / 100;

        if (detect == onePercent)
        {
            cout << "     " << loadingStatus << "%";
            if (loadingStatus % 10 == 0)
            {
                cout << "     ...random checks: " << allanTemp << endl;
            }
            else cout << endl;
            detect = 3;
            loadingStatus++;
        }
        else detect++;

        output << n << "\t" << allanTemp << endl; // zapisanie wynikow do pliku
    }

    input.close();
    output.close();

    cout << endl << "...opus magnum, its done.";
    return 0;
}
