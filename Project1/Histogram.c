#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)
#define X_MAX 512
#define Y_MAX 512

int main() {
	FILE *in, *out;
	int i, j;
	unsigned char in_data[X_MAX][Y_MAX], out_data[X_MAX][Y_MAX];
	int histogram[256]; // 8bit image일 경우, L=256, 256개의 gray level이 있다고 하자
	int s[256]; // 8bit image일 경우, L=256

	in = fopen("lena.img", "rb");
	if (in == NULL) {
		printf("file cannot open");
		return;
	}
	printf("success to open file\n");

	//fread(in_data, sizeof(unsigned char), X_MAX*Y_MAX, in);

	fread(&in_data[0][0], sizeof(unsigned char), Y_MAX*X_MAX, in);

	fclose(in);

	memset(histogram, 0, sizeof(histogram));
	memset(s, 0, sizeof(s));

	// nk 구하기
	int cnt = 0;
	for (i = 0; i < 256; i++) {
		histogram[i] = 0;
	}

	for (i = 0; i < Y_MAX; i++) {
		for (j = 0; j < X_MAX; j++) {
			histogram[in_data[i][j]]+=1;
		}
	}

	for (i = 0; i < 256; i++) {
		printf("%d\n", histogram[i]);
		cnt += histogram[i];
	}
	printf("cnt = %d\n", cnt);
	// pr 구하기
	float sum = 0;
	for (i = 0; i < 256; i++) {
		//printf("%f\n", (float)(histogram[i] / (float)(512 * 512)));
		sum += (float)(histogram[i] / (float)(512 * 512));
		//printf("sum = %f\n", sum);
		s[i] = (int)(sum * 255);
		//printf("s[%d] = %d\n", i, s[i]);
	}
	for (i = 0; i < Y_MAX; i++) {
		for (j = 0; j < X_MAX; j++) {
			out_data[i][j] = (unsigned char)s[in_data[i][j]];
		}
	}

	out = fopen("lena_H.img", "wb");
	fwrite(out_data, sizeof(unsigned char), X_MAX*Y_MAX, out);
	fclose(out);
	printf("success to write file\n");
}