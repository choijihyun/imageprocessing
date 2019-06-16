#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#pragma warning(disable: 4996)
#define Pi 3.141592
#define N1 512
#define N2 8

double DCT[512][512] = { 0.0 };
double DCT_inv[512][512] = { 0.0 };
double DCT_data = 0.0;
double DCT_inv_data = 0.0;

double alpha1 = 0.0;
double alpha2 = 0.0;


int main()
{
	// 원본 이미지
	FILE *fp = fopen("lena.img", "rb");

	unsigned char **a;
	a = (unsigned char**)malloc(sizeof(unsigned char*) * 512);

	for (int i = 0; i<512; i++)
	{
		a[i] = (unsigned char*)malloc(sizeof(unsigned char) * 512);
	}

	for (int i = 0; i<512; i++)
	{
		fread(a[i], sizeof(unsigned char), 512, fp);
	}

	fclose(fp);


	// 새로운 이미지 파일
	fp = fopen("lena_DCT_inverse.raw", "wb");

	unsigned char **b;
	unsigned char **c;

	b = (unsigned char**)malloc(sizeof(unsigned char*) * 512);
	c = (unsigned char**)malloc(sizeof(unsigned char*) * 512);

	for (int i = 0; i<512; i++)
	{
		b[i] = (unsigned char*)malloc(sizeof(unsigned char) * 512);
		c[i] = (unsigned char*)malloc(sizeof(unsigned char) * 512);
	}

	// DCT
	for (int i = 0; i < (N1 / N2); i++)
	{
		for (int j = 0; j < (N1 / N2); j++)
		{
			for (int u = 0; u < N2; u++)
			{
				for (int v = 0; v < N2; v++)
				{
					DCT_data = 0.0;

					for (int x = 0; x < N2; x++)
					{
						for (int y = 0; y < N2; y++)
						{
							DCT_data += a[(i*N2) + x][(j*N2) + y] * cos((Pi * (2 * (double)x + 1.0) * (double)u) / (2.0 * N2)) * cos((Pi * (2 * (double)y + 1.0) * (double)v) / (2.0 * N2));
						}
					}

					if (u == 0)
						alpha1 = 1.0 / sqrt((double)N2);
					else
						alpha1 = sqrt(2.0 / (double)N2);

					if (v == 0)
						alpha2 = 1.0 / sqrt((double)N2);
					else
						alpha2 = sqrt(2.0 / (double)N2);

					DCT[(i*N2) + u][(j*N2) + v] = alpha1 * alpha2 * DCT_data;
				}
			}
		}
	}

	for (int i = 0; i < N1; i++)
	{
		for (int j = 0; j < N1; j++)
		{
			if (DCT[i][j] < 0)
				b[i][j] = (unsigned char)((-1) * DCT[i][j]);
			else
				b[i][j] = (unsigned char)DCT[i][j];
		}
	}

	// DCT_inverse
	for (int i = 0; i < (N1 / N2); i++)
	{
		for (int j = 0; j < (N1 / N2); j++)
		{
			for (int x = 0; x < N2; x++)
			{
				for (int y = 0; y < N2; y++)
				{
					DCT_inv_data = 0.0;

					for (int u = 0; u < N2; u++)
					{
						for (int v = 0; v < N2; v++)
						{
							if (u == 0)
								alpha1 = 1.0 / sqrt((double)N2);
							else
								alpha1 = sqrt(2.0 / (double)N2);

							if (v == 0)
								alpha2 = 1.0 / sqrt((double)N2);
							else
								alpha2 = sqrt(2.0 / (double)N2);

							DCT_inv_data += (alpha1 * alpha2) * DCT[(i*N2) + u][(j*N2) + v] * cos((Pi * (2 * (double)x + 1.0) * (double)u) / (2.0 * N2)) * cos((Pi * (2 * (double)y + 1.0) * (double)v) / (2.0 * N2));
						}
					}
					c[(i*N2) + x][(j*N2) + y] = (unsigned char)DCT_inv_data;
				}
			}
		}
	}



	for (int i = 0; i<512; i++)
	{
		free(a[i]);
		fwrite(b[i], sizeof(unsigned char), 512, fp);
		free(b[i]);
		free(c[i]);
	}

	free(a);
	free(b);
	free(c);
	fclose(fp);
}