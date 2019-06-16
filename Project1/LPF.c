#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)
#define X_MAX 512
#define Y_MAX 512

int main() {
	FILE *in, *out;
	int i, j;
	unsigned char in_data[X_MAX][Y_MAX], out_data[X_MAX][Y_MAX];

	//in = fopen("lena_ZoomOut.img", "rb");
	in = fopen("lena.img", "rb");
	if (in == NULL) {
		printf("file cannot open");
		return;
	}
	printf("success to open file\n");

	fread(in_data, sizeof(unsigned char), X_MAX*Y_MAX, in);
	fclose(in);

	//low pass filtering
	for (i = 0; i < Y_MAX; i++) {
		for (j = 0; j < X_MAX; j++) {
			int sum = 0, cnt=0;
			for (int k = -1; k < 2; k++) {
				for (int l = -1; l < 2; l++) {
					if (i + k > 0 && i + k < X_MAX && j + l>0 && j + l < Y_MAX) {
						sum += in_data[i + k][j + l];
						cnt++;
					}
				}
			}
			out_data[i][j] = sum / cnt;
		}
	}

	out = fopen("lena_LPF2.img", "wb");
	fwrite(out_data, sizeof(unsigned char), X_MAX*Y_MAX, out);
	fclose(out);
	printf("success to write file\n");
}