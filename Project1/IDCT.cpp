// 이거는 freq domain에서 보여주기용

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
	unsigned char in_data[X_MAX][Y_MAX], out_data[X_MAX][Y_MAX], dct[X_MAX][X_MAX];
	float multi = 1.0, alpha, ALPHA, SUM;
	float COS, cu, cv, cnt1, cnt2;
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
					if (m == 0) alpha = (float)sqrt(1.0 / (float)size);
					else alpha = (float)sqrt(2.0 / (float)size);

					if (n == 0) ALPHA = (float)sqrt(1.0 / (float)size);
					else ALPHA = (float)sqrt(2.0 / (float)size);

					//f를 도는 포문
					for (x = 0; x < size; x++) {
						for (y = 0; y < size; y++) {
							COS = (float)cos(PI*(2 * (x)+1)*(m) / (float)(2 * size)) * (float)cos(PI*(2 * (y)+1)*(n) / (float)(2 * size));
							//printf("cos : %f\n", COS);
							
							//printf("in_data[%d][%d] = %d\n", u + x, v + y, in_data[u + x][v + y]);
							//printf("sum : %f\n", SUM);
							SUM += in_data[u + x][v + y] * COS;
						}
					}

					float temp;
					temp = SUM*alpha*ALPHA;

					dct[u + m][v + n] = temp;

				}
			}// mn forh
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
							if (u == 0) alpha = (float)sqrt(1.0 / (float)size);
							else alpha = (float)sqrt(2.0 / (float)size);

							if (v == 0) ALPHA = (float)sqrt(1.0 / (float)size);
							else ALPHA = (float)sqrt(2.0 / (float)size);

							COS = (float)cos(PI*(2 * m+1)*(u*1.0) / (float)(2 * size)) * (float)cos(PI*(2 * n+1)*(v*1.0) / (float)(2 * size));
							SUM += dct[x + u][y + v] * COS*alpha*ALPHA;
						}
					}

					if (255.0*(log10(SUM + 1.0) / log10(dct[X_MAX / 2][X_MAX / 2] + 1.0)) > 255)
						out_data[x + m][y + n] = 255;
					else if (255.0*(log10(SUM + 1.0) / log10(dct[X_MAX / 2][X_MAX / 2] + 1.0)) < 0)
						out_data[x + m][y + n] = 0;
					else
						out_data[x + m][y + n] = 255.0*(log10(SUM + 1.0) / log10(dct[X_MAX / 2][X_MAX / 2] + 1.0));

				}
			}
		}
	}


	out = fopen("lena_IDCT_8.img", "wb");
	fwrite(out_data, sizeof(unsigned char), X_MAX*Y_MAX, out);
	fclose(out);
	printf("success to write file\n");
}

