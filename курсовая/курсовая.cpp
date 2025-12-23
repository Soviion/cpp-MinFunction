#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <Windows.h>

using namespace std;

void printCenteredText(const string& text) {

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    int textWidth = static_cast<int>(text.length());
    int leftPadding = (consoleWidth - textWidth) / 2;

    for (int i = 0; i < leftPadding; i++) {
        cout << " ";
    }
   cout << text << endl;
}

double foo(double x) {
    return pow(x,2)-(4*x*sin(x))+cos(x);              
}

double binlineMethod(double a, double b, double eps, int& it) {
    double x = 0;
    double x1 = (a + b - eps) / 2.0;
    double x2 = (a + b + eps) / 2.0;
    double y1 = foo(x1);
    double y2 = foo(x2);
    it = 1;
    while (abs(b - a) >= (2 * eps)) {
        if (y1 < y2) {
            b = x2;
        }
        else {
            a = x1;
        }
        x1 = (a + b - eps) / 2.0;
        x2 = (a + b + eps) / 2.0;
        y1 = foo(x1);
        y2 = foo(x2);
        it++;
    }
    return (a + b) / 2.0;
}

pair<double, double> static CubicParabolaMethod(double a, double b, double eps) {
    pair<double, double> minimum;

    double x1 = a;
    double x2 = (a + b) / 2.0;
    double x3 = b;

    double f1 = foo(x1);
    double f2 = foo(x2);
    double f3 = foo(x3);

    double minimumX = NAN;
    double minimumY = INFINITY;

    while (abs(x3 - x1) > eps)
    {
        double a1 = (f2 - f1) / (x2 - x1);
        double a2 = ((f3 - f1) / (x3 - x1) - (f2 - f1) / (x2 - x1)) / (x3 - x2);

        minimumX = (x1 + x2 - a1 / a2) / 2;

        minimumY = foo(minimumX);

        if (minimumX < x2) {
            if (minimumY < f2) {
                x3 = x2;
                f3 = f2;
                x2 = minimumX;
                f2 = minimumY;
            }
            else {
                x1 = minimumX;
                f1 = minimumY;
            }
        }
        else {
            if (minimumY < f2) {
                x1 = x2;
                f1 = f2;
                x2 = minimumX;
                f2 = minimumY;
            }
            else {
                x3 = minimumX;
                f3 = minimumY;
            }
        }
    }

    minimum.first = minimumX;
    minimum.second = foo(minimumX);

    return minimum;
}

void line() {
    cout << "-----------------------------" << endl;
}

int main() {
    while (true) {
        setlocale(LC_ALL, "ru");
        string text = "- - - - - - - - - - - - - - - - - - - НАХОЖДЕНИЕ МИНИМУМА ФУНКЦИИ ОДНОЙ ПЕРЕМЕННОЙ - - - - - - - - - - - - - - - - - - - \n";
        printCenteredText(text);

        int choice, it1;
        double a = 0, b = 0, eps = 0, x_min, y_min;
       
        while (true) {
            cout << "Выбери ввод интервала и точности: 1 - пользовательский(с клавиатуры)\n";
            cout << "                                  2 - записанный [-4,9], eps = 0.001\n";
            cout << "                                  3 - Очистить консоль\n";
            cout << "                                  0 - Выйти из программы\n";
            cout << "Выбор: "; cin >> choice;

            if (choice == 1) {
                cout << "Введи границу a: ";
                while (!(cin >> a)) {
                    cout << "Ошибка ввода. Введите число для границы a: ";
                    cin.clear();               // Сброс флага ошибки
                    cin.ignore(10000, '\n');   // Очистка входного буфера
                }

                cout << "Введи границу b: ";
                while (!(cin >> b)) {
                    cout << "Ошибка ввода. Введите число для границы b: ";
                    cin.clear();               // Сброс флага ошибки
                    cin.ignore(10000, '\n');   // Очистка входного буфера
                }

                cout << "Введи точность eps: ";
                while (!(cin >> eps)) {
                    cout << "Ошибка ввода. Введите число для точности eps: ";
                    cin.clear();               // Сброс флага ошибки
                    cin.ignore(10000, '\n');   // Очистка входного буфера
                }
                break;
            }
            else if (choice == 2) {
                a = -4.0;                         // a,b границы интервала [-4, 9]
                b = 9.0;
                eps = 0.001;                     // eps - эпселон (точность)
                break;
            }
            else if (choice == 3) {
                system("cls");
                printCenteredText(text);
            }
            else if (choice == 0) {
                return 0; // Завершение программы
            }
            else {
                cout << "Не верный выбор. Пожалуйста, выберите снова.\n";
                line();
            }
        }
        line();

        double min1 = binlineMethod(a, b, eps, it1);
        cout << "Точка минимума(метод деления отрезком пополам): (" << min1 << ',' << foo(min1) << ')' << " Количество итераций: " << it1 << endl;

        pair<double, double> minimum_right = CubicParabolaMethod(a, b, eps);

        // проверка на то был ли ранее минимум

        pair<double, double> minimum_left = CubicParabolaMethod(a, minimum_right.first - eps, eps);

        if (abs(minimum_right.second - minimum_left.second) > DBL_EPSILON) {
            // минимум найден раньше его и выводим
            cout << "Точка минимума(метод кубической параболы): (" << minimum_left.first << ',' << minimum_left.second << ')' << endl;
        }
        else {
            cout << "Точка минимума(метод кубической параболы): (" << minimum_right.first << ',' << minimum_right.second << ')' << endl;
        }
    } 
    system("pause");
    return 0;
}