#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996)
#define X_MAX 256
#define Y_MAX 256

int main() {
	FILE *in, *out;
	int i, j;
	char in_data[X_MAX/2][Y_MAX/2], out_data[X_MAX*2][Y_MAX*2], out_data2[X_MAX * 2][Y_MAX * 2];

	in = fopen("lena_ZoomOut2.img", "rb");
	if (in == NULL) {
		printf("file cannot open");
		return;
	}
	printf("success to open file\n");

	fread(in_data, sizeof(char), (X_MAX*Y_MAX)/4, in);
	fclose(in);

	int pixel_x,pixel_y, size;
	printf("이동시킬 화소(위아래) : ");
	scanf("%d", &pixel_x);
	printf("이동시킬 화소(가로세로) : ");
	scanf("%d", &pixel_y);
	printf("키우고 싶은 배수 : ");
	scanf("%d", &size);
	
	memset(out_data, 0, sizeof(out_data));
	memset(out_data2, 0, sizeof(out_data2));
	/*
	//ZoomOut & translation
	for (i = 0; i < Y_MAX/2; i++) {
		for (j = 0; j < X_MAX/2; j++) {

			//Translation
			int loc_x = i + pixel_x;
			int loc_y = j + pixel_y;

			//check range
			if (loc_x > Y_MAX*2 || loc_y > Y_MAX*2)
				continue;

			//ZoomOut
			for (int x = 0; x < size; x++) {
				for (int y = 0; y < size; y++) {
					//256,256이동 2배 확대에서 깨짐 
					//if문에서 문제 있는 듯! 사이즈 이동 후 커질때 상태 확인하기
					if (((loc_x*size + x) > Y_MAX * 2) || ((loc_y*size + y) > Y_MAX * 2))
						continue;
					out_data[loc_x*size + x][loc_y*size + y] = in_data[i][j];
				}
			}
		}
	}*/

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
	//Translation
	for (i = 0; i < Y_MAX * 2; i++) {
		for (j = 0; j < Y_MAX * 2; j++) {
			//Translation
			int loc_x = i + pixel_x;
			int loc_y = j + pixel_y;
			//check range
			if (loc_x >= Y_MAX * 2 || loc_y >= Y_MAX * 2 || loc_x < 0 || loc_y < 0)
				continue;

			out_data2[loc_x][loc_y] = out_data[i][j];
		}
	}
	

	//printf("finish for loop\n");
	/*
	//translation
	for (i = 0; i < X_MAX / 2 * size; i++) {
		for (j = 0; j < X_MAX / 2 * size; j++) {
			int loc_x = i + pixel_x;
			int loc_y = j + pixel_y;
			if(loc_x<512 && loc_y<512)
				out_data2[loc_x][loc_y] = out_data[i][j];
		}
	}
	*/
	out = fopen("lena_ZOT.img", "wb");
	fwrite(out_data2, sizeof(char), X_MAX*Y_MAX*4, out);
	fclose(out);
	printf("success to write file\n");

	return 0;
}