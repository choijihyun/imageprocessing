#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#pragma warning(disable:4996)
#define Pi 3.141592
#define N1 512
#define N2 16

double DST[512][512] = { 0.0 };
double DST_inv[512][512] = { 0.0 };
double DST_data = 0.0;
double DST_inv_data = 0.0;


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
	fp = fopen("lena_DST_16x16.raw", "wb");

	unsigned char **b;
	unsigned char **c;

	b = (unsigned char**)malloc(sizeof(unsigned char*) * 512);
	c = (unsigned char**)malloc(sizeof(unsigned char*) * 512);

	for (int i = 0; i<512; i++)
	{
		b[i] = (unsigned char*)malloc(sizeof(unsigned char) * 512);
		c[i] = (unsigned char*)malloc(sizeof(unsigned char) * 512);
	}

	// DST
	for (int i = 0; i < (N1 / N2); i++)
	{
		for (int j = 0; j < (N1 / N2); j++)
		{
			for (int u = 0; u < N2; u++)
			{
				for (int v = 0; v < N2; v++)
				{
					DST_data = 0.0;

					for (int x = 0; x < N2; x++)
					{
						for (int y = 0; y < N2; y++)
						{
							DST_data += a[(i*N2) + x][(j*N2) + y] * sin((Pi * ((double)x + 1.0) * ((double)u + 1.0)) / (N2 + 1.0)) * sin((Pi * ((double)y + 1.0) * ((double)v + 1.0)) / (N2 + 1.0));
						}
					}

					DST[(i*N2) + u][(j*N2) + v] = (2.0 / (N2 + 1.0)) * DST_data;
				}
			}
		}
	}

	for (int i = 0; i < N1; i++)
	{
		for (int j = 0; j < N1; j++)
		{
			if (DST[i][j] < 0)
				b[i][j] = (unsigned char)((-1) * DST[i][j]);
			else
				b[i][j] = (unsigned char)DST[i][j];
		}
	}

	// DST_inverse
	for (int i = 0; i < (N1 / N2); i++)
	{
		for (int j = 0; j < (N1 / N2); j++)
		{
			for (int x = 0; x < N2; x++)
			{
				for (int y = 0; y < N2; y++)
				{
					DST_inv_data = 0.0;

					for (int u = 0; u < N2; u++)
					{
						for (int v = 0; v < N2; v++)
						{
							DST_inv_data += DST[(i*N2) + u][(j*N2) + v] * sin((Pi * ((double)x + 1.0) * ((double)u + 1.0)) / (N2 + 1.0)) * sin((Pi * ((double)y + 1.0) * ((double)v + 1.0)) / (N2 + 1.0));
						}
					}

					c[(i*N2) + x][(j*N2) + y] = (unsigned char)((2.0 / (N2 + 1.0)) * DST_inv_data);
				}
			}
		}
	}



	for (int i = 0; i<512; i++)
	{
		free(a[i]);
		fwrite(c[i], sizeof(unsigned char), 512, fp);
		free(b[i]);
		free(c[i]);
	}

	free(a);
	free(b);
	free(c);
	fclose(fp);
}