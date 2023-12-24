#ifndef EXEPTION_HPP_
#define EXEPTION_HPP_

#include <iostream>
#include <stdexcept>

class SoundProcessorException : public std::runtime_error {
   public:
    explicit SoundProcessorException(const std::string& message) : std::runtime_error(message) {}
};

class ArgumentException : public SoundProcessorException {
   public:
    explicit ArgumentException(const std::string& message) : SoundProcessorException(message) {}
};

class UnsupportedFormatException : public SoundProcessorException {
   public:
    explicit UnsupportedFormatException(const std::string& message) : SoundProcessorException(message) {}
};

class FileOpenException : public SoundProcessorException {
   public:
    explicit FileOpenException(const std::string& filename) : SoundProcessorException("Failed to open file: " + filename) {}
};

#endif  // EXEPTION_HPP_
