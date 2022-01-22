#pragma once

#include <stdlib.h>

#define datatype const char*
#define datatypenoconst char*

datatype LoadRom(const char* path, int* dataSize)
{
	FILE* inFile;
#ifdef WIN32
    fopen_s(&inFile, path, "rb");
#else
    inFile = fopen(path, "rb");
#endif // WIN32


	if (!inFile)
		return NULL;

	fseek(inFile, 0, SEEK_END);
	*dataSize = ftell(inFile);
	fseek(inFile, 0, SEEK_SET);

	datatypenoconst data = (datatypenoconst)malloc(*dataSize);
	fread(data, 4096, sizeof(char), inFile);

	fclose(inFile);
	return data;
}

void UnloadRom(datatype data)
{
	free((datatypenoconst)data);
}
