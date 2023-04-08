#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct {
	int16_t bfType;
	int32_t bfSize;
	int16_t bfReserved1;
	int16_t bfReserved2;
	int32_t bfOffBits;
} file_header;

typedef struct {
	int32_t biSize;
	int32_t biWidth;
	int32_t biHeight;
	int16_t biPlanes;
	int16_t biBitCount;
	int32_t biCompression;
	int32_t biSizeImage;
	int32_t biXPelsPerMeter;
	int32_t biYPelsPerMeter;
	int32_t biClrUsed;
	int32_t biClrImportant;
} file_info;

void fill_header(file_header *header, FILE *file) {
	uint32_t shift = 0;
	fread(&header->bfType, sizeof(header->bfType), 1, file);
	shift += sizeof(header->bfType);
	fseek(file, shift, 0);
	fread(&header->bfSize, sizeof(header->bfSize), 1, file);
	shift += sizeof(header->bfSize);
	fseek(file, shift, 0);
	fread(&header->bfReserved1, sizeof(header->bfReserved1), 1, file);
	shift += sizeof(header->bfReserved1);
	fseek(file, shift, 0);
	fread(&header->bfReserved2, sizeof(header->bfReserved2), 1, file);
	shift += sizeof(header->bfReserved2);
	fseek(file, shift, 0);
	fread(&header->bfOffBits, sizeof(header->bfOffBits), 1, file);
	shift += sizeof(header->bfOffBits);
	fseek(file, shift, 0);
}

void fill_info(file_info *info, FILE *file) {
	uint32_t shift = 0;
	fread(&info->biSize, sizeof(info->biSize), 1, file);
	shift += sizeof(info->biSize);
	fseek(file, shift, 0);
	fread(&info->biWidth, sizeof(info->biWidth), 1, file);
	shift += sizeof(info->biWidth);
	fseek(file, shift, 0);
	fread(&info->biHeight, sizeof(info->biHeight), 1, file);
	shift += sizeof(info->biHeight);
	fseek(file, shift, 0);
	fread(&info->biPlanes, sizeof(info->biPlanes), 1, file);
	shift += sizeof(info->biPlanes);
	fseek(file, shift, 0);
	fread(&info->biBitCount, sizeof(info->biBitCount), 1, file);
	shift += sizeof(info->biBitCount);
	fseek(file, shift, 0);
	fread(&info->biCompression, sizeof(info->biCompression), 1, file);
	shift += sizeof(info->biCompression);
	fseek(file, shift, 0);
	fread(&info->biSizeImage, sizeof(info->biSizeImage), 1, file);
	shift += sizeof(info->biSizeImage);
	fseek(file, shift, 0);
	fread(&info->biXPelsPerMeter, sizeof(info->biXPelsPerMeter), 1, file);
	shift += sizeof(info->biXPelsPerMeter);
	fseek(file, shift, 0);
	fread(&info->biYPelsPerMeter, sizeof(info->biYPelsPerMeter), 1, file);
	shift += sizeof(info->biYPelsPerMeter);
	fseek(file, shift, 0);
	fread(&info->biClrUsed, sizeof(info->biClrUsed), 1, file);
	shift += sizeof(info->biClrUsed);
	fseek(file, shift, 0);
	fread(&info->biClrImportant, sizeof(info->biClrImportant), 1, file);
	shift += sizeof(info->biClrImportant);
	fseek(file, shift, 0);
}

int32_t read_file(FILE *file, char **bytes) {
	fseek(file, 0, SEEK_END);
	int32_t file_size = ftell(file);
	*bytes = malloc(file_size * sizeof(char));
	fseek(file, 0, SEEK_SET);
	fread(*bytes, file_size * sizeof(char), 1, file);
	return file_size;
}

void printStructs(file_header *header, file_info *info) {
	printf("Header:\n");
	printf("	bfType: %x\n", header->bfType);
	printf("	bfSize: %x\n", header->bfSize);
	printf("	bfReserved1: %x\n", header->bfReserved1);
	printf("	bfReserved2: %x\n", header->bfReserved2);
	printf("	bfOffBits: %x\n", header->bfOffBits);
	printf("Info:\n");
	printf("	biSize: %x\n", info->biSize);
	printf("	biWidth: %x\n", info->biWidth);
	printf("	biHeight: %x\n", info->biHeight);
	printf("	biPlanes: %x\n", info->biPlanes);
	printf("	biBitCount: %x\n", info->biBitCount);
	printf("	biCompression: %x\n", info->biCompression);
	printf("	biSizeImage: %x\n", info->biSizeImage);
	printf("	biXPelsPerMeter: %x\n", info->biXPelsPerMeter);
	printf("	biYPelsPerMeter: %x\n", info->biYPelsPerMeter);
	printf("	biClrUsed: %x\n", info->biClrUsed);
	printf("	biClrImportant: %x\n", info->biClrImportant);
}

void getComponents(int shift, const char *bytes, int32_t file_size) {
	char *structs = (char*) malloc(shift);
	for (int i = 0; i < shift; ++i)
		*(structs + i) = *(bytes + i);
	char *colors[] = {"blue", "green", "red"};
	for (int i = 0; i < 3; ++i) {
		char *color = (char*) malloc((file_size - shift) * sizeof(char));
		for (int j = i; j < file_size - shift; j += 3) {
			*(color + j) = *(bytes + shift + j);
		}
		FILE *file = fopen(*(colors + i), "w");
		fwrite(structs, shift, 1, file);
		fwrite(color, file_size - shift, 1, file);
		fclose(file);
		//free(color);
	}
}

int main(int argc, char **argv) {
	if (argc != 2) {
		puts("Required two arguments!\n");
		return 1;
	}
	FILE *file = fopen(argv[1], "r");
	char *bytes;
	int32_t file_size = read_file(file, &bytes);
	//file_header *header = (file_header*) malloc(sizeof(file_header));
	//header = bytes;
	//file_info *info = (file_info*) malloc(sizeof(file_info));
	//info = bytes + sizeof(file_info);
	//memcpy(header, bytes, sizeof(file_header));
	file_header header;
	fill_header(&header, file);
	int shift = header.bfOffBits;
	getComponents(shift, bytes, file_size);
	fclose(file);
	return 0;
}
