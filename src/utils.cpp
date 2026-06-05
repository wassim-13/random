#include "utils.h"

vector<string> split_whitespace(const string& str)
{
    vector<string> vec_s;
    string temp;

    for (size_t j = 0; j <= str.size(); j++)
    {
        if (str[j] == ' ' || str[j] == '\n' || str[j] == '\0')
        {
            vec_s.push_back(temp);
            temp.clear();
            continue;
        }
        temp.push_back(str[j]);
    }
    return vec_s;
}
