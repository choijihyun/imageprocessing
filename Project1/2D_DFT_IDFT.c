// 이거는 freq domain에서 보여주기용

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#pragma warning(disable:4996)
#define X_MAX 128
#define Y_MAX 128
#define PI 3.141592

typedef struct Complex {
	float image;
	float real;
} Complex;


int main() {
	FILE *in, *out, *out2;
	int n, m, v, u;
	unsigned char in_data[X_MAX][Y_MAX], out_data[X_MAX][Y_MAX], out_data2[X_MAX][Y_MAX];
	Complex dft[X_MAX][Y_MAX];
	Complex a, sum, b;
	//float cos, sin;
	float multi = 1.0;
	//256*256 image
	in = fopen("lena_ZoomOut2.img", "rb");
	if (in == NULL) {
		printf("file cannot open");
		return;
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
					a.real = (float)cos(2.0 * PI / (float)X_MAX*((float)(u*m + v*n)));
					a.image = -1 * (float)sin(2.0 * PI / (float)X_MAX*((float)(u*m + v*n)));

					sum.real += a.real*((float)in_data[m][n] * multi);
					sum.image += a.image*((float)in_data[m][n] * multi);
					//sum.real += a.real*(float)in_data[m][n];
					//sum.image += a.image*(float)in_data[m][n];
				}
			}

			sum.real = sum.real / (X_MAX*X_MAX);
			sum.image = sum.image / (X_MAX*X_MAX);

			float temp;
			temp = sqrt(sum.real*sum.real + sum.image*sum.image);
			//IDFT할때는 이 값말고 크기로 저장해야함!!!!sqrt값인가 이거루!
			if (255.0*(log10(temp + 1.0) / log10(in_data[64][64] + 1.0)) > 255)
				out_data[u][v] = 255;
			else if (255.0*(log10(temp + 1.0) / log10(in_data[64][64] + 1.0)) < 0)
				out_data[u][v] = 0;
			else
				out_data[u][v] = 255.0*(log10(temp + 1.0) / log10(in_data[64][64] + 1.0));

			dft[u][v].real = sum.real;
			dft[u][v].image = sum.image;
			//printf("dft : %f %f\n", dft[u][v].real, dft[u][v].real);
		}
		//printf("[%d ,%d]\n", u, v);
	}

	out = fopen("lena_DFT.img", "wb");
	fwrite(out_data, sizeof(unsigned char), X_MAX*Y_MAX, out);
	fclose(out);
	
	printf("success to write DFT display file\n");
	
	
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
					a.image = -1*(float)sin(2.0 * PI / (float)X_MAX*((float)(u*m + v*n)));

					//b.real = (float)cos(PI*(float)(u+v));
					//b.image = (float)sin(PI*(float)(u + v));
					sum.real += a.real*((float)dft[u][v].real*multi);
					sum.image += a.image*((float)dft[u][v].image*multi);

				}
			}



			out_data2[m][n] = sum.real + sum.image;
			//out_data2[m][n] = sqrt(sum.real*sum.real + sum.image*sum.image);
			//out_data2[m][n] = SUM;
		}
	}
	out2 = fopen("lena_IDFT.img", "wb");
	fwrite(out_data2, sizeof(unsigned char), X_MAX*Y_MAX, out2);
	fclose(out2);

	printf("success to write IDFT display file\n");

	

}