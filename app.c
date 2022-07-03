#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
    newtonPolynomial
    Fungsi untuk mencari hasil dari polinomial Newton jika diketahui array a, array c, dan x.
    aSize adalah banyaknya elemen dari array a.
    Keluaran adalah berupa nilai y atau p(x).

    Bentuk polinomial Newton :
    y = a0 + a1(x - c1) + a2(x - c1)(x - c2) + a3(x - c1)(x - c2)(x - c3) + ...
*/

float newtonPolynomial(float a[], float c[], float x, int aSize)
{
    if (aSize == 1)
        return a[0];
    float result = a[0];
    float temp = 1;
    for (int i = 0; i < aSize - 1; i++)
    {
        temp *= x - c[i];
        result += a[i + 1] * temp;
    }
    return result;
}

/*
    collocationPolynomial
    Fungsi untuk membangun polinomial kolokasi jika diketahui titik-titik kolokasinya.
    points adalah array 1 dimensi dimana 1 titik kolokasi (x dan y) menempati 2 elemen pada array.
    Contoh titik kolokasi : (-1,-8), (1,-2), (2,19), (-2,-5); maka elemen array points : [-1,-8,1,-2,2,19,-2,-5]
    n adalah banyaknya titik kolokasi.
    Keluaran adalah berupa array dari pointer (lihat bagian bawah).
*/

float **collocationPolynomial(float points[], int n, int nRef)
{
    if (n < 2)
    {
        printf("[ERROR] Titik kolokasi harus > 1");
        return 0;
    }
    static float a[256];
    static float c[256];
    for (int i = 0; i < n; i++)
    {
        c[i] = points[i * 2];
    }
    a[0] = points[1];
    for (int i = 1; i < nRef; i++)
    {
        float temp = 1;
        for (int j = 0; j < i; j++)
        {
            temp *= points[i * 2] - c[j];
        }
        a[i] = (points[i * 2 + 1] - newtonPolynomial(a, c, points[i * 2], i + 1)) / temp;
    }

    static float *p[] = {a, c};
    return p;

    /*
        Return adalah berupa array pointer yang menunjuk ke pointer lain, p[0] menunjuk ke a dan p[1] menunjuk ke c
        Contoh kasus diketahui titik kolokasi : (-1,-8), (1,-2), (2,19), (-2,-5)
        Hasil pembangunan polinomial kolokasi : y = -8 + 3(x+1) + 6(x+1)(x-1) + (x+1)(x-1)(x-2)
        p[0][0] = -8;   p[0][1] = 3;
        p[1][0] = -1;   p[1][1] = 1;
    */
}

/*
    differentiate
    Fungsi untuk mencari nilai turunan pada titik x sebuah fungsi menggunakan jika diketahui array a, array c, x, dan delta.
*/

float differentiate(float a[], float c[], float x, float h, int n)
{
    float result;
    result = (newtonPolynomial(a, c, x + h, n) - newtonPolynomial(a, c, x, n)) / h;
    return result;
}

/*
    integral
    Fungsi untuk mencari nilai integral sebuah fungsi menggunakan metode integral Riemann jika diketahui array a, array c, batas bawah,
    batas atas, dan delta.
*/

float integral(float a[], float c[], float b1, float b2, float h, int n)
{
    float result;
    int iter = (b2 - b1) / h;
    if (iter > 100)
    {
        printf("[ERROR] Iterasi lebih dari 100 kali!");
        return 0;
    }
    result = 0;
    for (int i = 0; i < iter; i++)
    {
        result += h * newtonPolynomial(a, c, b1 + i * h / 2, n);
    }
    return result;
}

/*
    arrayMin
    Fungsi untuk mencari indeks elemen terkecil pada sebuah array.
*/

int arrayMin(float array[], int n, int fromIndex)
{
    int minIndex = fromIndex;
    for (int i = fromIndex; i < n; i++)
    {
        if (array[i] < array[minIndex])
            minIndex = i;
    }
    return minIndex;
}

/*
    sort
    Fungsi untuk mengurutkan array dari titik kolokasi.
    Masukan adalah berupa array 1 dimensi yaitu titik - titik kolokasi [x1,y1,x2,y2,x3,y3,...]
    Keluaran adalah array titik kolokasi yang sudah diurutkan berdasarkan nilai x nya.
    Contoh input : [-1,-8,1,-2,2,19,-2,-5]
    Contoh output : [-2,-5,-1,-8,1,-2,2,19]
*/

float *sort(float points[], int n)
{
    float x[256];
    for (int i = 0; i < n; i++)
    {
        x[i] = points[i * 2];
    }

    int minXIndex;
    float temp;
    for (int i = 0; i < n; i++)
    {
        minXIndex = arrayMin(x, n, i);
        temp = x[i];
        x[i] = x[minXIndex];
        x[minXIndex] = temp;
        temp = points[i * 2];
        points[i * 2] = points[minXIndex * 2];
        points[minXIndex * 2] = temp;
        temp = points[i * 2 + 1];
        points[i * 2 + 1] = points[minXIndex * 2 + 1];
        points[minXIndex * 2 + 1] = temp;
    }

    return points;
}

/*
    xPos
    Fungsi untuk mencari letak nilai xa di antara titik - titik kolokasi yang diberikan dengan asumsi array points sudah disort.
    Masukan adalah berupa array 1 dimensi yaitu titik - titik kolokasi yang sudah disort berdasarkan nilai x nya;
*/

int xPos(float points[], float xa, int n)
{
    int pos = 0;

    for (int i = 0; i < n; i++)
    {
        if (xa + 0.001 >= points[i * 2])
        {
            pos = i + 1;
        }
    }

    return pos;
}

/*
    getYFromX
    Fungsi untuk mendapatkan nilai y dari nilai x pada titik - titik kolokasi yang diberikan.
    Parameter glob digunakan untuk memberikan sinyal jika nilai x pada titik kolokasi tidak ditemukan sehingga
    tidak dapat melanjutkan dengan menggunakan formula.
*/

float getYFromX(float points[], float x, int n, int *glob)
{
    for (int i = 0; i < n; i++)
    {
        if (fabsf(points[i * 2] - x) < 0.001)
            return points[i * 2 + 1];
    }
    printf("\n[WARNING] Nilai p(%.3f) tidak ditemukan pada titik kolokasi yang diberikan", x);
    *glob = -1;
    return -999;
}

/*
    getDifferentiatePoints
    Fungsi untuk mendapatkan titik - titik yang dibutuhkan sesuai dengan metode aproksimasi turunan yang paling tepat digunakan.
    differentiateFormula digunakan untuk memberikan sinyal formula aproksimasi turunan mana yang akan digunakan.
    Keluaran adalah berupa array titik - titik yang akan digunakan dalam formula aproksimasi.
*/

float *getDifferentiatePoints(float points[], float x, int n, float h, int nRef, int *differentiateFormula, int *glob)
{
    static float usedPoints[10];
    int xpos = xPos(points, x, n);

    /*
        differentiateFormula adalah berupa nilai kategori dari formula yang akan digunakan :
            0 = formula selisih terpusat
            1 = formula 3 titik
            2 = formula 5 titik tipe 1
            3 = formula 5 titik tipe 2
            -1 = formula tidak bisa digunakan, gunakan alternatif dengan membangun polinomial kolokasi
    */

    if (nRef != 2 && nRef != 3 && nRef != 5)
    {
        printf("[WARNING] Titik referensi yang bisa digunakan hanya 2, 3, atau 5, Anda memasukkan %d", nRef);
    }
    else if ((xpos == 0 || xpos >= n) && (nRef != 3 && nRef != 5))
    {
        printf("[WARNING] Titik yang dicari (%.3f) kurang dari titik kolokasi minimum (%.3f) atau lebih dari sama dengan titik kolokasi maksimum (%.3f)", x, points[0], points[2 * (n - 1)]);
    }

    /*
         Jika banyak titik referensi yang digunakan adalah 2, atau jika 3 namun epsilon jatuh di antara x0-h dan x0+h,
         maka gunakan formula selisih terpusat
    */

    else if (nRef == 2 || (nRef == 3 && (xpos > n - 2)))
    {
        float yo = getYFromX(points, x, n, glob);
        if (*glob != -1)
        {
            *differentiateFormula = 0;
            usedPoints[0] = x - h;
            usedPoints[1] = getYFromX(points, x - h, n, glob);
            usedPoints[2] = x + h;
            usedPoints[3] = getYFromX(points, x + h, n, glob);
        }
    }

    /*
        Jika banyak titik referensi yang digunakan adalah 3 dan epsilon jatuh di antara x0 dan x0+2h
    */

    else if (nRef == 3)
    {
        float yo = getYFromX(points, x, n, glob);
        if (*glob != -1)
        {
            *differentiateFormula = 1;
            usedPoints[0] = x;
            usedPoints[1] = yo;
            usedPoints[2] = x + h;
            usedPoints[3] = getYFromX(points, x + h, n, glob);
            usedPoints[4] = x + 2 * h;
            usedPoints[5] = getYFromX(points, x + 2 * h, n, glob);
        }
    }

    /*
         Jika banyak titik referensi yang digunakan adalah 5, namun tidak terdapat cukup titik kolokasi yang dapat dipakai dalam formula.
    */

    else if (nRef == 5 && xpos > n - 2)
    {
        printf("[WARNING] Terdapat %d titik kolokasi, xo terdapat pada index ke-%d sedangkan formula membutuhkan 5 buah titik referensi", n, xpos);
    }

    /*
         Jika banyak titik referensi yang digunakan adalah 5 dan epsilon jatuh pada x0 dan x0+4h
    */

    else if (nRef == 5 && xpos < 2)
    {
        float yo = getYFromX(points, x, n, glob);
        if (*glob != -1)
        {
            *differentiateFormula = 2;
            usedPoints[0] = x;
            usedPoints[1] = yo;
            usedPoints[2] = x + h;
            usedPoints[3] = getYFromX(points, x + h, n, glob);
            usedPoints[4] = x + 2 * h;
            usedPoints[5] = getYFromX(points, x + 2 * h, n, glob);
            usedPoints[6] = x + 3 * h;
            usedPoints[7] = getYFromX(points, x + 3 * h, n, glob);
            usedPoints[8] = x + 4 * h;
            usedPoints[9] = getYFromX(points, x + 4 * h, n, glob);
        }
    }

    /*
         Jika banyak titik referensi yang digunakan adalah 5 dan epsilon jatuh pada x0-2h dan x0+2h
    */

    else
    {
        float yo = getYFromX(points, x, n, glob);
        if (*glob != -1)
        {
            *differentiateFormula = 3;
            usedPoints[0] = x - 2 * h;
            usedPoints[1] = getYFromX(points, x - 2 * h, n, glob);
            usedPoints[2] = x - h;
            usedPoints[3] = getYFromX(points, x - h, n, glob);
            usedPoints[4] = x + h;
            usedPoints[5] = getYFromX(points, x + h, n, glob);
            usedPoints[6] = x + 2 * h;
            usedPoints[7] = getYFromX(points, x + 2 * h, n, glob);
        }
    }

    return usedPoints;
}

float centralPointApproximation(float usedPoints[], float h)
{
    float result;
    result = (usedPoints[3] - usedPoints[1]) / (2 * h);
    return result;
}

float threePointsApproximation(float usedPoints[], float h)
{
    float result;
    result = ((-3) * usedPoints[1] + 4 * usedPoints[3] - usedPoints[5]) / (2 * h);
    return result;
}

/*
    FivePointsApproximationA
    Metode aproksimasi turunan bentuk A digunakan jika epsilon jatuh pada x0 dan x0+4h
*/

float fivePointsApproximationA(float usedPoints[], float h)
{
    float result;
    result = ((-25) * usedPoints[1] + 48 * usedPoints[3] - 36 * usedPoints[5] + 16 * usedPoints[7] - 3 * usedPoints[9]) / (12 * h);
    return result;
}

/*
    FivePointsApproximationB
    Metode aproksimasi turunan bentuk B digunakan jika epsilon jatuh pada x0-2h dan x0+2h
*/

float fivePointsApproximationB(float usedPoints[], float h)
{
    float result;
    result = (usedPoints[1] - 8 * usedPoints[3] + 8 * usedPoints[5] - usedPoints[7]) / (12 * h);
    return result;
}

/*
    getIntegralPoints
    Fungsi untuk mendapatkan titik - titik yang dibutuhkan sesuai dengan metode aproksimasi integral Simpson.
    Keluaran adalah berupa array titik - titik yang akan digunakan dalam formula aproksimasi.
*/

float *getIntegralPoints(float points[], int n, float h, int *glob)
{
    *glob = 0;
    static float usedPoints[6];
    float x = (points[0] + points[2 * (n - 1)]) / 2;
    int xpos = xPos(points, x, n);

    float yo = getYFromX(points, x, n, glob);
    if (*glob != -1)
    {
        usedPoints[0] = x - h;
        usedPoints[1] = getYFromX(points, x - h, n, glob);
        usedPoints[2] = x;
        usedPoints[3] = yo;
        usedPoints[4] = x + h;
        usedPoints[5] = getYFromX(points, x + h, n, glob);
    }

    return usedPoints;
}

float simpsonApproximation(float usedPoints[])
{
    float result;
    result = (usedPoints[1] + 4 * usedPoints[3] + usedPoints[5]) * (usedPoints[4] - usedPoints[0]) / 6;
    return result;
}

void printPolynomial(float a[], float c[], int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("(%.3f)", a[i]);
        for (int j = 0; j < i; j++)
        {
            printf("(x-(%.3f))", c[j]);
        }
        if (i != n - 1)
            printf(" + ");
    }
}

int main()
{
    int glob = 0;                  // Mengindikasikan bisa tidaknya menggunakan formula aproksimasi, -1 = tidak bisa
    int differentiateFormula = -1; // Mengindikasikan formula aproksimasi turunan yang digunakan, lihat bagian getDifferentiatePoints
    char dummy[1];                 // Untuk keperluan freeze screen setelah menampilkan hasil

    int nPoints;
    float points[256];
    printf("Masukkan banyaknya titik kolokasi (min 2, max 128) : ");
    scanf("%d", &nPoints);
    for (int i = 0; i < 2 * nPoints; i++)
    {
        printf("Masukkan titik (%d, %d) : ", (i / 2) + 1, (i % 2) + 1);
        scanf("%f", &points[i]);
    }

    int select = -1;

    while (select != 0)
    {
        system("cls");
        printf("\nTitik kolokasi :\n");
        for (int i = 0; i < nPoints; i++)
        {
            printf("(%.3f, %.3f), ", points[i * 2], points[i * 2 + 1]);
        }

        printf("\nMenu\n==========\n1. Polinomian kolokasi\n2. Turunan\n3. Integral\n\n0. Keluar\n\nMasukkan pilihan : ");
        scanf("%d", &select);

        int nRef;
        float x, h;

        if (select == 1)
        {
            printf("\nPolinomial Kolokasi\n==========\nMasukkan x yang ingin dicari : ");
            scanf("%f", &x);
            printf("Masukkan banyaknya titik referensi yang ingin digunakan : ");
            scanf("%d", &nRef);
            float **res = collocationPolynomial(points, nPoints, nRef);
            printf("\nAproksimasi polinomial kolokasi\n==========\nTitik yang digunakan : %d\nHasil polinomial : p(x) = ", nRef);
            printPolynomial(res[0], res[1], nRef);
            printf("\np(%.3f) = %.3f", x, newtonPolynomial(res[0], res[1], x, nPoints));
        }
        else if (select == 2)
        {
            printf("\nAproksimasi turunan\n==========\nMasukkan x yang ingin diaproksimasi : ");
            scanf("%f", &x);
            printf("Masukkan banyaknya titik referensi yang ingin digunakan : ");
            scanf("%d", &nRef);
            printf("Masukkan nilai h : ");
            scanf("%f", &h);
            float *sortedPoints = sort(points, nPoints);
            float *usedPoints = getDifferentiatePoints(sortedPoints, x, nPoints, h, nRef, &differentiateFormula, &glob);
            if (glob == -1 || differentiateFormula == -1)
            {
                printf("\n[WARNING] Tidak dapat melanjutkan dengan menggunakan formula aproksimasi, beralih dengan cara membangun polinomial kolokasi...");
                float **res = collocationPolynomial(sortedPoints, nPoints, nPoints);
                printf("\nHasil aproksimasi : p'(x) = %.3f", differentiate(res[0], res[1], x, h, nPoints));
            }
            else
            {
                float res;
                if (differentiateFormula == 0)
                {
                    printf("\nGunakan formula titik terpusat");
                    res = centralPointApproximation(usedPoints, h);
                }
                else if (differentiateFormula == 1)
                {
                    printf("\nGunakan formula 3 titik");
                    res = threePointsApproximation(usedPoints, h);
                }
                else if (differentiateFormula == 2)
                {
                    printf("\nGunakan formula 5 titik tipe A");
                    res = fivePointsApproximationA(usedPoints, h);
                }
                else if (differentiateFormula == 3)
                {
                    printf("\nGunakan formula 5 titik tipe B");
                    res = fivePointsApproximationB(usedPoints, h);
                }
                printf("\nHasil aproksimasi : p'(x) = %.3f", res);
            }
        }
        else if (select == 3)
        {
            printf("\nAproksimasi integral\n==========\nMasukkan nilai h : ");
            scanf("%f", &h);
            float *sortedPoints = sort(points, nPoints);
            float *usedPoints = getIntegralPoints(sortedPoints, nPoints, h, &glob);
            if (glob == -1)
            {
                printf("\n[WARNING] Tidak dapat melanjutkan dengan menggunakan formula aproksimasi, beralih dengan cara membangun polinomial kolokasi...");
                float **res = collocationPolynomial(sortedPoints, nPoints, nPoints);
                printf("\nHasil aproksimasi : %.3f", integral(res[0], res[1], sortedPoints[0], sortedPoints[2 * (nPoints - 1)], h, nPoints));
            }
            else
            {
                printf("\nGunakan metode Simpson");
                printf("\nHasil aproksimasi : %.3f", simpsonApproximation(usedPoints));
            }
        }
        printf("\nMasukkan apapun untuk melanjutkan...");
        scanf("%s", dummy);
    }

    return 0;
}