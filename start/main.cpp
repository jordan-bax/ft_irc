#include <iostream>
#include <filesystem>
#include <chrono>
// #define fsize 1000
// void bzero(char *s, size_t n){
// 	for (size_t i = 0 ; n > i; i++)
// 	{
// 		s[i] = '\0';
// 	}
// }

// int main(int argc, char const *argv[])
// {
// 	FILE * file;
// 	FILE * copy;
// 	char transfer[fsize];
// 	file = fopen(argv[1], "r+");
// 	copy = fopen("copy", "w+");
// 	bzero(transfer, fsize);
// 	while (fread(transfer, sizeof(char), fsize, file)){
// 		fwrite(transfer,sizeof(char), fsize, copy);
// 	}
// 	fclose(copy);
// 	fclose(file);
// 	return 0;
	
// }

int main(void)
{
	// Using time point and system_clock
	std::chrono::time_point<std::chrono::system_clock> start, end;

	start = std::chrono::system_clock::now();
	end = std::chrono::system_clock::now();
	time_t date;
	// std::time(date);
	date = std::chrono::system_clock::to_time_t(start);
	std::cout << ctime(&date);

	std::chrono::duration<double> elapsed_seconds = end - start;
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);

	std::cout << "finished computation at " << std::ctime(&end_time)
			<< "elapsed time: " << elapsed_seconds.count() << "s\n";
	return 0;
}