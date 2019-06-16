#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#pragma warning(disable:4996)
#define X_MAX 256
#define Y_MAX 256
#define PI 3.141592


int main() {
	FILE *in, *out;
	int n, m, v, u, i, j;
	unsigned char in_data[X_MAX][Y_MAX],  out_data2[X_MAX][Y_MAX];
	int out_data[X_MAX][Y_MAX];
	in = fopen("lena_ZoomOut1.img", "rb");
	if (in == NULL) {
		printf("file cannot open");
		return;
	}
	printf("success to open file\n");

	fread(in_data, sizeof(unsigned char), X_MAX * Y_MAX, in);
	fclose(in);

	for (i = 0; i < X_MAX; i++) {
		for (j = 0; j < X_MAX; j++) {
			if (j == 0) {
				out_data[i][j] = in_data[i][j];
			}

			else {
				out_data[i][j] = in_data[i][j] - in_data[i][j - 1];
			}
		}
	}


	for (i = 0; i < X_MAX; i++) {
		for (j = 0; j < X_MAX; j++) {
			if (j == 0) {
				out_data2[i][j] = out_data[i][j];
			}

			else {
				out_data2[i][j] = out_data[i][j] + out_data2[i][j - 1];
			}
		}
	}


	out = fopen("lena_IDPCM.img", "wb");
	fwrite(out_data2, sizeof(unsigned char), X_MAX*Y_MAX, out);
	fclose(out);
	
	printf("success to write file\n");
}