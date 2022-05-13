#include "JSON_minify.h"

#include <iostream>
#include <vector>

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

// g++ tests.cpp JSON_minify.cpp
int main()
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
	std::cout << "Done." << std::endl;
}
