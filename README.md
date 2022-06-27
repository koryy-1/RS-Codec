# кодек Рида-Соломона

представляет из себя программу, демонстрирующую работу кодирования и декодирования по алгоритму Рида-Соломона

## как запустить
1) скачайте и разархивируйте codec for linux.zip либо codec for windows.zip 
2) запустите программу через командную строку с аргументами в таком формате: RSCodec.exe <исправляющая способность кода> <шанс ошибки в %, с которой элемент в кодовом слове может быть поврежден> <степень неприводимого полинома> (<неприводимый полином>)
например, формат записи полинома 1+x^2+x^3+x^4+x^8 следующий: 101110001
3) программу можно запускать без аргументов, в таком случае в открывшейся консоли нужно ввести степень полинома, исправляющую способность кода и шанс ошибки.
4) если есть Visual Studio, скачайте проект, запустите Reed–Solomon codes.sln, скомпилируйте и запустите программу, также в корне проекта есть батник с уже введенными аргументами для запуска.

## пример кодера

```
#include <iostream>
#include <ctime>
#include "RSLib.h"
using namespace std;


int main()
{
    int m = 4; // степень RS-полинома
    int n = pow(2, m) - 1; // длина кодового слова
    int t = 3; // количество ошибок, которые мы хотим скорректировать
    int k = n - 2 * t; // длина информационного слова

    int* p = new int[m + 1]{ 1, 1, 0, 0, 1 }; // неприводимый полином

    set_m_t_p(m, t, p);

    calc_n_k(m, t, n, k);

    int* recd = new int[n];
    int* inf_data = new int[k];
    int* b = new int[n - k];

    int* alpha_to = new int[n];
    int* index_of = new int[n];

    // сначала формируем поле Галуа
    generate_gf(alpha_to, index_of);

    // далее вычисляем порождающий полином для этого кода RS
    gen_poly();

    srand(time(0));

    // формируем какие-нибудь данные
    for (size_t i = 0; i < k; i++)
    {
        inf_data[i] = rand() % (n + 1);
    }

    cout << "Results for encode data with Reed-Solomon code (n = 15, k = 9, t = 3)" << endl;

    // ввод данных inf_data и вывод битов четности b в полиномиальной форме
    encode_rs(inf_data, b);


    for (int i = 0; i < k; i++) recd[i] = inf_data[i];
    for (int i = 0; i < n - k; i++)  recd[i + k] = b[i];

	// вывод полученного кодового слова
    for (int i = 0; i < n; i++)
    {
        if (i % 7 == 0)
        {
            cout << endl;
        }
        cout << "[" << i << "]\t= " << recd[i] << "\t";
    }

    system("pause");
}
```

## пример декодера

```
#include <iostream>
#include <ctime>
#include "RSLib.h"
using namespace std;


int main()
{
    int m = 4; // степень RS-полинома
    int n = pow(2, m) - 1; // n = 2 ** m - 1 (длина кодового слова)
    int t = 3; // количество ошибок, которые мы хотим скорректировать
    int k = n - 2 * t; // k = n - 2 * t (длина информационного слова)

    int* p = new int[m + 1]{ 1, 1, 0, 0, 1 }; // неприводимый полином

    set_m_t_p(m, t, p);

    calc_n_k(m, t, n, k);

    int* recd = new int[n] {0, 6, 7, 5, 5, 8, 11, 3, 8, 10, 11, 5, 8, 0, 8};

    int* alpha_to = new int[n];
    int* index_of = new int[n];

    // формируем поле Галуа
    generate_gf(alpha_to, index_of);

    cout << "Results for encode data with Reed-Solomon code (n = 15, k = 9, t = 3)" << endl;

    decode_rs(recd);

    for (int i = 0; i < n; i++)
    {
        if (i % 7 == 0)
        {
            cout << endl;
        }
        cout << "[" << i << "]\t= " << recd[i] << "\t";
    }

    system("pause");
}
```

### алгоритм написан на основе исходников из [статьи Криса Касперского](http://samag.ru/archive/article/211) и страницы [The Error Correcting Codes (ECC) Page](http://www.eccpage.com/)