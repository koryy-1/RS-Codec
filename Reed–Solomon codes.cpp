#include <iostream>
#include <ctime>
#include "RSLib.h"
using namespace std;

void print_array(int* array, int length, string name = "", int* array2 = new int[0], int length2 = 0)
{
    if (name.size() != 0)
        cout << name + ":" << endl;

    if (length < 32)
    {
        if (length2 == 0)
        {
            for (size_t i = 0; i < length; i++)
            {
                if (i % 7 == 0)
                {
                    cout << endl;
                }
                cout << "[" << i << "]\t= " << array[i] << "\t";
            }
        }
        else
        {
            for (size_t i = 0; i < length; i++)
            {
                if (i % 7 == 0)
                {
                    cout << endl;
                }
                cout << "[" << array2[i] << "]\t= " << array[array2[i]] << "\t";
            }
        }
        cout << endl << endl << endl;
    }
    else
    {
        if (length2 == 0)
        {
            for (size_t i = 0; i < 32; i++)
            {
                if (i % 7 == 0)
                {
                    cout << endl;
                }
                cout << "[" << i << "]\t= " << array[i] << "\t";
            }
            cout << endl << "\t\t\t\t\t\t\t..." << endl << endl;
            for (size_t i = length - 1; i > length - 8; i--)
            {
                cout << "[" << i << "]\t= " << array[i] << "\t";
            }
        }
        else
        {
            for (size_t i = 0; i < 32; i++)
            {
                if (i % 7 == 0)
                {
                    cout << endl;
                }
                cout << "[" << array2[i] << "]\t= " << array[array2[i]] << "\t";
            }
            cout << endl << "\t\t\t\t\t\t\t..." << endl << endl;
            for (size_t i = length - 1; i > length - 8; i--)
            {
                cout << "[" << array2[i] << "]\t= " << array[array2[i]] << "\t";
            }
        }
        cout << endl << endl << endl;
    }
}

int *set_irreducible_polynomial(int m, int* p)
{
    /**** неприводимые полиномы ****/
    if (m == 2)
        //int p[m + 1] = { 1,1,0,1 }; /* 1 + x + x^3 */
        p = new int[m + 1]{ 1,1,1 };

    else if (m == 3)
        //int p[m + 1] = { 1,1,0,1 }; /* 1 + x + x^3 */
        p = new int[m + 1]{ 1,1,0,1 };

    else if (m == 4)
        //int p[m + 1] = { 1, 1, 0, 0, 1 }; /* 1 + x + x^4 */
        p = new int[m + 1]{ 1, 1, 0, 0, 1 };

    else if (m == 5)
        //int p[m + 1] = { 1, 0, 1, 0, 0, 1 }; /* 1 + x^2 + x^5 */
        p = new int[m + 1]{ 1, 0, 1, 0, 0, 1 };

    else if (m == 6)
        p = new int[m + 1]{ 1, 1, 0, 0, 0, 0, 1 };

    else if (m == 7)
        p = new int[m + 1]{ 1, 0, 0, 1, 0, 0, 0, 1 };

    else if (m == 8)
        //int p[m + 1] = { 1, 0, 1, 1, 1, 0, 0, 0, 1 }; /* 1+x^2+x^3+x^4+x^8 */
        p = new int[m + 1]{ 1, 0, 1, 1, 1, 0, 0, 0, 1 };

    else if (m == 9)
        p = new int[m + 1]{ 1, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

    else if (m == 10)
        p = new int[m + 1]{ 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1 };

    else if (m == 11)
        p = new int[m + 1]{ 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 };

    else if (m == 12)
        p = new int[m + 1]{ 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1 };
    else
    {
        cout << "invalid data";
        system("pause");
        exit(1);
    }
    return p;
}

int main()
{
    int m; // степень RS-полинома
    int n; // n = 2 ** m - 1 (длина кодового слова)
    int t; // количество ошибок, которые мы хотим скорректировать
    int k; // k = n - 2 * t (длина информационного слова)

    int error_chance;

    int* p = new int[0];

    string command_for_random_data = "";
    string command_for_select_polynom = "";
    if (__argc == 1)
    {
        cout << "how data will be entered?" << endl << endl
            << "1 - automatically" << endl << "2 - manually" << endl;
        
        cin >> command_for_random_data;

        cout << "how polynomials for Galois field will be entered?" << endl << endl
            << "1 - automatically" << endl << "2 - manually" << endl;
        
        cin >> command_for_select_polynom;

        if (command_for_select_polynom == "1")
        {
            cout << "available polynomials for Galois field generation:" << endl
                << "1 + x + x^2" << endl
                << "1 + x + x^3" << endl
                << "1 + x + x^4" << endl
                << "1 + x^2 + x^5" << endl
                << "1 + x + x^6" << endl
                << "1 + x^3 + x^7" << endl
                << "1 + x^2 + x^3 + x^4 + x^8" << endl
                << "1 + x^4 + x^9" << endl
                << "1 + x^3 + x^10" << endl
                << "1 + x^2 + x^11" << endl
                << "1 + x + x^4 + x^6 + x^12" << endl;
            cout << endl << "Enter polynomial degree: ";
            cin >> m;
            cout << endl;
            p = set_irreducible_polynomial(m, p);
            print_array(p, m + 1, "Entered irreducible_polynomial");
        }
        else if (command_for_select_polynom == "2")
        {
            cout << endl << "Enter polynomial degree: ";
            cin >> m;
            if (!(1 < m < 20))
            {
                cout << "invalid data\n";
                system("pause");
                return 1;
            }
            p = new int[m + 1];
            cout << "Example for inputting data for polynomial 1+x^2+x^3+x^4+x^8 is given below:" << endl;
            cout << "{ 1, 0, 1, 1, 1, 0, 0, 0, 1 }" << endl;
            for (size_t i = 0; i < m + 1; i++)
            {
                cout << "Enter p[" << i << "]: ";
                cin >> p[i];
            }
        }
        else
        {
            cout << "bad try";
            system("pause");
            return 1;
        }

        cout << "Enter number of errors that can be corrected (no more than " << pow(2, m) / 2 - 1 << " errors): ";
        cin >> t;

        if (pow(2, m) / 2 <= t)
        {
            cout << "too many errors for this code\n";
            system("pause");
            return 1;
        }

        cout << "Enter chance of error as percentage with which the element can be corrupted: ";
        cin >> error_chance;

        if (!( - 1 < error_chance < 101))
        {
            cout << "invalid data\n";
            system("pause");
            return 1;
        }
    }
    else if (__argc == 4 || __argc == 5)
    {
        t = atoi(__argv[1]);
        error_chance = atoi(__argv[2]);
        
        m = atoi(__argv[3]);
        command_for_random_data = "1";

        if (pow(2, m) / 2 <= t || !(-1 < error_chance < 101) || !(1 < m < 20))
        {
            cout << "invalid data\n";
            system("pause");
            return 1;
        }
        
        if (__argc == 4)
            p = set_irreducible_polynomial(m, p);
        else if (__argc == 5)
        {
            string pStr = __argv[4];
            if (pStr.size() != m + 1)
            {
                cout << "invalid data\n";
                system("pause");
                return 1;
            }

            p = new int[m + 1];
            for (size_t i = 0; i < pStr.size(); i++)
                p[i] = int(pStr[i]) - 48;
        }
    }
    else
    {
        cout << "invalid args, program must be in form: " << __argv[0] << " <t> <chance of error> <m> (<p[]>) or without argc\n";
        system("pause");
        return 1;
    }

    
    set_m_t_p(m, t, p);

    calc_n_k(m, t, n, k);
    
    int* recd = new int[n];
    int* inf_data = new int[k];
    int* b = new int[n - k];

    int* alpha_to = new int[n];
    int* index_of = new int[n];

    generate_gf(alpha_to, index_of);

    system("cls");

    cout << endl << "----------------------------------------------------GF" << pow(2, m) << "----------------------------------------------------"
        << endl << endl;

    cout << "null = " << 0;
    print_array(alpha_to, n);

    cout << "------------------------------------------------------------------------------------------------------------"
        << endl << endl;

    //вычислим порождающий полином для этого кода RS
    gen_poly();

    srand(time(0));

    if (command_for_random_data == "1")
    {
        for (size_t i = 0; i < k; i++)
        {
            inf_data[i] = rand() % (n + 1);
        }
    }
    else if (command_for_random_data == "2")
    {
        for (size_t i = 0; i < k; i++)
        {
            cout << "Enter inf_data[" << i << "]: ";
            cin >> inf_data[i];
        }
    }
    else
    {
        cout << "bad try";
        return 1;
    }

    cout << "========== Results for Reed-Solomon code (n = " << n << ", k = " << k << ", t = " << t << ") ==========" << endl << endl;

    //ввод данных inf_data и вывод битов четности b в полиномиальной форме
    encode_rs(inf_data, b);

    print_array(inf_data, k, "data");

    print_array(b, 2 * t, "parity bits");

    for (int i = 0; i < k; i++) recd[i] = inf_data[i];
    for (int i = 0; i < n - k; i++)  recd[i + k] = b[i];

    int* old_recd = new int[n];

    for (size_t i = 0; i < n; i++)
    {
        old_recd[i] = recd[i];
    }

    print_array(recd, n, "code word");

    srand(time(0));

    int num_corrupted_el = round(n * error_chance / 100.);
    int* error_idx = new int[num_corrupted_el];
    for (size_t i = 0; i < num_corrupted_el; i++)
    {
        error_idx[i] = rand() % n;
        recd[error_idx[i]] = rand() % n; // впишем  ошибки
    }

    print_array(recd, num_corrupted_el, "errors", error_idx, num_corrupted_el);
    delete[] error_idx;

    print_array(recd, n, "recd with errors");

    decode_rs(recd);

    print_array(recd, n, "decode recd");

    bool failed = false;
    for (size_t i = 0; i < n; i++)
    {
        if (old_recd[i] != recd[i])
        {
            failed = true;
            cout << "element failed verification [" << i << "]\t= " << recd[i] << endl;
        }
    }
    cout << endl;

    if (failed) cout << "errors were made during decoding";
    else cout << "code successfully decoded";

    cout << endl << endl;

    system("pause");
}
