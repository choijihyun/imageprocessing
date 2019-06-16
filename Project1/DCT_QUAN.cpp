#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#pragma warning(disable:4996)
#define X_MAX 512
#define Y_MAX 512
#define PI 3.141592
#define size 8

double DCT[X_MAX][X_MAX] = { 0.0 };
double SUM = 0.0;

int main() {
	FILE *in, *out;
	int n, m, v, u, i, j, x, y;
	unsigned char in_data[X_MAX][Y_MAX], out_data[X_MAX][Y_MAX], c[X_MAX][Y_MAX];
	double alpha, ALPHA;
	double COS, cu, cv, cnt1, cnt2;

	printf("lena dct quantization\n");
	in = fopen("lena.img", "rb");
	if (in == NULL) {
		printf("file cannot open");
		return 0;
	}
	printf("success to open file\n");

	fread(in_data, sizeof(unsigned char), X_MAX * Y_MAX, in);
	fclose(in);

	//F(u,v)구하기
	for (u = 0; u < X_MAX; u += size) {
		for (v = 0; v < X_MAX; v += size) {

			// F를 도는 포문
			for (m = 0; m < size; m++) {
				for (n = 0; n < size; n++) {

					SUM = 0.0;
					if (m == 0) alpha = (double)sqrt(1.0 / (double)size);
					else alpha = (double)sqrt(2.0 / (double)size);

					if (n == 0) ALPHA = (double)sqrt(1.0 / (double)size);
					else ALPHA = (double)sqrt(2.0 / (double)size);

					//f를 도는 포문
					for (x = 0; x < size; x++) {
						for (y = 0; y < size; y++) {
							COS = (double)cos((PI*(double)(2 * x + 1)*(m*1.0)) / (double)(2 * size)) * (double)cos((PI*(double)(2 * y + 1)*(n*1.0)) / (double)(2 * size));
							SUM += in_data[u + x][v + y] * COS;
						}
					}

					DCT[u + m][v + n] = round(SUM*alpha*ALPHA/80.0)*80.0;

				}
			}// mn forh

		}
	}

	for (i = 0; i < X_MAX; i++) {
		for (j = 0; j < X_MAX; j++) {
			if (DCT[i][j] < 0)
				out_data[i][j] = (unsigned char)((-1)*DCT[i][j]);
			else
				out_data[i][j] = (unsigned char)DCT[i][j];
		}
	}


	//f(m,n)구하기
	for (x = 0; x < X_MAX; x += size) {
		for (y = 0; y < X_MAX; y += size) {
			// f를 도는 포문
			for (m = 0; m < size; m++) {
				for (n = 0; n < size; n++) {
					SUM = 0.0;
					//F를 도는 포문
					for (u = 0; u < size; u++) {
						for (v = 0; v < size; v++) {
							if (u == 0) alpha = (double)sqrt(1.0 / (double)size);
							else alpha = (double)sqrt(2.0 / (double)size);

							if (v == 0) ALPHA = (double)sqrt(1.0 / (double)size);
							else ALPHA = (double)sqrt(2.0 / (double)size);

							COS = cos(PI*(2 * (double)m + 1.0)*((double)u*1.0) / (2.0 * size)) * cos(PI*(2 * (double)n + 1.0)*((double)v*1.0) / (2.0 * size));
							SUM += DCT[x + u][y + v] * COS*alpha*ALPHA;
						}
					}


					c[x + m][y + n] = (unsigned char)SUM;


				}
			}
		}
	}

	out = fopen("lena_DCT_quantization.img", "wb");
	fwrite(c, sizeof(unsigned char), X_MAX*Y_MAX, out);
	fclose(out);

	printf("success to write file\n");
}

