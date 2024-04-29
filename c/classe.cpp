#include <math.h>
#include <locale.h>
#include <iostream>

using namespace std;

struct polinomio
{
    float a, b, c;
    float delta, x1, x2;

    void recebe_coeficiente()
    {
        cout << "Coeficiente a: ";
        cin >> a;
        cout << "Coeficiente b: ";
        cin >> b;
        cout << "Coeficiente c: ";
        cin >> c;
    }

    void raizes()
    {
        delta = (b * b) - (4 * a * c);
        if (delta < 0)
        {
            cout << "Não existe raiz real" << endl;
        }
        else
        {
            x1 = (-b + sqrt(delta)) / (2 * a);
            x2 = (-b - sqrt(delta)) / (2 * a);
            cout << "x1: " << x1 << endl;
            cout << "x2: " << x2 << endl;
        }
    }
};

int main()
{
    setlocale(LC_ALL,"Portuguese");
    polinomio p;

    while (1)
    {
        p.recebe_coeficiente();
        p.raizes();
    }
    
}