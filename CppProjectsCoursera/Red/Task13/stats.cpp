#include "stats.h"
#include <algorithm>

const array<string, 5> Stats::MethodNames = {"GET",
                                             "PUT",
                                             "POST",
                                             "DELETE",
                                             "UNKNOWN"};
const array<string, 6> Stats::UriNames = {"/",
                                          "/order",
                                          "/product",
                                          "/basket",
                                          "/help",
                                          "unknown"};

HttpRequest ParseRequest(string_view line)
{
    HttpRequest new_Request;
    line.remove_prefix(line.find_first_not_of(' '));
    size_t space = line.find(' ');
    new_Request.method = line.substr(0, space);
    if (space == line.npos) { new_Request.uri = Stats::UriNames[5]; return new_Request;}
    line.remove_prefix(space + 1);
    space = line.find(' ');
    new_Request.uri = line.substr(0, space);
    if (space != line.npos) new_Request.protocol = line.substr(space + 1);
    return new_Request;
}

Stats::Stats()
{
    for (const string_view method_name : MethodNames)
    {
        MethodStats[method_name] = 0;
    }
    for (const string_view uri_name : UriNames)
    {
        UriStats[uri_name] = 0;
    }
}

void Stats::AddMethod(string_view method)
{
    if (find(Stats::MethodNames.begin(), Stats::MethodNames.end(), method) != Stats::MethodNames.end())
        ++MethodStats[method];
    else
        ++MethodStats["UNKNOWN"];
}

void Stats::AddUri(string_view uri)
{
    if (find(Stats::UriNames.begin(), Stats::UriNames.end(), uri) != Stats::UriNames.end())
        ++UriStats[uri];
    else
        ++UriStats["unknown"];
}

const map<string_view, int>& Stats::GetMethodStats() const
{
    return MethodStats;
}

const map<string_view, int>& Stats::GetUriStats() const
{
    return UriStats;
}
