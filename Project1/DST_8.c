#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#pragma warning(disable:4996)
#define X_MAX 512
#define Y_MAX 512
#define PI 3.141592
#define size 8

int main() {
	FILE *in, *out;
	int n, m, v, u, i, j, x, y;
	unsigned char in_data[X_MAX][Y_MAX], out_data[X_MAX][Y_MAX], dct[X_MAX][Y_MAX];
	double multi = 1.0, alpha, ALPHA, SUM, max, min;
	double COS, cu, cv, cnt1, cnt2;
	in = fopen("lena.img", "rb");
	if (in == NULL) {
		printf("file cannot open");
		return;
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
					//f를 도는 포문
					for (x = 0; x < size; x++) {
						for (y = 0; y < size; y++) {
							COS = (double)sin((PI*(double)(x + 1)*(double)(m+1)) / (double)(size + 1)) * (double)sin((PI*(double)(y + 1)*(double)(n + 1)) / (double)(1+size));
							SUM += in_data[u + x][v + y] * COS;
						}
					}

					double temp;
					temp = SUM*2.0/(double)(size+1);
				
					//if(temp>255) out_data[u + m][v + n] = 255;
					//else if(temp<0)out_data[u + m][v + n] = 0;
					//else out_data[u + m][v + n] = temp;
					//

				/*	if (255.0*(log10(abs(temp) + 1.0) / log10(in_data[0][0] + 1.0)) > 255)
						out_data[u + m][v + n] = 255;
					else if (255.0*(log10(abs(temp) + 1.0) / log10(in_data[0][0] + 1.0)) < 0)
						out_data[u + m][v + n] = 0;
					else
						out_data[u + m][v + n] = 255.0*(log10(abs(temp) + 1.0) / log10(in_data[0][0] + 1.0));
					if (255.0*(log10((temp) + 1.0) / log10(in_data[0][0] + 1.0)) > 255)
						out_data[u + m][v + n] = 255;
					else if (255.0*(log10((temp) + 1.0) / log10(in_data[0][0] + 1.0)) < 0)
						out_data[u + m][v + n] = 0;
					else
						out_data[u + m][v + n] = 255.0*(log10((temp) + 1.0) / log10(in_data[0][0] + 1.0));
						*/
					//out_data[u*size+m][v*size+n] = SUM*alpha*ALPHA;

					out_data[u + m][v + n] = abs(temp); 

				}
			}// mn forh

		}
	}
/*
	for (i = 0; i < X_MAX; i++) {
		for (j = 0; j < X_MAX; j++) {
			out_data[i][j] = 1.0 / (max - min) * 255 * dct[i][j];
		}
	}


*/

	out = fopen("lena_DST_8.img", "wb");
	fwrite(out_data, sizeof(unsigned char), X_MAX*Y_MAX, out);
	fclose(out);
	printf("success to write file\n");
}

