#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#pragma warning(disable:4996)
#define X_MAX 128
#define Y_MAX 128
#define PI 3.141592
#define r 5

typedef struct Complex {
	double image;
	double real;
} Complex;


Complex IDEAL[X_MAX][X_MAX];
double H[X_MAX][X_MAX];

void ideal() {
	for (int i = 0; i < X_MAX; i++) {
		for (int j = 0; j < X_MAX; j++) {
			H[i][j] = exp(-1.0 * ((double)(X_MAX / 2 - (double)i)*(X_MAX / 2 - (double)i) + (double)(X_MAX / 2.0 - j)*(double)(X_MAX / 2.0 - j)) / (double)(2.0 * r*r));
			
		}
	}
}


int main() {
	FILE *in, *out;
	int n, m, v, u;
	unsigned char in_data[X_MAX][Y_MAX], out_data[X_MAX][Y_MAX], out_data2[X_MAX][Y_MAX], out_data3[X_MAX][Y_MAX];
	Complex a, sum;
	double multi = 1.0;

	printf("LPF in frequency domain - Gaussian\n");
	ideal();

	//256*256 image
	in = fopen("lena_ZoomOut2.img", "rb");
	if (in == NULL) {
		printf("file cannot open");
		return 0;
	}
	printf("success to open file\n");

	fread(in_data, sizeof(unsigned char), X_MAX * Y_MAX, in);
	fclose(in);

	//F(u,v)구하기
	for (u = 0; u < X_MAX; u++) {
		for (v = 0; v < X_MAX; v++) {
			sum.real = 0;
			sum.image = 0;
			//f(m,n)
			for (m = 0; m < X_MAX; m++) {
				for (n = 0; n < Y_MAX; n++) {
					multi = 1.0;
					if ((m + n) % 2) { // 센터로 이동
						multi = -1.0;
					}
					a.real = cos(2.0 * PI*(((double)(u*m) + (double)(v*n)) / (double)X_MAX));
					a.image = -1 * sin(2.0 * PI*(((double)(u*m) + (double)(v*n)) / (double)X_MAX));

					sum.real += a.real*(double)in_data[m][n] * multi;
					sum.image += a.image*(double)in_data[m][n] * multi;

				}
			}

			sum.real = sum.real / (X_MAX*X_MAX);
			sum.image = sum.image / (X_MAX*X_MAX);

			double temp;
			temp = sqrt(sum.real*sum.real + sum.image*sum.image);
			//IDFT할때는 이 값말고 크기로 저장해야함!!!!sqrt값인가 이거루!
			if (255.0*(log10(temp + 1.0) / log10(in_data[64][64] + 1.0)) > 255)
				out_data[u][v] = 255;
			else if (255.0*(log10(temp + 1.0) / log10(in_data[64][64] + 1.0)) < 0)
				out_data[u][v] = 0;
			else
				out_data[u][v] = 255.0*(log10(temp + 1.0) / log10(in_data[64][64] + 1.0));

			IDEAL[u][v].real = sum.real * H[u][v];
			IDEAL[u][v].image = sum.image * H[u][v];

			//printf("%d %d\n", IDEAL[u][v].real, IDEAL[u][v].image);

		}
	}

	out = fopen("lena_LPF_GAUSSIAN_FREQ.img", "wb");
	fwrite(out_data, sizeof(unsigned char), X_MAX*Y_MAX, out);
	fclose(out);
	printf("success to write LPF_DFT file\n");

	for (m = 0; m < X_MAX; m++) {
		for (n = 0; n < Y_MAX; n++) {
			sum.real = 0;
			sum.image = 0;
			//F(u,v)

			multi = 1.0;
			if ((m + n) % 2) { // 0,0으로 이동
				multi = -1.0;
			}

			for (u = 0; u < X_MAX; u++) {
				for (v = 0; v < Y_MAX; v++) {

					a.real = (float)cos(2.0 * PI / (float)X_MAX*((float)(u*m + v*n)));
					a.image = -1 * (float)sin(2.0 * PI / (float)X_MAX*((float)(u*m + v*n)));

					//b.real = (float)cos(PI*(float)(u+v));
					//b.image = (float)sin(PI*(float)(u + v));
					sum.real += a.real*((float)IDEAL[u][v].real*multi);
					sum.image += a.image*((float)IDEAL[u][v].image*multi);

				}
			}



			out_data2[m][n] = sum.real + sum.image;
			//out_data2[m][n] = sqrt(sum.real*sum.real + sum.image*sum.image);
			//out_data2[m][n] = SUM;
		}
	}
	out = fopen("lena_LPF_GAUSSIAN_SPATIAL.img", "wb");
	fwrite(out_data2, sizeof(unsigned char), X_MAX*Y_MAX, out);
	fclose(out);
	printf("success to write Inverse lena_LPF_GAUSSIAN_SPATIAL file\n");
}