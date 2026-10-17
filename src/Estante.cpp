#include "Estante.hpp"

#include <algorithm>

#include "Texto.hpp"

Estante::Estante()
    : proximaOrdem(0) {
}

Resultado Estante::cadastrar(const std::string& titulo, const std::string& autor, const std::string& genero) {
    std::string tituloAparado = Texto::aparar(titulo);
    std::string autorAparado = Texto::aparar(autor);
    std::string generoAparado = Texto::aparar(genero);

    if (tituloAparado.empty() || autorAparado.empty() || generoAparado.empty()) {
        return CAMPO_VAZIO;
    }
    if (buscar(tituloAparado) != livros.end()) {
        return JA_EXISTE;
    }

    ++proximaOrdem;
    livros.push_back(Livro(tituloAparado, autorAparado, generoAparado, proximaOrdem));
    return OK;
}

Resultado Estante::remover(const std::string& titulo) {
    if (livros.empty()) {
        return ESTANTE_VAZIA;
    }

    std::list<Livro>::iterator encontrado = buscar(titulo);
    if (encontrado == livros.end()) {
        return NAO_ENCONTRADO;
    }

    livros.erase(encontrado);
    return OK;
}

bool Estante::vazia() const {
    return livros.empty();
}

std::vector<Livro> Estante::listar() const {
    std::vector<Livro> resultado;
    for (std::list<Livro>::const_iterator it = livros.begin(); it != livros.end(); ++it) {
        resultado.push_back(*it);
    }
    return resultado;
}

std::list<Livro>::iterator Estante::buscar(const std::string& titulo) {
    return std::find(livros.begin(), livros.end(), Livro(titulo, "", "", 0));
}
