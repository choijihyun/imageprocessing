#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)
#define X_MAX 512
#define Y_MAX 512


void main(void)
{
	int i, j;
	FILE *in, *out;

	char in_data[X_MAX][Y_MAX];
	char out_data[X_MAX][Y_MAX];



	// �̹����� �о�´�

	in = fopen("lena.img", "rb");
	if (in == NULL)
	{
		printf("File not found!!\n");
		return;
	}
	fread(in_data, sizeof(char), X_MAX * Y_MAX, in);
	fclose(in);



	// �о�� �̹����� �������� �����Ѵ�

	for (i = 0; i < Y_MAX; i++)
		for (j = 0; j < X_MAX; j++) {
			//printf("%d", in_data[i][j]);
			out_data[i][j] = 255 - in_data[i][j];
		}



	// �������� �ٲ� �̹����� �����Ѵ�

	out = fopen("lenna_o.img", "wb");

	fwrite(out_data, sizeof(char), X_MAX * Y_MAX, out);
	fclose(out);
}
