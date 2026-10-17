#ifndef TEXTO_HPP
#define TEXTO_HPP

#include <string>

class Texto {
public:
    static std::string aparar(const std::string& texto);
    static std::string normalizar(const std::string& texto);
};

#endif
