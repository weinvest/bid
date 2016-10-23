#pragma once
#include <string>
#include <map>
struct HttpResponse
{
	std::string status;
	std::string uri;
	std::map<std::string, std::string> headers;
	const char* content;
	size_t content_length;
};