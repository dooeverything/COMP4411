#include "tga.h"

#include <stdio.h>
#include <string.h>

using namespace std;

typedef struct {

	// TGA File Header

	char	idLength; // 1byte
	char	colorMapType; // 1byte
	char	imageType; // 1byte

	// Color Map Specification - 5byte
	short	firstEntry; // 2byte
	short	numEntries; // 2byte
	char	bitsPerEntry; // 1byte

	// Image Specification
	short	xOrigin; // 2byte
	short	yOrigin; // 2byte
	short	width; // 2byte
	short	height; // 2byte
	char	bitsPerPixel; // 1byte
	char	descriptor; // 1byte
} TGAHEADER;



unsigned char* loadTGA(const char* fname, int& width, int& height, int& dataType)
{
	//std::cout << "Loading TGA: " << fname << std::endl;

	TGAHEADER tH;

	FILE* tgaFile = NULL;
	unsigned char* tgaData = NULL;

	if ((tgaFile = fopen(fname, "rb")) == NULL) {
		return tgaData;
	}

	fread(&tH.idLength, sizeof(tH.idLength), 1, tgaFile);
	fread(&tH.colorMapType, sizeof(tH.colorMapType), 1, tgaFile);
	fread(&tH.imageType, sizeof(tH.imageType), 1, tgaFile);
	
	
	fread(&tH.firstEntry, sizeof(tH.firstEntry), 1, tgaFile);
	fread(&tH.numEntries, sizeof(tH.numEntries), 1, tgaFile);
	fread(&tH.bitsPerEntry, sizeof(tH.bitsPerEntry), 1, tgaFile);
	
	fread(&tH.xOrigin, sizeof(tH.xOrigin), 1, tgaFile);
	fread(&tH.yOrigin, sizeof(tH.yOrigin), 1, tgaFile);
	fread(&tH.width, sizeof(tH.width), 1, tgaFile);
	fread(&tH.height, sizeof(tH.height), 1, tgaFile);
	fread(&tH.bitsPerPixel, sizeof(tH.bitsPerPixel), 1, tgaFile);
	fread(&tH.descriptor, sizeof(tH.descriptor), 1, tgaFile);

	// Check Image Type
	if ((tH.imageType != 2 && tH.imageType != 3 && tH.imageType != 10)) {
		cout << "No Image Type" << endl;
		fclose(tgaFile);
		return tgaData;
	}


	width = tH.width; //tH.width + tH.height*256;
	height = tH.height; //tH.width + tH.height*256;
	dataType = tH.bitsPerPixel / 8;

	// If each pixel does not contain RGB or RGBA
	if (dataType != 3 && dataType != 4) {
		cout << "No RGB or RGBA" << endl;
		fclose(tgaFile);
		return tgaData;
	}

	int tgaSize = width * height * dataType;
	

	try {
	
		tgaData = new unsigned char[tgaSize];
	}
	catch (const bad_alloc& ex)
	{
		cout << ex.what() << endl;
		cout << " Ran out of memory" << endl;
	}


	//cout << "The size of tga image is " << width << " " << height << " " << byteCount << endl;

	// Read the image data
	if (!tgaData) return NULL;
	
	fread(tgaData, sizeof(unsigned char), tgaSize, tgaFile);

	unsigned char colorSwap;

	for (int imageIdx = 0; imageIdx < tgaSize; imageIdx += dataType)
	{
		colorSwap = tgaData[imageIdx];
		tgaData[imageIdx] = tgaData[imageIdx + 2];
		tgaData[imageIdx + 2] = colorSwap;
	}

	fclose(tgaFile);

	return tgaData;
}
