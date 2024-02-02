#ifndef SOUNDPROCESSOR_EXCEPTIONS_H
#define SOUNDPROCESSOR_EXCEPTIONS_H

#include <stdexcept>

class soundProcessorException : public std::runtime_error {
public:
    explicit soundProcessorException(const std::string& message) : std::runtime_error(message) {}
};

class unsupportedFormatException : public soundProcessorException {
public:
    explicit unsupportedFormatException(const std::string& message) : soundProcessorException(message) {}
};

class fileOpenException : public soundProcessorException {
public:
    explicit fileOpenException(const std::string& filename) : soundProcessorException("Error opening file: " + filename) {}
};

#endif //SOUNDPROCESSOR_EXCEPTIONS_H
