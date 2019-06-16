#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#pragma warning(disable:4996)
#define X_MAX 512
#define PI 3.141592
#define size 8


double DST_data = 0.0;
double DST_inv_data = 0.0;
double DST[512][512] = { 0.0 };

int main() {
	FILE *in, *out;
	int n, m, v, u, i, j, x, y;
	unsigned char **a, **out_data, **dst, **b;
	double multi = 1.0, alpha, ALPHA, SUM, SUM2;
	double COS, cu, cv, cnt1, cnt2;

	in = fopen("lena.img", "rb");
	if (in == NULL) {
		printf("file cannot open");
		return 0;
	}
	printf("success to open file\n");

	a = (unsigned char **)malloc(sizeof(unsigned char *)*X_MAX);
	for (i = 0; i < X_MAX; i++) {
		a[i] = (unsigned char *)malloc(sizeof(unsigned char)*X_MAX);
	}

	for (i = 0; i < X_MAX; i++) {
		fread(a[i], sizeof(unsigned char), X_MAX, in);
	}
	fclose(in);

	out_data = (unsigned char **)malloc(sizeof(unsigned char *)*X_MAX);
	dst = (unsigned char **)malloc(sizeof(unsigned char *)*X_MAX);
	b = (unsigned char **)malloc(sizeof(unsigned char *)*X_MAX);
	for (i = 0; i < X_MAX; i++) {
		out_data[i] = (unsigned char *)malloc(sizeof(unsigned char)*X_MAX);
		dst[i] = (unsigned char *)malloc(sizeof(unsigned char)*X_MAX);
		b[i] = (unsigned char *)malloc(sizeof(unsigned char)*X_MAX);
	}


	//F(u,v)구하기
	for (i = 0; i < (X_MAX/size); i++) {
		for (j = 0; j < (X_MAX/size); j++) {

			// F를 도는 포문
			for (u = 0; u < size; u++) {
				for (v = 0; v < size; v++) {

					DST_data = 0.0;
					//f를 도는 포문
					for (x = 0; x < size; x++) {
						for (y = 0; y < size; y++) {
							DST_data += a[(i*size) + x][(j*size) + y] * sin((PI*((double)x + 1.0)*((double)u + 1.0)) / (size + 1.0)) * sin((PI*((double)y + 1.0)*((double)v + 1.0)) / (1.0 + size));
						}
					}

					DST[(i*size) + u][(j*size) + v] = (2.0 / (size + 1.0))*DST_data;
				}
			}// mn forh

		}
	}

	for (i = 0; i < X_MAX; i++) {
		for (j = 0; j < X_MAX; j++) {
			if (DST[i][j] < 0) 
				b[i][j] = (unsigned char)((-1) * dst[i][j]);
			else 
				b[i][j] = (unsigned char)dst[i][j];
		}
	}

	for (i = 0; i < X_MAX/size; i++) {
		for (j = 0; j < X_MAX/size; j++) {

			// F를 도는 포문
			for (x = 0; x < size; x++) {
				for (y = 0; y < size; y++) {

					DST_inv_data = 0.0;
					//f를 도는 포문
					for (u = 0; u < size; u++) {
						for (v = 0; v < size; v++) {
							DST_inv_data += DST[(i*size) + u][(j*size) + v] * sin((PI*((double)x+1.0)*((double)u+1.0)) / (size + 1.0)) * sin((PI*((double)y+1.0)*((double)v+1.0)) / (1.0 + size));
						}
					}


					out_data[(i*size) + x][(j*size) + y] = (unsigned char)((2.0 / (size + 1.0))*DST_inv_data);

				}
			}// mn forh

		}
	}


	out = fopen("lena_DST_8_8.img", "wb");

	for (i = 0; i < X_MAX; i++) {
		fwrite(b[i], sizeof(unsigned char), X_MAX, out);
	}

	fclose(out);
	

	out = fopen("lena_IDST_8_8.img", "wb");
	for (i = 0; i < X_MAX; i++) {
		fwrite(out_data[i], sizeof(unsigned char), X_MAX, out);
	}

	fclose(out);

	printf("success to write file\n");
}

