#ifndef ARQUIVO_HPP
#define ARQUIVO_HPP

#include <string>
#include <vector>

#include "Livro.hpp"

class Arquivo {
public:
    explicit Arquivo(const std::string& nomeArquivo);

    void carregar(std::vector<Livro>& livros) const;
    void salvar(const std::vector<Livro>& livros) const;

private:
    std::string nomeArquivo_;

    static std::string escapar(const std::string& texto);
    static bool desescapar(const std::string& texto, std::string& resultado);
    static bool dividir(const std::string& linha, std::vector<std::string>& campos);
    static bool livroDaLinha(const std::string& linha, Livro& livro, std::string& motivo);
    static std::string linhaDoLivro(const Livro& livro);
};

#endif
