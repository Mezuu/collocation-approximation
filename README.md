# collocation-approximation

> _Repository_ GitHub dapat diakses pada [https://github.com/Mezuu/collocation-approximation](https://github.com/Mezuu/collocation-approximation)

> _Project_ OnlineGDB dapat diakses pada [https://onlinegdb.com/jHgCgHbXP](https://onlinegdb.com/jHgCgHbXP)

Sebuah program yang terdiri dari 3 fitur utama untuk melakukan aproksimasi polinomial kolokasi, turunan, dan integral. Dibuat dengan tujuan memenuhi tugas akhir mata kuliah Metode Numerik.

---

## Instalasi

Instalasi dapat dilakukan dengan melakukan kompilasi _source code_ menggunakan program _C compiler_ seperti [gcc](https://gcc.gnu.org)

```
gcc app.c -o app && app
```

## Penggunaan

Program ini terdiri atas 3 fitur utama :

- Aproksimasi polinomial kolokasi
- Aproksimasi turunan
- Aproksimasi integral

### Aproksimasi polinomial kolokasi

Digunakan untuk mengaproksimasi sebuah polinomial jika diketahui beberapa titik kolokasi, proses dilakukan dengan membangun sebuah polinomial kolokasi terlebih dahulu kemudian melakukan aproksimasi menggunakan polinomial yang telah dibangun tadi.

#### Contoh input

Diketahui titik - titik kolokasi sebuah fungsi yang tidak diketahui sebagai berikut :

| x   | y   |
| --- | --- |
| -1  | -8  |
| 1   | -2  |
| 2   | 19  |
| -2  | -5  |

Carilah nilai p(0) dengan menggunakan aproksimasi polinomial.

#### Contoh output

![output1](https://i.ibb.co/HBKmLhv/image-2022-07-02-204911280.png)

Hasil polinomial kolokasi : p(x) = -8 + 3(x+1) + 6(x+1)(x-1) + (x+1)(x-1)(x-2)

Hasil aproksimasi : **p(0) = -9**

### Aproksimasi turunan

Digunakan untuk mengaproksimasi nilai turunan sebuah fungsi yang tidak diketahui jika diketahui beberapa titik kolokasinya, terdapat 2 kemungkinan alternatif penyelesaian, yaitu dengan menggunakan formula aproksimasi atau melalui polinomial kolokasi.

Beberapa metode yang mungkin digunakan :

- Formula titik terpusat
- Formula 3 titik dengan epsilon berada di antara x<sub>0</sub> dan x<sub>0</sub> + 2h
- Formula 5 titik dengan epsilon berada di antara x<sub>0</sub> dan x<sub>0</sub> + 4h
- Formula 5 titik dengan epsilon berada di antara x<sub>0</sub> - 2h dan x<sub>0</sub> + 2h
- Integral melalui aproksimasi polinomial kolokasi

#### Contoh input

Diketahui titik - titik kolokasi sebuah fungsi yang tidak diketahui sebagai berikut :

| x   | y         |
| --- | --------- |
| 1.8 | 10.889365 |
| 1.9 | 12.703199 |
| 2.0 | 14.778112 |
| 2.1 | 17.148957 |
| 2.2 | 19.855030 |

Carilah nilai turunan fungsi tersebut di titik x = 2.0

#### Contoh output

![output2](https://i.ibb.co/nLgGzFg/image-2022-07-02-205646918.png)

Hasil aproksimasi : **p'(2.0) = 22.167**

### Aproksimasi integral

Digunakan untuk mengaproksimasi nilai integral sebuah fungsi yang tidak diketahui jika diketahui beberapa titik kolokasinya, terdapat 2 kemungkinan alternatif penyelesaian, yaitu dengan menggunakan metode Simpson atau melalui polinomial kolokasi.

#### Contoh input

Diketahui titik - titik kolokasi sebuah fungsi yang tidak diketahui sebagai berikut :

| x   | y     |
| --- | ----- |
| 0   | 1     |
| 1   | 2.718 |
| 2   | 7.389 |

Carilah nilai integral fungsi tersebut pada rentang [0,2]

#### Contoh output

![output3](https://i.ibb.co/vqxsSLM/image-2022-07-02-210346015.png)

Hasil aproksimasi : nilai integral tentu fungsi tersebut pada [0,2] adalah **6.42**
