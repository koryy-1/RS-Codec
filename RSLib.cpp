#include "RSLib.h"
#include <math.h>

using namespace std;

int m; // ������� RS-��������
int n; // n = 2 ** m - 1 (����� �������� �����)
int t; // ���������� ������, ������� �� ����� ���������������
int k; // k = n - 2 * t (����� ��������������� �����)


int* p; // ������������ �������
int* g; // ����������� �������
int* alpha_to; //������� �������� ������������ �����
int* index_of; //��������� ������� ��� �������� ���������

void calc_n_k(int mm, int tt, int& nn, int& kk)
{
    nn = pow(2, mm) - 1;
    kk = nn - 2 * tt;
}

void set_m_t_p(int mm, int tt, int* pp)
{
    m = mm;
    t = tt;
    calc_n_k(m, t, n, k);
    /*n = pow(2, m) - 1;
    k = n - 2 * t;*/
    //p = pp;
    p = new int[m + 1];
    for (int i = 0; i < m + 1; i++)
    {
        p[i] = pp[i];
    }
    alpha_to = new int[n + 1];
    index_of = new int[n + 1];
    g = new int[n - k + 1];
}

int *generate_gf(int* l_alpha_to, int* l_index_of)
{
    //----------------------------------------------------------------------------
    // ���������� look-up ������� ��� GF(2 ^ m) �� ������
    // ������������� ������������ �������� Pc �� p[0] �� p[m].
    //
    // look-up �������:
    // index->polynomial �� alpha_to[] �������� j = alpha ^ i,
    // ��� alpha ���� ����������� ����, ������ ������ 2
    // � ^ - �������� ���������� � ������� (�� XOR!);
    //
    // polynomial form -> index �� index_of[j = alpha ^ i] = i;
    //----------------------------------------------------------------------------

    int i, mask;

    mask = 1; alpha_to[m] = 0;

    for (i = 0; i < m; i++)
    {
        alpha_to[i] = mask;
        index_of[alpha_to[i]] = i;

        if (p[i] != 0) alpha_to[m] ^= mask;
        mask <<= 1;
    }
    index_of[alpha_to[m]] = m;
    mask >>= 1;

    for (i = m + 1; i < n; i++)
    {
        if (alpha_to[i - 1] >= mask)
            alpha_to[i] = alpha_to[m] ^ ((alpha_to[i - 1] ^ mask) << 1);
        else
            alpha_to[i] = alpha_to[i - 1] << 1;

        index_of[alpha_to[i]] = i;
    }
    index_of[0] = -1;

    for (int i = 0; i < n; i++)
    {
        l_alpha_to[i] = alpha_to[i];
        l_index_of[i] = index_of[i];
    }
    
    return l_alpha_to;
}

int *gen_poly()
{
    register int i, j;

    g[0] = 2;    /* primitive element alpha = 2  for GF(2**mm)  */
    g[1] = 1;    /* g(x) = (X+alpha) initially */
    for (i = 2; i <= n - k; i++)
    {
        g[i] = 1;
        for (j = i - 1; j > 0; j--)
            if (g[j] != 0)  g[j] = g[j - 1] ^ alpha_to[(index_of[g[j]] + i) % n];
            else g[j] = g[j - 1];
        g[0] = alpha_to[(index_of[g[0]] + i) % n];     /* g[0] can never be zero */
    }
    /* convert g[] to index form for quicker encoding */
    for (i = 0; i <= n - k; i++)  g[i] = index_of[g[i]];
    return g;
}

/*-----------------------------------------------------------------------------------------------------

*                           ����� ����-��������

*                           ========================

* ���������� ������ ���������� ����� ������ data[i], ��� i=0..(k-1), � ��������������� ������� ��������

* ��������� � ������ b[0]..b[2*t-1]. �������� � �������������� ������ ������ ���� ������������

* � �������������� ����� (�.�. � ������� ����� ��������� ������������� ������).

* ����������� ������������ � �������������� ���������� feedback-��������, ������������ ����������������

* ���������� ������� g[] � ����������� ��������� ������. ��������������� ������� ����� ����������� ��������� ��������:

* �(x) = data(x)*x^(n-k) + b(x)

���������������������������������������������������������---------------------------------------------*/
int *encode_rs(int* inf_data, int* b)
{
    int i, j;

    int feedback;

    for (i = 0; i < n - k; i++) b[i] = 0;

    // ������������ ��� ������� �������� ������ ������ ������

    for (i = k - 1; i >= 0; i--)

    {

        // ������� (data[i] + b[n � k �1]) � ��������� �� g[i], �.�. ���������� ��������� ������������

        // ������ �������� ������ � ������� �������� ����� �������� (���������������� ��������� b2t-1,

        // � ��������� ��� � ��������� �����, �������� ��������� � �������� feedback,

        // ����� ���� �������� ���� ������������ ���������

        feedback = index_of[inf_data[i] ^ b[n - k - 1]];

        // ���� ��� ������� ��� ���������?

        if (feedback != -1)
        {

            // ������������ ����� ���� bx-���������

            for (j = n - k - 1; j > 0; j--)

                // ���� ������� ����������� g � ��� �������������� (�.�. ��������� �����������,

                // �� �������� feedback �� ��������������� g-����������� � ���������� ���

                // �� ��������� ��������� �������

                if (g[j] != -1) //�

                    b[j] = b[j - 1] ^ alpha_to[(g[j] + feedback) % n];

                else

                    // ���� ������� ����������� g � ��� ������� �����������, ��������� ���� ����

                    // ����� ��� ���������, ��������� ������ �� ������ m-�������� � ������

                    b[j] = b[j - 1];

            // �������������� ��������� ������ � ������� ����� b0-�������

            b[0] = alpha_to[(g[0] + feedback) % n];

        }
        else
        {  // ������� ���������, ������������ ��������� ����� ��������, �� ������ ��������

           // ����� �������, ������� ��������, � � ����� �������� � ������� �������

            for (j = n - k - 1; j > 0; j--) b[j] = b[j - 1]; b[0] = 0;

        }

    }

    return b;
}

/*-----------------------------------------------------------------------------------------------------

*                                     ������� ����-��������

*                                     =====================

* ��������� ������������� ����� ����-�������� ������� �� ���������� �����: ������� �� ���������

* 2t-���������� ������� ����� ���������� alpha**i � recd(x), ��� recd � ���������� ������� �����,

* �������������� ������������ � ��������� �����. �� ����� ���������� recd(x) �� ����������

* ��������� ������ �������� � s[i], ��� i ��������� �������� �� 1 �� 2t, �������� s[0] ������ ����.

* �����, ��������� ����������� �������� ����������, �� ������� ������� �������� ������ � elp[i].

* ���� ������� elp ��������� ����� �������� t, �� ��������� ��������������� ��� ������ � ��������������

* ������� ��������� � ������������ ������, ����� ���� ��������� ��������� ����� �� ��������.

* ���� �� ������� elp �� ��������� t, �� ����������� alpha**i, ��� i = 1..n � elp ��� ����������

* ������ ��������. ��������� ��������� ������ ���� ��� ������� ���������� ��������. ���� ����������

* ������������ ������� ���������� �������� ������ ������� elp, ��������� ����������� ����� ��� t

* �������� � �� �� ����� ������������ ��. �� ���� ��������� ������� �������������� �������������

* ����������� ���������� �������� ������ ��������. � ������, ����� ���������� ������ �������� ������,

* ��� �� ����������� ������������ ������� �������� ������ ������� ��� �����-���� ���������.

���������������������������������������������������������--------------------------------------------*/

int *decode_rs(int* recd)
{
    int i, j, u, q;

    //int s[n - k + 1];              // ������� �������� ������
    int* s = new int[n - k + 1];

    //int elp[n - k + 2][n - k]; // ������� �������� ������ ������
    int** elp = new int* [n - k + 2];
    for (size_t i = 0; i < n - k + 2; i++)
    {
        elp[i] = new int[n - k];
    }

    //int d[n - k + 2];
    int* d = new int[n - k + 2];

    //int l[n - k + 2];
    int* l = new int[n - k + 2];

    //int u_lu[n - k + 2];
    int* u_lu = new int[n - k + 2];

    int count = 0, syn_error = 0;/* root[t], loc[t], z[t + 1], err[n], reg[t + 1];*/
    int* root = new int[t];
    int* loc = new int[t];
    int* z = new int[t + 1];
    int* err = new int[n];
    int* reg = new int[t + 1];


    // ��������� ���������� ������� ����� � ��������� ����� ��� ��������� ����������

    for (i = 0; i < n; i++) recd[i] = index_of[recd[i]];

    // ��������� �������

    //-------------------------------------------------------------------------------------------------

    for (i = 1; i <= n - k; i++)
    {

        s[i] = 0;     // ������������� s-�������� (�� ��� ���� �� ��������� ��������� ����)   

        // ��������� s[i] += recd[j]*ij �.�. ����� ��������� ������ ������������ ������, �������� ���

        // �� ���������� ����� ������� �������, ���������� �� ����� ���������� ������� � ����������

        // ���������� ��������� � ���������� s-�������� �� ����� ���������� ���� ������������ ��������,

        // �� ��������� ���� ���� ���������� ����� � �� ������ ���� ��� ������� ������� ��������

        for (j = 0; j < n; j++) if (recd[j] != -1) s[i] ^= alpha_to[(recd[j] + i * j) % n];

        if (s[i] != 0) syn_error = 1;  // ���� ������� �� ����� ����, ������� ���� ������   

        // ����������� ������� �� �������������� ����� � ���������

        s[i] = index_of[s[i]];

    }

    // ��������� ������

    //-------------------------------------------------------------------------------------------------

    if (syn_error)     // ���� ���� ������, �������� �� ���������������
    {

        // ���������� �������� �������� ������

        //-----------------------------------------------------------------------------------------------

        // ��������� ������� �������� ������ ����� ����������� �������� ����������. ������ ������������

       // Lin and Costello (��. "Error Control Coding: Fundamentals and Applications" Prentice Hall 1983

       // ISBN 013283796) d[u] ������������ ����� m (����), ���������� ����������� (discrepancy),

       // ��� u = m + 1 � m ���� ����� ���� �� ��������� �� �1 �� 2t. � �������� �� �� ����� ��������

       // ������������ D(x) (�������) � ���������� ��������. l[u] ������������ ����� ������� elp

       // ��� ������� ���� ��������, u_l[u] ������������ ����� ������� ����� ������� ���� � �������� elp,

        // �������������� �������� �������

        d[0] = 0;        // ��������� �����

        d[1] = s[1];    // ��������� �����

        elp[0][0] = 0;  // ��������� �����

        elp[1][0] = 1;  // �������������� �����   

        for (i = 1; i < n - k; i++)
        {

            elp[0][i] = -1; // ��������� �����

            elp[1][i] = 0;  // �������������� �����

        }

        l[0] = 0; l[1] = 0; u_lu[0] = -1; u_lu[1] = 0; u = 0;

        do
        {

            u++;

            if (d[u] == -1)
            {

                l[u + 1] = l[u];

                for (i = 0; i <= l[u]; i++)
                {

                    elp[u + 1][i] = elp[u][i];

                    elp[u][i] = index_of[elp[u][i]];

                }

            }
            else
            {

                // ����� ���� � ���������� u_lu[q], ����� ��� d[q]!=0

                q = u - 1;

                while ((d[q] == -1) && (q > 0)) q--;



                // ������ ������ ��������� d[q]

                if (q > 0)
                {

                    j = q;

                    do
                    {

                        j--;

                        if ((d[j] != -1) && (u_lu[q] < u_lu[j]))

                            q = j;

                    } while (j > 0);

                };

                // ��� ������ �� ������ q, ����� ��� d[u]!=0 � u_lu[q] ���� ��������

                // ������� ������� ������ elp ��������

                if (l[u] > l[q] + u - q) l[u + 1] = l[u]; else l[u + 1] = l[q] + u - q;

                // ��������� ����� elp(x)

                for (i = 0; i < n - k; i++) elp[u + 1][i] = 0;

                for (i = 0; i <= l[q]; i++)

                    if (elp[q][i] != -1)

                        elp[u + 1][i + u - q] = alpha_to[(d[u] + n - d[q] + elp[q][i]) % n];

                for (i = 0; i <= l[u]; i++)
                {

                    elp[u + 1][i] ^= elp[u][i];

                    // ����������� ������ elp � ��������� �����

                    elp[u][i] = index_of[elp[u][i]];

                }

            }

            u_lu[u + 1] = u - l[u + 1];

            // ��������� (u + 1) �������

            //------------------------------------------------------------------------------------------------

            if (u < n - k)  // �� ��������� �������� ����������� �� ���� ����������
            {

                if (s[u + 1] != -1) d[u + 1] = alpha_to[s[u + 1]]; else d[u + 1] = 0;

                for (i = 1; i <= l[u + 1]; i++)

                    if ((s[u + 1 - i] != -1) && (elp[u + 1][i] != 0))

                        d[u + 1] ^= alpha_to[(s[u + 1 - i] + index_of[elp[u + 1][i]]) % n];

                // ��������� d[u+1] � ��������� �����

                d[u + 1] = index_of[d[u + 1]];

            }

        } while ((u < n - k) && (l[u + 1] <= t));

        // ������ �������� ��������

        //-----------------------------------------------------

        u++;

        if (l[u] <= t)
        {  // ��������� ������ ��������



          // ��������� elp � ��������� �����

            for (i = 0; i <= l[u]; i++) elp[u][i] = index_of[elp[u][i]];

            // ���������� ������ �������� �������� ������

            //----------------------------------------------

            for (i = 1; i <= l[u]; i++) reg[i] = elp[u][i]; count = 0;

            for (i = 1; i <= n; i++)
            {

                q = 1;

                for (j = 1; j <= l[u]; j++)

                    if (reg[j] != -1)
                    {

                        reg[j] = (reg[j] + j) % n;

                        q ^= alpha_to[reg[j]];

                    }

                if (!q)
                {  // ���������� ������ � ������ ������� ������

                    root[count] = i;

                    loc[count] = n - i;

                    count++;

                }

            }

            if (count == l[u])
            {       // ��� ������ � ������� elp < t ������   

               // ��������� ������� z(x)

                for (i = 1; i <= l[u]; i++) // Z[0] ������ ����� 1
                {

                    if ((s[i] != -1) && (elp[u][i] != -1))

                        z[i] = alpha_to[s[i]] ^ alpha_to[elp[u][i]];

                    else

                        if ((s[i] != -1) && (elp[u][i] == -1))

                            z[i] = alpha_to[s[i]];

                        else

                            if ((s[i] == -1) && (elp[u][i] != -1))

                                z[i] = alpha_to[elp[u][i]];

                            else

                                z[i] = 0;

                    for (j = 1; j < i; j++)

                        if ((s[j] != -1) && (elp[u][i - j] != -1))

                            z[i] ^= alpha_to[(elp[u][i - j] + s[j]) % n];

                    // ��������� z[i] � ��������� �����

                    z[i] = index_of[z[i]];

                }

                // ���������� �������� ������ � �������� loc[i]

                //----------------------------------------------------------------------------------------------

                for (i = 0; i < n; i++)
                {

                    err[i] = 0;

                    // ��������� recd[] � �������������� �����

                    if (recd[i] != -1) recd[i] = alpha_to[recd[i]]; else recd[i] = 0;

                }

                // ������� ��������� ��������� ������

                for (i = 0; i < l[u]; i++)
                {

                    err[loc[i]] = 1;

                    for (j = 1; j <= l[u]; j++)

                        if (z[j] != -1)

                            err[loc[i]] ^= alpha_to[(z[j] + j * root[i]) % n];

                    if (err[loc[i]] != 0)
                    {

                        err[loc[i]] = index_of[err[loc[i]]];

                        q = 0; // ��������� ����������� ������������ ������

                        for (j = 0; j < l[u]; j++)

                            if (j != i)

                                q += index_of[1 ^ alpha_to[(loc[j] + root[i]) % n]];

                        q = q % n; err[loc[i]] = alpha_to[(err[loc[i]] - q + n) % n];

                        // recd[i] ������ ���� � �������������� �����

                        recd[loc[i]] ^= err[loc[i]];

                    }

                }

            }
            else // ��� ������, ������� ������� ��������� ����������, �.�. ������� elp >= t
            {

                // ��������� recd[] � �������������� �����

                for (i = 0; i < n; i++)

                    if (recd[i] != -1) recd[i] = alpha_to[recd[i]];

                    else

                        recd[i] = 0;  // ������� �������������� ����� ��� ����

            }
        }
        else            // ������� elp > t, ������� ����������
        {

            // ��������� recd[] � �������������� �����

            for (i = 0; i < n; i++)

                if (recd[i] != -1)

                    recd[i] = alpha_to[recd[i]];

                else

                    recd[i] = 0; // ������� �������������� ����� ��� ����

        }
    }
    else { // ������ �� ����������
        for (i = 0; i < n; i++) if (recd[i] != -1)recd[i] = alpha_to[recd[i]]; else recd[i] = 0;

    }
    delete[] s;
    for (size_t i = 0; i < n - k + 2; i++)
    {
        delete[] elp[i];
    }
    delete[] elp;
    delete[] d;
    delete[] l;
    delete[] u_lu;
    delete[] root;
    delete[] loc;
    delete[] z;
    delete[] err;
    delete[] reg;

    return recd;
}
