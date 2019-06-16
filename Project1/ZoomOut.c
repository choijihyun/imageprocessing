#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)

#define X_MAX 512
#define Y_MAX 512

int main() {
	FILE *in, *out, *out2;

	unsigned char in_data[X_MAX][Y_MAX];
	unsigned char out_data[X_MAX / 2][Y_MAX / 2];  //256x256
	unsigned char out_data2[X_MAX / 4][Y_MAX / 4]; //128x128

	//파일 열기
	in = fopen("lena.img", "rb");
	if (in == NULL) {
		printf("파일 열기 실패\n");
		return 1;
	}

	fread(in_data, sizeof(unsigned char), X_MAX * Y_MAX, in);
	fclose(in);

	//zoom out (256*256)
	for (int i = 0; i < X_MAX/2; i++) {
		for (int j = 0; j < Y_MAX/2; j++) {
			/*out_data[i][j] = (in_data[i * 2][j * 2]
				+ in_data[i * 2 + 1][j * 2] +
				in_data[i * 2][j * 2 + 1] +
				in_data[i * 2 + 1][j * 2 + 1]) / 4;
			*/
			out_data[i][j] = in_data[i * 2][j * 2];
		}
	}
	//zoom out (128*128)
	for (int i = 0; i < X_MAX / 4; i++) {
		for (int j = 0; j < Y_MAX / 4; j++) {
			out_data2[i][j] = in_data[i * 4][j * 4];

		}
	}


	//저장
	out = fopen("lena_ZoomOut1.img", "wb");
	out2 = fopen("lena_ZoomOut2.img", "wb");
	fwrite(out_data, sizeof(unsigned char), (X_MAX / 2) * (Y_MAX / 2), out);
	fwrite(out_data2, sizeof(unsigned char), (X_MAX / 4) * (Y_MAX / 4), out2);

	fclose(out);
	fclose(out2);

	printf("파일 쓰기 성공!");
	/*
	for (int i = 0; i < 512; i++)
		free(a[i]);
	free(a);
	*/
	return 0;
}