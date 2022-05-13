#include "JSON_minify_c.h"
#include "JSON_minify.h"

#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>

struct Test
{
	std::string source;
	std::string assert;
};

std::vector<Test> tests{
	Test{
		"\
			// this is a JSON file with comments\n\
			{\n\
				\"foo\": \"bar\",	// this is cool\n\
				\"bar\": [\n\
					\"baz\", \"bum\", \"zam\"\n\
				],\n\
			/* the rest of this document is just fluff\n\
			   in case you are interested. */\n\
				\"something\": 10,\n\
				\"else\": 20\n\
			}\n\
			\n\
			/* NOTE: You can easily strip the whitespace and comments\n\
			   from such a file with the JSON.minify() project hosted\n\
			   here on github at http://github.com/getify/JSON.minify\n\
			*/\n",
		"{\"foo\":\"bar\",\"bar\":[\"baz\",\"bum\",\"zam\"],\"something\":10,\"else\":20}"
	},
	Test{
		"\
			\n\
			{\"/*\":\"*/\",\"//\":\"\",/*\"//\"*/\"/*/\"://\n\
			\"//\"}\n\
			\n",
		"{\"/*\":\"*/\",\"//\":\"\",\"/*/\":\"//\"}"
	},
	Test{
		"\
			/*\n\
			this is a\n\
			multi line comment */{\n\
			\n\
			\"foo\"\n\
			:\n\
				\"bar/*\"// something\n\
				,	\"b\\\"az\":/*\n\
			something else */\"blah\"\n\
			\n\
			}\n",
		"{\"foo\":\"bar/*\",\"b\\\"az\":\"blah\"}"
	},
	Test{
		"\
			{\"foo\": \"ba\\\"r//\", \"bar\\\\\": \"b\\\\\\\"a/*z\",\n\
				\"baz\\\\\\\\\": /* yay */ \"fo\\\\\\\\\\\"*/o\"\n\
			}\n",
		"{\"foo\":\"ba\\\"r//\",\"bar\\\\\":\"b\\\\\\\"a/*z\",\"baz\\\\\\\\\":\"fo\\\\\\\\\\\"*/o\"}"
	}
};

char* data(std::string& str)
{
	// return str.data(); // no non-const version in old C++
	return str.empty() ? nullptr : &str[0];
}

int test_cpp()
{
	for (size_t idx = 0; idx < tests.size(); idx++)
	{
		const auto& test = tests[idx];
		auto res = JSON_minify(test.source);
		if (test.assert != res) {
			std::cout << "Test " << (idx + 1) << " failed:\n" << res << std::endl;
			return 1;
		}
		std::cout << "Test " << (idx + 1) << " passed" << std::endl;
	}
	return 0;
}

int test_c()
{
	for (size_t idx = 0; idx < tests.size(); idx++)
	{
		auto& test = tests[idx];
		auto res_size = JSON_minify_c(data(test.source), test.source.size(), data(test.source));
		std::string res(test.source.data(), res_size);
		if (test.assert != res) {
			std::cout << "Test " << (idx + 1) << " failed:\n" << res << std::endl;
			return 1;
		}
		std::cout << "Test " << (idx + 1) << " passed" << std::endl;
	}
	return 0;
}

std::string read_file(const std::string& file_name)
{
	std::ifstream fr(file_name);
	fr.seekg(0, std::ios::end);
	size_t len = fr.tellg();
	std::string buf(len, '\0');
	fr.seekg(0);
	fr.read(data(buf), len); 
	return buf;
}

// g++ tests.cpp JSON_minify.cpp JSON_minify_c.c
int main(int argc, char* argv[])
{
	if (argc > 1) {
		auto json = read_file(argv[1]);		
		std::cout << json.size() << std::endl;
		{
			auto start = std::chrono::high_resolution_clock::now();
			auto res = JSON_minify(json);
			auto end = std::chrono::high_resolution_clock::now();
			auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			std::cout << "cpp time " << time << " ms = " << json.size() / time << " chars/ms" << std::endl;
			std::cout << res.size() << std::endl;
		}
		{
			auto start = std::chrono::high_resolution_clock::now();
			std::string res(json);
			auto len = JSON_minify_c(data(json), json.size(), data(res));
			auto end = std::chrono::high_resolution_clock::now();
			auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			std::cout << "c time " << time << " ms = " << json.size() / time << " chars/ms" << std::endl;
			std::cout << res.size() << std::endl;
		}
	}
	else {
		test_cpp();
		test_c();
		std::cout << "Done." << std::endl;
	}
}
