
#pragma once

#include <sstream>
#include <stdexcept>
#include <string>

#include "libnlp/base/export.h"

#if defined(_MSC_VER) && _MSC_VER < 1900
// Before Visual Studio 2015 (14.0), C++ 11 "noexcept" qualifier is not
// supported
#define noexcept
#endif // ifdef _MSC_VER

namespace libnlp {

    class LIBNLP_EXPORT Exception {
    public:
        Exception() {}

        virtual ~Exception() throw() {}

        Exception(const std::string &_message) : message(_message) {}

        virtual const char *what() const noexcept { return message.c_str(); }

    protected:
        std::string message;
    };

    class LIBNLP_EXPORT FileNotFound : public Exception {
    public:
        FileNotFound(const std::string &fileName)
                : Exception(fileName + " not found or not accessible.") {}
    };

    class LIBNLP_EXPORT FileNotWritable : public Exception {
    public:
        FileNotWritable(const std::string &fileName)
                : Exception(fileName + " not writable.") {}
    };

    class LIBNLP_EXPORT InvalidFormat : public Exception {
    public:
        InvalidFormat(const std::string &message)
                : Exception("Invalid format: " + message) {}
    };

    class LIBNLP_EXPORT InvalidTextDictionary : public InvalidFormat {
    public:
        InvalidTextDictionary(const std::string &_message, size_t lineNum)
                : InvalidFormat("") {
            std::ostringstream buffer;
            buffer << "Invalid text dictionary at line " << lineNum << ": " << _message;
            message = buffer.str();
        }
    };

    class LIBNLP_EXPORT InvalidUTF8 : public Exception {
    public:
        InvalidUTF8(const std::string &_message)
                : Exception("Invalid UTF8: " + _message) {}
    };

    class LIBNLP_EXPORT ShouldNotBeHere : public Exception {
    public:
        ShouldNotBeHere() : Exception("ShouldNotBeHere! This must be a bug.") {}
    };

} // namespace libnlp
