#include "JSON_minify.h"

std::string JSON_minify(const std::string& json)
{
    bool in_string = false;
    bool in_multiline_comment = false;
    bool in_singleline_comment = false;
    bool backslash = false;
    size_t len = json.length();
    std::string new_str;
    size_t from = 0;
    size_t new_chars = 0;
    size_t add_chars = 0;
    char c = 0;

    new_str.reserve(len);
    for (size_t i = 0; i < len;)
    {
        backslash = !backslash && c == '\\';
        c = json[i++];
        if (in_singleline_comment)
        {
            if (c == '\r' || c == '\n')
            {
                in_singleline_comment = false;
            }
        }
        else if (in_multiline_comment)
        {
            if (c == '*')
            {
                if (i >= len)
                {
                    break;
                }
                c = json[i++];
                if (c == '/')
                {
                    in_multiline_comment = false;
                }
            }
        }
        else
        {
            if (c == '\"' && !backslash)
            {
                in_string = !in_string;
                add_chars = 1;
            }
            else if (in_string)
            {
                if (c != '\n' && c != '\r')
                {
                    add_chars = 1;
                }
            }
            else
            {
                if (c == '/')
                {
                    if (i >= len)
                    {
                        add_chars = 1;
                        break;
                    }
                    backslash = !backslash && c == '\\';
                    c = json[i++];
                    if (c == '/')
                    {
                        in_singleline_comment = true;
                    }
                    else if (c == '*')
                    {
                        in_multiline_comment = true;
                    }
                    else
                    {
                        add_chars = 2;
                    }
                }
                else if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
                {
                    add_chars = 1;
                }
            }
        }
        if (add_chars > 0)
        {
            if (new_chars == 0)
            {
                from = i - add_chars;
            }
            new_chars += add_chars;
            add_chars = 0;
        }
        else if (new_chars > 0)
        {
            new_str += json.substr(from, new_chars);
            new_chars = 0;
        }
    }
    if (new_chars > 0)
    {
        new_str += json.substr(from, new_chars);
    }
    return new_str;
}