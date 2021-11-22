#include <string>
#include <stdexcept>

namespace Except {
    class LinearAlgebraException: public std::runtime_error {
        public:
            LinearAlgebraException() : runtime_error("Matrix dimensions do not match") {};
            LinearAlgebraException(std::string msg) : runtime_error(msg) {};
    };
};