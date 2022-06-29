# Кодек Рида-Соломона

Представляет из себя программу, демонстрирующую работу кодирования и декодирования по алгоритму Рида-Соломона

## Кодек работает по следующему алгоритму:

генерирует таблицу для GF(2 ^ m) на основе неприводимого полинома Pc от p[0] до p[m].
Таблица:
index->polynomial из alpha_to[] содержит j = alpha ^ i, где alpha есть примитивный член, обычно равный 2,
а ^ - операция возведения в степень;
polynomial form -> index из index_of[j = alpha ^ i] = i;

далее получаем порождающий полином коррекции tt-ошибок для данного РС кода (n, k)
из произведения (X+@^(b0+i)), i = 0,...,(2*tt-1)

Примеры:
Если b0 = 1, tt = 1. deg(g(x)) = 2*tt = 2.

г(х) = (х+@) (х+@^2)

Если b0 = 0, tt = 2. deg(g(x)) = 2*tt = 4.

г(х) = (х+1) (х+@) (х+@^2) (х+@^3)

для того чтобы закодировать сообщение, кодируемые данные передаются через массив data[i], где i=0..(k-1), а сгенерированные символы четности заносятся в массив b[0]..b[2*t-1]. При этом:
- Кодирование производится с использованием сдвигового feedback-регистра, заполненного соответствующими элементами массива g[] с порожденным полиномом внутри.
- Исходные и результирующие данные должны быть представлены в полиномиальной форме (т.е. в обычной форме машинного представления данных).
- Сгенерированное кодовое слово описывается следующей формулой:
с(x) = data(x)*x^(n-k) + b(x)

Процедура декодирования кодов Рида-Соломона состоит из нескольких шагов: 
1) вычисляем 2t-символьный синдром путем постановки alpha**i в recd(x), где recd – полученное кодовое слово, предварительно переведенное в индексную форму. По факту вычисления recd(x) записываем очередной символ синдрома в s[i], где i принимает значение от 1 до 2t, оставляя s[0] равным нулю.
2) используя итеративный алгоритм Берлекэмпа, находим полином локатора ошибки – elp[i].
Если степень elp превышает собой величину t, мы бессильны скорректировать все ошибки и ограничиваемся выводом сообщения о неустранимой ошибке, после чего совершаем аварийный выход из декодера.
Если же степень elp не превышает t, подставляем alpha**i, где i = 1..n в elp для вычисления
корней полинома.
3) Обращение найденных корней дает позиции искаженных символов. 
- Если количество определенных позиций искаженных символов меньше степени elp, искажению подверглось более чем t
символов и мы не можем восстановить их.
- Во всех остальных случаях восстановление оригинального
содержимого искаженных символов вполне возможно.
- В случае, когда количество ошибок заведомо велико,
для их исправления декодируемые символы проходят сквозь декодер без каких-либо изменений.
4) Реализуется процедура Ченя — нахождение корней многочлена локатора ошибок.
5) Используется алгоритм Форни для вычисления значения ошибки.
6) Вносятся корректирующие поправки в искаженные кодовые слова;

## Как запустить
1) скачайте и разархивируйте codec for linux.zip либо codec for windows.zip 
2) запустите программу через командную строку с аргументами в таком формате: RSCodec.exe <исправляющая способность кода> <шанс ошибки в %, с которой элемент в кодовом слове может быть поврежден> <степень неприводимого полинома> (<неприводимый полином>).

Пример запуска программы в командной строке для полинома 1 + x + x^4 следующий:
```
RSCodec.exe 3 20 4 11001
```
3) программу можно запускать без аргументов, в таком случае в открывшейся консоли нужно ввести степень полинома, исправляющую способность кода и шанс ошибки.
4) если есть Visual Studio, скачайте проект, запустите Reed–Solomon codes.sln, скомпилируйте и запустите программу, также в корне проекта есть батник с уже введенными аргументами для запуска.

### Алгоритм написан на основе исходников из [статьи Криса Касперского](http://samag.ru/archive/article/211) и страницы [The Error Correcting Codes (ECC) Page](http://www.eccpage.com/)

Ниже будут приведены примеры кодера и декодера. Чтобы их использовать подключите 2 файла RSLib.h и RSLib.cpp в проект а в файл с функцией main() вставьте следующие строки кода:

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