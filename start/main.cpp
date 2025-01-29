#include <iostream>
#include <filesystem>
#define fsize 1000
void bzero(char *s, size_t n){
	for (size_t i = 0 ; n > i; i++)
	{
		s[i] = '\0';
	}
}

int main(int argc, char const *argv[])
{
	FILE * file;
	FILE * copy;
	char transfer[fsize];
	file = fopen(argv[1], "r+");
	copy = fopen("copy", "w+");
	bzero(transfer, fsize);
	while (fread(transfer, sizeof(char), fsize, file)){
		fwrite(transfer,sizeof(char), fsize, copy);
	}
	fclose(copy);
	fclose(file);
	return 0;
	
}
