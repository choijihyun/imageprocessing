#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996)

#define X_MAX 512
#define Y_MAX 512

char horizontal[3][3];
char vertical[3][3];

void mask();

int main() {
	FILE *in, *out;
	int i, j;
	unsigned char in_data[X_MAX][Y_MAX];
	unsigned char out_data[X_MAX][Y_MAX];
	unsigned char out_data2[X_MAX][Y_MAX];


	in = fopen("lena.img", "rb");
	if (in == NULL) {
		printf("file cannot open");
		return;
	}
	printf("success to open file\n");
	
	fread(in_data, sizeof(unsigned char), X_MAX*Y_MAX, in);
	fclose(in);
	
	mask();

	//sobel horizontal
	for (i = 0; i < Y_MAX; i++) {
		for (j = 0; j < X_MAX; j++) {
			int sum = 0;
			for (int k = -1; k < 2; k++) {
				for (int l = -1; l < 2; l++) {
					if (i + k >= 0 && i + k < X_MAX && j + l>=0 && j + l < Y_MAX) {
						sum += (in_data[i + k][j + l] * horizontal[k + 1][l + 1]);
					}
				}
			}
			if (sum > 255)
				out_data[i][j] = 255;
			else if (sum < 0)
				out_data[i][j] = 0;
			else
				out_data[i][j] = sum;
		}
	}

	//sobel vertical
	for (i = 0; i < Y_MAX - 2; i++) {
		for (j = 0; j < X_MAX - 2; j++) {
			int sum = 0;
			for (int k = -1; k < 2; k++) {
				for (int l = -1; l < 2; l++) {
					if (i + k >= 0 && i + k < X_MAX && j + l>=0 && j + l < Y_MAX) {
						sum += in_data[i + k][j + l] * vertical[k + 1][l + 1];
					}
				}
			}
			if (sum  > 255)
				out_data2[i][j] = 255;
			else if (sum  < 0)
				out_data2[i][j] = 0;
			else
				out_data2[i][j] = sum;
		}
	}

	out = fopen("lena_HPF_horizontal.img", "wb");
	fwrite(out_data, sizeof(unsigned char), X_MAX*Y_MAX, out);

	out = fopen("lena_HPF_vertical.img", "wb");
	fwrite(out_data2, sizeof(unsigned char), X_MAX*Y_MAX, out);

	fclose(out);
	printf("success to write file\n");
}

void mask() {
	/*horizontal init*/
	horizontal[0][0] = 1;
	horizontal[0][1] = 2;
	horizontal[0][2] = 1;

	horizontal[1][0] = 0;
	horizontal[1][1] = 0;
	horizontal[1][2] = 0;

	horizontal[2][0] = -1;
	horizontal[2][1] = -2;
	horizontal[2][2] = -1;

	/*vertical init*/
	vertical[0][0] = 1;
	vertical[0][1] = 0;
	vertical[0][2] = -1;

	vertical[1][0] = 2;
	vertical[1][1] = 0;
	vertical[1][2] = -2;

	vertical[2][0] = 1;
	vertical[2][1] = 0;
	vertical[2][2] = -1;
}