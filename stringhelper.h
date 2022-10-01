#ifndef STRINGHELPER_H
#define STRINGHELPER_H
#include <sstream>

template <typename T>
std::string toString(const T& value);
template <typename T>
std::string toString(const T& value)
{
    std::stringstream stream;
    stream << value;
    return stream.str();
}

#endif // STRINGHELPER_H
