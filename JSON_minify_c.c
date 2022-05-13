#include "JSON_minify_c.h"

#include <stdbool.h>

size_t JSON_minify_c(char* json, size_t len, char* new_str)
{
    bool in_string = false;
    bool in_multiline_comment = false;
    bool in_singleline_comment = false;
    bool backslash = false;
    char c = 0;

    size_t i = 0;
    size_t j = 0;
    while (i < len)
    {
        backslash = !backslash && c == '\\';
        c = json[i++];
        if (c == '\0') {
            break;
        }
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
                if (c == '\0') {
                    break;
                }
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
                new_str[j++] = c;
            }
            else if (in_string)
            {
                if (c != '\n' && c != '\r')
                {
                    new_str[j++] = c;
                }
            }
            else
            {
                if (c == '/')
                {
                    if (i >= len)
                    {
                        new_str[j++] = c;
                        break;
                    }
                    backslash = !backslash && c == '\\';
                    c = json[i++];
                    if (c == '\0') {
                        break;
                    }
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
                        new_str[j - 1] = '/';
                        new_str[j++] = c;
                    }
                }
                else if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
                {
                    new_str[j++] = c;
                }
            }
        }
    }
    if (i < len) {
        json[j] = '\0';
    }
    return j;
}