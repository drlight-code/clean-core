#include "from_string.hh"

#include <sstream>
#include <string>

// NOTE: this is just to get the API and basic functionality up
//       <charconv> still has spotty support and is thus not an option
namespace
{
template <class T>
bool parse(cc::string_view s, T& v)
{
    std::istringstream ss(std::string(s.data(), s.size()));
    ss >> v;
    return bool(ss) && ss.eof();
}
}

bool cc::from_string(cc::string_view s, signed char& v) { return parse(s, v); }

bool cc::from_string(cc::string_view s, signed short& v) { return parse(s, v); }

bool cc::from_string(cc::string_view s, signed int& v) { return parse(s, v); }

bool cc::from_string(cc::string_view s, signed long& v) { return parse(s, v); }

bool cc::from_string(cc::string_view s, signed long long& v) { return parse(s, v); }

bool cc::from_string(cc::string_view s, unsigned char& v) { return parse(s, v); }

bool cc::from_string(cc::string_view s, unsigned short& v) { return parse(s, v); }

bool cc::from_string(cc::string_view s, unsigned int& v) { return parse(s, v); }

bool cc::from_string(cc::string_view s, unsigned long& v) { return parse(s, v); }

bool cc::from_string(cc::string_view s, unsigned long long& v) { return parse(s, v); }

bool cc::from_string(cc::string_view s, float& v) { return parse(s, v); }

bool cc::from_string(cc::string_view s, double& v) { return parse(s, v); }
