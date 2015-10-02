/*
* Computation of the n'th decimal digit of pi with very little memory.
* Written by Fabrice Bellard on February 26, 1997.
*
* We use a slightly modified version of the method described by Simon
* Plouffe in "On the Computation of the n'th decimal digit of various
* transcendental numbers" (November 1996). We have modified the algorithm
* to get a running time of O(n^2) instead of O(n^3log(n)^3).
*
* This program uses a variation of the formula found by Gosper in 1974 :
*
* pi = sum( (25*n-3)/(binomial(3*n,n)*2^(n-1)), n=0..infinity);
*
* This program uses mostly integer arithmetic. It may be slow on some
* hardwares where integer multiplications and divisons must be done by
* software. We have supposed that 'int' has a size of at least 32 bits. If
* your compiler supports 'long long' integers of 64 bits, you may use the
* integer version of 'mul_mod' (see HAS_LONG_LONG).
*/

#include "pch.h"
#include "BenchCPU.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <ppltasks.h>
#include <collection.h>

using namespace UVEngineNative;
using namespace concurrency;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace std;

#define mul_mod(a,b,m) (( (long long) (a) * (long long) (b) ) % (m))
#define DIVN(t,a,v,vinc,kq,kqinc)		\
{						\
  kq+=kqinc;					\
  if (kq >= a) {				\
    do { kq-=a; } while (kq>=a);		\
    if (kq == 0) {				\
      do {					\
	t=t/a;					\
	v+=vinc;				\
	  	  	        } while ((t % a) == 0);			\
			    }						\
        }						\
}

inline int inv_mod(int x, int y)
{
	int q, u, v, a, c, t;

	u = x;
	v = y;
	c = 1;
	a = 0;
	do
	{
		q = v / u;

		t = c;
		c = a - q * c;
		a = t;

		t = u;
		u = v - q * u;
		v = t;
	} while (u != 0);
	a = a % y;
	if (a < 0)
		a = y + a;
	return a;
}

inline int inv_mod2(int u, int v)
{
	int u1, u3, v1, v3, t1, t3;

	u1 = 1;
	u3 = u;

	v1 = v;
	v3 = v;

	if ((u & 1) != 0)
	{
		t1 = 0;
		t3 = -v;
		goto Y4;
	}
	else
	{
		t1 = 1;
		t3 = u;
	}

	do
	{

		do
		{
			if ((t1 & 1) == 0)
			{
				t1 = t1 >> 1;
				t3 = t3 >> 1;
			}
			else
			{
				t1 = (t1 + v) >> 1;
				t3 = t3 >> 1;
			}
		Y4:;
		} while ((t3 & 1) == 0);

		if (t3 >= 0)
		{
			u1 = t1;
			u3 = t3;
		}
		else
		{
			v1 = v - t1;
			v3 = -t3;
		}
		t1 = u1 - v1;
		t3 = u3 - v3;
		if (t1 < 0)
		{
			t1 = t1 + v;
		}
	} while (t3 != 0);
	return u1;
}

inline int pow_mod(int a, int b, int m)
{
	int r, aa;

	r = 1;
	aa = a;
	while (1)
	{
		if (b & 1)
			r = mul_mod(r, aa, m);
		b = b >> 1;
		if (b == 0)
			break;
		aa = mul_mod(aa, aa, m);
	}
	return r;
}

inline int is_prime(int n)
{
	int r, i;
	if ((n % 2) == 0)
		return 0;

	r = (int) (sqrt((double) n));
	for (i = 3; i <= r; i += 2)
		if ((n % i) == 0)
			return 0;
	return 1;
}

inline int next_prime(int n)
{
	do
	{
		n++;
	} while (!is_prime(n));
	return n;
}

inline void calc_digit(int argc)
{
	int av, a, vmax, N, n, num, den, k, kq1, kq2, kq3, kq4, t, v, s, i, t1;
	double sum;

	n = argc;
	N = (int) ((n + 20) * log(10.0) / log(13.5));
	sum = 0;

	for (a = 2; a <= (3 * N); a = next_prime(a))
	{
		vmax = (int) (log(3 * (double) N) / log((double) a));
		if (a == 2)
		{
			vmax = vmax + (N - n);
			if (vmax <= 0)
				continue;
		}
		av = 1;
		for (i = 0; i < vmax; i++)
			av = av * a;

		s = 0;
		den = 1;
		kq1 = 0;
		kq2 = -1;
		kq3 = -3;
		kq4 = -2;
		if (a == 2)
		{
			num = 1;
			v = -n;
		}
		else
		{
			num = pow_mod(2, n, av);
			v = 0;
		}

		for (k = 1; k <= N; k++)
		{

			t = 2 * k;
			DIVN(t, a, v, -1, kq1, 2);
			num = mul_mod(num, t, av);

			t = 2 * k - 1;
			DIVN(t, a, v, -1, kq2, 2);
			num = mul_mod(num, t, av);

			t = 3 * (3 * k - 1);
			DIVN(t, a, v, 1, kq3, 9);
			den = mul_mod(den, t, av);

			t = (3 * k - 2);
			DIVN(t, a, v, 1, kq4, 3);
			if (a != 2)
				t = t * 2;
			else
				v++;
			den = mul_mod(den, t, av);

			if (v > 0)
			{
				if (a != 2)
					t = inv_mod2(den, av);
				else
					t = inv_mod(den, av);
				t = mul_mod(t, num, av);
				for (i = v; i < vmax; i++)
					t = mul_mod(t, a, av);
				t1 = (25 * k - 3);
				t = mul_mod(t, t1, av);
				s += t;
				if (s >= av)
					s -= av;
			}
		}

		t = pow_mod(5, n - 1, av);
		s = mul_mod(s, t, av);
		sum = fmod(sum + (double) s / (double) av, 1.0);
	}
}


IAsyncOperationWithProgress <double, double>^ BenchCPU::CalcPiSingle(int digits)
{
	return create_async([digits](progress_reporter<double> progress)->double
	{
		clock_t start, finish;
		double Digits = (double) digits;

		start = clock();
		for (int i = 0; i < digits; i++)
		{
			calc_digit(i * 15 + 1);
			progress.report(i / Digits * 100);
		}
		finish = clock();

		return (double) (finish - start) / CLOCKS_PER_SEC;
	});
}

IAsyncOperationWithProgress <double, double>^ BenchCPU::CalcPiMulti(int digits)
{
	return create_async([digits](progress_reporter<double> progress)->double
	{
		clock_t start, finish;
		double Digits = (double) digits;

		start = clock();
		parallel_for(0, digits, [progress, &Digits](int i)
		{
			calc_digit(i * 15 + 1);
			progress.report(i / Digits * 100);
		});
		finish = clock();

		return (double) (finish - start) / CLOCKS_PER_SEC;
	});
}