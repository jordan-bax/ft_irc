#include <iostream>
#include "../mycolor.hpp"
#include <experimental/source_location>


void	log(const std::string &message,
		const std::experimental::source_location location =
			std::experimental::source_location::current())
{
	std::clog << FG_LMAGENTA"file: "
			<< location.file_name() << "("
			<< location.line() << ":"
			<< location.column() << ") `"
			<< location.function_name() << "`: "
			<< message << FG_DEFAULT << std::endl;
}

int		err_int(int err, int res,const std::string &message,
		const std::experimental::source_location location =
			std::experimental::source_location::current())
{
	if (res == err)
	{
		std::cerr << FG_LRED"Error in file: "
			<< location.file_name() << "(L "
			<< location.line() << ":C "
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
			<< location.file_name() << "(L "
			<< location.line() << ":C "
			<< location.column() << ") `"
			<< location.function_name() << "`: "
			<< message << FG_DEFAULT << std::endl;
		exit (1);
	} 
	return (res);
}
