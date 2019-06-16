#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#pragma warning(disable:4996)

#define X_MAX 512
#define Y_MAX 512

int main() {
	int i, j;

	FILE *in, *out;

	unsigned char in_data[X_MAX][Y_MAX];
	unsigned char **out_data;
	unsigned char **out_data2;
	//unsigned char out_data2[X_MAX*4][Y_MAX*4];

	out_data = (unsigned char **)malloc(sizeof(unsigned char *)*Y_MAX * 2);
	for (i = 0; i < Y_MAX * 2; i++) {
		out_data[i] = (unsigned char *)malloc(sizeof(unsigned char)*X_MAX * 2);
	}
	
	out_data2 = (unsigned char **)malloc(sizeof(unsigned char *)*Y_MAX * 4);
	for (i = 0; i < Y_MAX * 4; i++) {
		out_data2[i] = (unsigned char *)malloc(sizeof(unsigned char)*X_MAX * 4);
	}
	
	//파일 열기
	in = fopen("lena.img", "rb");
	if (in == NULL) {
		printf("파일 열기 실패\n");
		return 1;
	}


	fread(in_data, sizeof(unsigned char), X_MAX * Y_MAX, in);
	fclose(in);


	//zoom IN (1024*1024)
	for (i = 0; i < Y_MAX; i++) {
		for (j = 0; j < X_MAX; j++) {
			out_data[i * 2][j * 2] = in_data[i][j];
			out_data[i * 2 + 1][j * 2] = in_data[i][j];
			out_data[i * 2][j * 2 + 1] = in_data[i][j];
			out_data[i * 2 + 1][j * 2 + 1] = in_data[i][j];
		}
	}

	//zoom IN (2048	*2048)
	for (i = 0; i < Y_MAX; i++) {
		for (j = 0; j < X_MAX; j++) {

			for (int k = 0; k < 4; k++) {
				for (int l = 0; l < 4; l++) {
					out_data2[i * 4 + k][j * 4 + l] = in_data[i][j];
				}
			}
		}
	}

	//저장
	out = fopen("lena_ZoomIn1.img", "wb");
	for (int i = 0; i < X_MAX * 2; i++)
		fwrite(out_data[i], sizeof(unsigned char), X_MAX * 2, out);

	out = fopen("lena_ZoomIn2.img", "wb");
	for (int i = 0; i < X_MAX * 4; i++)
		fwrite(out_data2[i], sizeof(unsigned char), X_MAX * 4, out);

	fclose(out);

	/*for (int i = 0; i < X_MAX; i++)
		free(out_data[i]);
	free(out_data);
	*/
	printf("파일 쓰기 성공");

	return 0;
}