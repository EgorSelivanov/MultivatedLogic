#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

//не работает нормально аналог скнф, посмотреть функции записи в файл, обработку ввода типа x=>

int k = 0, // значность логики
n = 0; // количество переменных
int x; // рассматриваемые точки отрезка [a;b]
int y;

int l, // длина выражения f
i, // переменная-счётчик
q; // вспомогательная переменная-счетчик - курсор позиции в распознаваемом выражении
std::string f; // рассматриваемая функция

int partCalculation(int x, int y, int v, bool flag);
int analyzeExpr(int x, int y, int v);
int Numb();
int input_correct();

int functionPow4(int fa)
{
    fa = (int)pow(fa, 4) % k;
    return fa;
}
int functionImplication(int x, int y)
{
    int impl = 0;
    if (x < y)
        impl = k - 1;
    else
        impl = k - 1 - x + y;
    return impl;
}

int Fa(int x, int y, bool flag) // вычисление значения функции по выражению (в строке f)
{
    int v = NULL;
    q++;
    if (f[q] != '-' && q < l)
        v = partCalculation(x, y, v, flag);
    else if (f[q] == '-' && q < l)
    {
        q++;
        v = partCalculation(x, y, v, flag);
    }
    else
    {
        std::cout << "Введена некорректная формула!" << std::endl;
        exit(1);
    }
    if (!flag)
        q--;
    return v;
}

int partCalculation(int x, int y, int v, bool flag) // вычисление части функции f
{
    v = analyzeExpr(x, y, v);
    if (flag)
    {
        while (q < l)
        {
            v = analyzeExpr(x, y, v);
        }
    }
    return v;
}

int analyzeExpr(int x, int y, int v) // вычисление следующего дальше выражения, если оно:
{
    if (f[q] == '(') // сложное выражение
    {
        v = Fa(x, y, true);
    }
    else
        if (f[q] >= '0' && f[q] <= '9') // цифра (т.е. начинается число)
            v = Numb();
        else
            if (f[q] == '^' && f[q + 1] == '4') // функция степени
            {
                if (v != NULL)
                {
                    q = q + 1;
                    v = functionPow4(v);
                }
                else
                {
                    std::cout << "Введена некорректная формула." << std::endl;
                    exit(1);
                }
            }
            else if (f[q] == ')' && f[q + 1] == '^' && f[q + 2] == '4') // функция степени
            {
                if (v != NULL)
                {
                    q = q + 2;
                    v = functionPow4(v);
                }
                else
                {
                    std::cout << "Введена некорректная формула." << std::endl;
                    exit(1);
                }
            }
            else if (f[q] == '=' && f[q + 1] == '>') // функция импликации
            {
                q = q + 1;
                v = functionImplication(v, Fa(x, y, false));
            }
            else if (f[q] == 'x')
            {
                if (f[q + 1] == '^' && f[q + 2] == '4')
                {
                    q = q + 2;
                    v = functionPow4(x);
                }
                else
                    v = x;
            }
            else if (f[q] == 'y')
            {
                if (n == 2)
                {
                    if (f[q + 1] == '^' && f[q + 2] == '4')
                    {
                        q = q + 2;
                        v = functionPow4(y);
                    }
                    else
                        v = y;
                }
                else
                {
                    std::cout << "Введено неподдерживаемое значение." << std::endl;
                    exit(1);
                }
            }
            else // неподдерживаемое значение
            {
                std::cout << "Введено неподдерживаемое значение." << std::endl;
                exit(1);
            }
    q++;
    return v;
}

int Numb() // считывание числа
{
    float u = 0;
    int num = 0,
        razr = 0;

    while (((f[q] >= '0' && f[q] <= '9') || f[q] == '.') && q < l)
    {
        num = razr * num + f[q] - '0';
        razr += 10;
        if (q > 0)
            if (f[q - 1] == '-' && q > 0)
                num = -1 * num;
        q++;
    }
    if (num >= k || num < 0)
    {
        std::cout << "Некорректный ввод, т.к. встречено число >= k или < 0!" << std::endl;
        exit(1);
    }
    q--;
    return num;
}

int qF(int x, int y = -1) // присвоение q = -1 и вызов функции Fa(x) - подготовка к вычислению f(x) f(x,y) по выражению
{
    q = -1;
    return Fa(x, y, true);
}

void sknfX(int* resultMas)
{
    std::ofstream fout("output.txt", std::ios_base::out | std::ios_base::app);
    std::cout << "Условные обозначения:" << std::endl;
    std::cout << "Характеристическая функция второго рода числа i: I_i(x)" << std::endl;
    std::cout << "Отрицание Лукасевича: ~" << std::endl;
    std::cout << "Дизъюнкция: +" << std::endl;
    std::cout << "Конъюнкция: &" << std::endl;
    int last = 0;
    for (x = k - 1; x >= 0; x--)
    {
        if (resultMas[x] < k - 1)
        {
            last = x;
            break;
        }
    }
    for (x = 0; x < k; x++)
    {
        if (resultMas[x] < k - 1)
        {
            if (x != last)
            {
                fout << "(" << resultMas[x] << " + ~I_" << x << "(x)) & ";
                std::cout << "(" << resultMas[x] << " + ~I_" << x << "(x)) & ";
            }
            else
            {
                fout << "(" << resultMas[x] << " + ~I_" << x << "(x))";
                std::cout << "(" << resultMas[x] << " + ~I_" << x << "(x))";
            }
        }
    }
    std::cout << std::endl;
}

void sknfXY(int** resultMas)
{
    std::ofstream fout("output.txt", std::ios_base::out | std::ios_base::app);
    std::cout << "Условные обозначения:" << std::endl;
    std::cout << "Характеристическая функция второго рода числа i: I_i(x)" << std::endl;
    std::cout << "Отрицание Лукасевича: ~" << std::endl;
    std::cout << "Дизъюнкция: +" << std::endl;
    std::cout << "Конъюнкция: &" << std::endl;
    int lastX = 0;
    int lastY = 0;
    bool flag = false;
    for (x = k - 1; x >= 0; x--)
    {
        for (y = k - 1; y >= 0; y--)
            if (resultMas[x][y] < k - 1)
            {
                lastX = x;
                lastY = y;
                flag = true;
                break;
            }
        if (flag)
            break;
    }
    for (x = 0; x < k; x++)
    {
        for (y = 0; y < k; y++)
            if (resultMas[x][y] < k - 1)
            {
                if (x != lastX || y != lastY)
                {
                    std::cout << "(" << resultMas[x][y] << " + ~I_" << x << "(x) + ~I_" << y << "(y)) & ";
                    fout << "(" << resultMas[x][y] << " + ~I_" << x << "(x) + ~I_" << y << "(y)) & ";
                }
                else
                {
                    std::cout << "(" << resultMas[x][y] << " + ~I_" << x << "(x) + ~I_" << y << "(y))";
                    fout << "(" << resultMas[x][y] << " + ~I_" << x << "(x) + ~I_" << y << "(y))";
                }
            }
    }
    fout << std::endl;
    std::cout << std::endl;
}

void consoleIn()
{
    while (1)
    {
        std::cout << "Введите число k-значной логики:" << std::endl;
        k = input_correct();
        if (k > 2)
            break;
        else
            std::cout << "Значение k не может быть <= 2!" << std::endl;
    }

    while (1)
    {
        std::cout << "Введите число n(1 или 2) существенных переменных задаваемой функции k-значной логики" << std::endl;
        n = input_correct();
        if ((n == 1) || (n == 2))
            break;
        else
            std::cout << "Значение n может равняться только 1 или 2!" << std::endl;
    }
    std::cout << "Условные обозначения:" << std::endl;
    std::cout << "Возведение в 4 степень: x^4" << std::endl;
    std::cout << "Импликация: x=>y" << std::endl;
    std::cout << "Введите функцию k-значной логики в аналитической форме:" << std::endl;
    std::getline(std::cin, f);

    l = f.length();
}

int input_correct()
{
    int val;
    using namespace std;
    while (1)
    {
        std::cout << '>';
        cin >> std::noskipws >> val;
        if (cin.fail()) // если никакого извлечения не произошло
        {
            cin.clear(); // то сбрасываем все текущие флаги состояния и устанавливаем goodbit, чтобы иметь возможность использовать функцию ignore()
            cin.ignore(32767, '\n'); // очищаем поток от мусора
            std::cout << "Ввод не корректен! Введите символ снова!" << endl;
            continue; // просим пользователя ввести свой возраст еще раз
        }

        cin.ignore(32767, '\n'); // очищаем весь мусор, который остался в потоке после извлечения
        if (cin.gcount() > 1) // если мы очистили более одного символа
        {
            std::cout << "Ввод не корректен! Введите символ снова!" << endl;
            continue; // то этот ввод считается некорректным, и мы просим пользователя ввести свой возраст еще раз
        }

        break;
    }
    return val;
}

int main()
{
    setlocale(LC_ALL, "rus");
    std::ifstream fin;
    fin.open("input.txt");
    std::ofstream fout;
    fout.open("output.txt", std::ofstream::out | std::ofstream::trunc);
    fout.close();
    fout.open("output.txt", std::ios_base::out | std::ios_base::app);

    int num;
    std::cout << "Программу выполнил студент группы 4210 Селиванов Егор (14 порядковый номер)." << std::endl;

    while (1)
    {
        std::string rec;
        std::cout << "Введите 0 для загрузки значения из файла или любой другой символ для ввода из консоли." << std::endl;
        std::getline(std::cin, rec);
        if (rec == "0")
        {
            try
            {
                fin >> k;
                fin >> n;
                fin >> f;
                l = f.length();
            }
            catch (...)
            {
                std::cout << "Ошибка чтения файла!" << std::endl;
                return 1;
            }
            if (k <= 2)
            {
                std::cout << "Значение k не может быть <=2!" << std::endl;
                return 1;
            }
            if (n != 1 && n != 2)
            {
                std::cout << "Значение n может равняться только 1 или 2!" << std::endl;
                return 1;
            }
        }
        else
            consoleIn();
        if (n == 1)
        {
            int* resultMas = new int[k];
            std::cout << "f(x) = " << f << std::endl;
            fout << "f(x) = " << f << std::endl;
            // вывод шапки таблицы значений
            std::cout << "   x    |  f(x) " << std::endl;
            std::cout << "--------|--------" << std::endl;
            fout << "   x    |  f(x) " << std::endl;
            fout << "--------|--------" << std::endl;
            for (x = 0; x < k; x++)
            {
                int result = qF(x);
                resultMas[x] = result;
                std::cout << "   " << x << "    |  " << result << std::endl;
                fout << "   " << x << "    |  " << result << std::endl;
            }
            std::cout << std::endl;
            fout << std::endl;
            std::cout << "Аналог СКНФ:" << std::endl;
            fout << "Аналог СКНФ:" << std::endl;
            sknfX(resultMas);
            std::cout << std::endl;
            fout << std::endl;
            std::cout << "Информация о принадлежности заданной функции классу T(E):" << std::endl;
            std::cout << "Введите множество E через пробел:" << std::endl;
            fout << "Информация о принадлежности заданной функции классу T(E):" << std::endl;
            int* classT = new int[k];
            int i = 0;
            std::string str;
            while (1) {
                std::getline(std::cin, str);
                std::istringstream stream(str);
                int x;
                bool flag = true;
                while (stream >> x)
                {
                    if (x >= k || x < 0)
                    {
                        std::cout << "Значения не могут быть >= k или < 0" << std::endl;
                        flag = false;
                        break;
                    }
                    else
                    {
                        classT[i] = x;
                        i++;
                    }
                }
                if (stream.eof())
                {
                    if (flag)
                        break;
                }
                else
                    std::cout << "Введено некорректное значение! Попробуйте снова!" << std::endl;
            }

            bool affiliation = true;
            for (x = 0; x < k; x++)
            {
                bool flag = false;
                for (int j = 0; j < i; j++)
                {
                    if (x == classT[j])
                    {
                        flag = true;
                        break;
                    }
                }
                if (flag)
                {
                    flag = false;
                    for (int j = 0; j < i; j++)
                    {
                        if (resultMas[x] == classT[j])
                        {
                            flag = true;
                            break;
                        }
                    }
                    if (!flag)
                    {
                        std::cout << "Функция не принадлежит данному классу T(E)." << std::endl;
                        fout << "Функция не принадлежит данному классу T(E)." << std::endl;
                        fout << "Множество Е:" << std::endl;
                        for (int j = 0; j < i; j++)
                            fout << classT[j] << " ";
                        affiliation = false;
                        break;
                    }
                }
                if (!affiliation)
                    break;
            }
            if (affiliation)
            {
                std::cout << "Функция принадлежит данному классу T(E)." << std::endl;
                fout << "Функция принадлежит данному классу T(E)." << std::endl;
                fout << "Множество Е:" << std::endl;
                for (int j = 0; j < i; j++)
                    fout << classT[j] << " ";
            }
        }
        else if (n == 2)
        {
            int** resultMas = new int* [k];
            for (int i = 0; i < k; i++)
                resultMas[i] = new int[k];
            std::cout << "f(x, y) = " << f << std::endl;
            fout << "f(x, y) = " << f << std::endl;
            // вывод шапки таблицы значений
            std::cout << "   x    |   y    | f(x, y)" << std::endl;
            std::cout << "--------|--------|--------" << std::endl;
            fout << "   x    |   y    | f(x, y)" << std::endl;
            fout << "--------|--------|--------" << std::endl;
            for (x = 0; x < k; x++)
            {
                for (y = 0; y < k; y++)
                {
                    int result = qF(x, y);
                    resultMas[x][y] = result;
                    std::cout << "   " << x << "    |   " << y << "    | " << result << std::endl;
                    fout << "   " << x << "    |   " << y << "    | " << result << std::endl;
                }
            }
            std::cout << "Аналог СКНФ:" << std::endl;
            fout << "Аналог СКНФ:" << std::endl;
            sknfXY(resultMas);

            std::cout << std::endl;
            fout << std::endl;

            std::cout << "Информация о принадлежности заданной функции классу T(E):" << std::endl;
            std::cout << "Введите множество E через пробел:" << std::endl;
            fout << "Информация о принадлежности заданной функции классу T(E):" << std::endl;

            int* classT = new int[k];
            int i = 0;
            std::string str;
            while (1) {
                std::getline(std::cin, str);
                std::istringstream stream(str);
                int x;
                bool flag = true;
                while (stream >> x)
                {
                    if (x >= k)
                    {
                        std::cout << "Значения не могут быть >= k" << std::endl;
                        flag = false;
                        break;
                    }
                    else
                    {
                        classT[i] = x;
                        i++;
                    }
                }
                if (stream.eof())
                {
                    if (flag)
                        break;
                }
                else
                    std::cout << "Введено некорректное значение! Попробуйте снова!" << std::endl;
            }

            bool affiliation = true;
            for (x = 0; x < k; x++)
            {
                bool flag = false;
                for (int j = 0; j < i; j++)
                {
                    if (x == classT[j])
                    {
                        flag = true;
                        break;
                    }
                }
                if (flag)
                {
                    for (y = 0; y < k; y++)
                    {
                        flag = false;
                        for (int j = 0; j < i; j++)
                        {
                            if (y == classT[j])
                            {
                                flag = true;
                                break;
                            }
                        }
                        if (flag)
                        {
                            flag = false;
                            for (int j = 0; j < i; j++)
                            {
                                if (resultMas[x][y] == classT[j])
                                {
                                    flag = true;
                                    break;
                                }
                            }
                            if (!flag)
                            {
                                std::cout << "Функция не принадлежит данному классу T(E)." << std::endl;
                                fout << "Функция не принадлежит данному классу T(E)." << std::endl;
                                fout << "Множество Е:" << std::endl;
                                for (int j = 0; j < i; j++)
                                    fout << classT[j] << " ";
                                affiliation = false;
                                break;
                            }
                        }
                        else
                            break;
                    }
                }
                if (!affiliation)
                    break;
            }
            if (affiliation)
            {
                std::cout << "Функция принадлежит данному классу T(E)." << std::endl;
                fout << "Функция принадлежит данному классу T(E)." << std::endl;
                fout << "Множество Е:" << std::endl;
                for (int j = 0; j < i; j++)
                    fout << classT[j] << " ";
            }
        }

        std::string repeat;
        std::cout << "Введите 0 для завершения программы или любой другой символ, чтобы продолжить." << std::endl;
        std::getline(std::cin, repeat);
        if (repeat == "0")
            break;
    }
    fin.close();
    fout.close();
    return 0;
}
