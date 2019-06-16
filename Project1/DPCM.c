// 이거는 freq domain에서 보여주기용

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
	unsigned char in_data[X_MAX][Y_MAX], out_data[X_MAX][Y_MAX];
	float multi = 1.0;
	//256*256 image
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
				if (in_data[i][j] - in_data[i][j - 1] < 0)
					out_data[i][j] = 0;
				else if (in_data[i][j] - in_data[i][j - 1] > 255)
					out_data[i][j] = 255;
				else 
				out_data[i][j] = in_data[i][j] - in_data[i][j - 1];
			}
		}
	}

	out = fopen("lena_DPCM.img", "wb");
	fwrite(out_data, sizeof(unsigned char), X_MAX*Y_MAX, out);
	fclose(out);
	printf("success to write file\n");
}