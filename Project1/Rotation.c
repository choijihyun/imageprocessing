#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#pragma warning(disable:4996)
#define X_MAX 256
#define Y_MAX 256
#define PI 3.141592

double getRadian(int theta) {
	return theta*(PI / 180);
}


int main() {
	FILE *in, *out;
	int i, j;
	unsigned char in_data[X_MAX / 2][Y_MAX / 2], out_data[X_MAX * 2][Y_MAX * 2],
		out_data2[X_MAX * 2][Y_MAX * 2], out_data3[X_MAX * 2][Y_MAX * 2];

	in = fopen("lena_ZoomOut2.img", "rb");
	if (in == NULL) {
		printf("file cannot open");
		return;
	}
	printf("success to open file\n");

	fread(in_data, sizeof(unsigned char), (X_MAX*Y_MAX) / 4, in);
	fclose(in);

	int pixel_x, pixel_y, size;
	double radian;
	printf("이동시킬 화소(위아래) : ");
	scanf("%d", &pixel_x);
	printf("이동시킬 화소(가로세로) : ");
	scanf("%d", &pixel_y);
	printf("키우고 싶은 배수 : ");
	scanf("%d", &size);
	printf("회전 각도 : ");
	scanf("%lf", &radian);

	for (i = 0; i < X_MAX * 2; i++) {
		for (j = 0; j < Y_MAX * 2; j++) {
			out_data3[i][j] = 0;
			out_data2[i][j] = 0;
		}
	}

	//Zoomout
	for (i = 0; i < Y_MAX / 2; i++) {
		for (j = 0; j < X_MAX / 2; j++) {
			if (i*size > X_MAX * 2 || j*size > Y_MAX * 2)
				continue;
			for (int k = 0; k < size; k++) {
				for (int l = 0; l < size; l++) {
					if (i*size + k > X_MAX * 2 || j*size + l > X_MAX * 2)
						continue;
					out_data[i*size + k][j*size + l] = in_data[i][j];
				}
			}
		}
	}


	double cc = cos(radian);
	double ss = sin(radian);

	//Translation && rotation
	for (i = 0; i < Y_MAX * 2; i++) {
		for (j = 0; j < Y_MAX * 2; j++) {
			//Translation
			int loc_x = i + pixel_x;
			int loc_y = j + pixel_y;
			//check range
			if (loc_x >= Y_MAX/2 + pixel_x || loc_y >= Y_MAX /2+ pixel_y || loc_x < pixel_x || loc_y < pixel_y)
				continue;

			int rot_i = (int)(cc*loc_x - ss*loc_y);
			int rot_j = (int)(ss*loc_x + cc*loc_y);

			//printf("%d %d\n", loc_x, loc_y);

			if (rot_i > X_MAX * 2 || rot_j > X_MAX * 2 || rot_i < 0 || rot_j < 0)
				continue;
			out_data2[rot_i][rot_j] = out_data[i][j];
		}
	}


	//hole 채우기
	for (i = 1; i < Y_MAX * 2 - 1; i++) {
		for (j = 1; j < Y_MAX * 2 - 1; j++) {
			if (out_data2[i][j] < 0)
				out_data2[i][j] = 0;
			else if (out_data2[i][j] > 255)
				out_data2[i][j] = 255;
			else if (out_data2[i][j] == 0) {
				out_data2[i][j] = (out_data2[i + 1][j] + out_data2[i - 1][j] + out_data2[i][j + 1] + out_data2[i][j - 1]) / 4;
			}
		}
	}


	out = fopen("lena_ZTR.img", "wb");
	fwrite(out_data2, sizeof(unsigned char), X_MAX*Y_MAX * 4, out);
	fclose(out);
	printf("success to write file\n");
}