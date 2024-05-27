#ifdef _WIN32
#include "wconnection.hpp"
#elif defined(__linux__)
#include "lconnection.hpp"
#endif

std::string portSelection(std::vector<std::string> ports);