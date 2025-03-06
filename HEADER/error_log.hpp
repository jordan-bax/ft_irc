#pragma once
#include <iostream>
#include <experimental/source_location>

void	log(const std::string &message, bool ascii = false,
		const std::experimental::source_location location =
			std::experimental::source_location::current());

int		err_int(int err, int res,const std::string &message,
		const std::experimental::source_location location =
			std::experimental::source_location::current());

void	*err_void(void *err, void *res, const std::string &message,
		const std::experimental::source_location location =
			std::experimental::source_location::current());