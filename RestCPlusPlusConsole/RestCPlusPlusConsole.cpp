// RestCPlusPlusConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cpprest\http_client.h>
#include <cpprest\filestream.h>

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;



int main()
{
	auto fileStream = std::make_shared<ostream>();
	pplx::task<void> requestTask = fstream::open_ostream(U("results.html")).then([=](ostream outFile) {
		*fileStream = outFile;
		//Create http_client to send the request
		http_client client(U("http://www.bing.com"));
		//Build a request URI and start the request
		uri_builder builder(U("/search"));
		builder.append_query(U("q"), U("cpprestsdk github"));
		return client.request(methods::GET, builder.to_string());
	})
	.then([=](http_response response) {
		printf("Received response status code:%u\n", response.status_code());
		return response.body().read_to_end(fileStream->streambuf());

	})
	.then([=](size_t) {
		return fileStream->close();
	});
	try
	{
		requestTask.wait();
	}
	catch (const std::exception &e)
	{
		printf("Error exception:%s\n", e.what());
	}
	system("pause");
	return 0;
}

