#include "Texto.hpp"

#include <cctype>

std::string Texto::aparar(const std::string& texto) {
    std::size_t inicio = 0;
    std::size_t fim = texto.size();
    while (inicio < fim && std::isspace(static_cast<unsigned char>(texto[inicio]))) {
        ++inicio;
    }
    while (fim > inicio && std::isspace(static_cast<unsigned char>(texto[fim - 1]))) {
        --fim;
    }
    return texto.substr(inicio, fim - inicio);
}

std::string Texto::normalizar(const std::string& texto) {
    std::string resultado = aparar(texto);
    for (std::size_t i = 0; i < resultado.size(); ++i) {
        unsigned char c = static_cast<unsigned char>(resultado[i]);
        if (c < 0x80) {
            resultado[i] = static_cast<char>(std::tolower(c));
        }
    }
    return resultado;
}
