#include <iostream>
#include "../HEADER/mycolor.hpp"
#include <experimental/source_location>

static void print_ascii(std::string s){for (int i: s){std::cout << i << " " << static_cast<char>(i) << " ";}std::cout <<'\n';}

void	log(const std::string &message, bool ascii = false,
		const std::experimental::source_location location =
			std::experimental::source_location::current())
{
	std::clog << FG_LMAGENTA"file: "
			<< location.file_name() << "("
			<< location.line() << ":"
			<< location.column() << ") `"
			<< location.function_name() << "`: "
			<< message << FG_DEFAULT << std::endl;
	if (ascii)
		print_ascii(message);
}

int		err_int(int err, int res,const std::string &message,
		const std::experimental::source_location location =
			std::experimental::source_location::current())
{
	if (res == err)
	{
		std::cerr << FG_LRED"Error in file: "
			<< location.file_name() << "("
			<< location.line() << ":"
			<< location.column() << ") `"
			<< location.function_name() << "`: "
			<< message << FG_DEFAULT << std::endl;
		exit (1);
	} 
	return (res);
}

void	*err_void(void *err, void *res, const std::string &message,
		const std::experimental::source_location location =
			std::experimental::source_location::current())
{
	if (res == err)
	{
		std::cerr << FG_LRED"Error in file: "
			<< location.file_name() << "("
			<< location.line() << ":"
			<< location.column() << ") `"
			<< location.function_name() << "`: "
			<< message << FG_DEFAULT << std::endl;
		exit (1);
	} 
	return (res);
}
