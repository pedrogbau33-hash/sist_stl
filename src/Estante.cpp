#include "Estante.hpp"

#include <algorithm>

#include "Texto.hpp"

static bool interpretarNota(const std::string& texto, int& valor) {
    if (texto.empty()) {
        return false;
    }

    int acumulado = 0;
    for (std::size_t i = 0; i < texto.size(); ++i) {
        char c = texto[i];
        if (c < '0' || c > '9') {
            return false;
        }
        acumulado = acumulado * 10 + (c - '0');
        if (acumulado > 10) {
            return false;
        }
    }

    valor = acumulado;
    return true;
}

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

Resultado Estante::definirStatus(const std::string& titulo, bool lido) {
    if (livros.empty()) {
        return ESTANTE_VAZIA;
    }

    std::list<Livro>::iterator encontrado = buscar(titulo);
    if (encontrado == livros.end()) {
        return NAO_ENCONTRADO;
    }

    encontrado->definirLido(lido);
    return OK;
}

Resultado Estante::avaliar(const std::string& titulo, const std::string& entradaNota, const std::string& entradaComentario) {
    if (livros.empty()) {
        return ESTANTE_VAZIA;
    }

    std::list<Livro>::iterator encontrado = buscar(titulo);
    if (encontrado == livros.end()) {
        return NAO_ENCONTRADO;
    }
    if (!encontrado->lido()) {
        return NAO_LIDO;
    }

    std::string notaTexto = Texto::aparar(entradaNota);
    bool vaiAlterarNota = false;
    bool vaiRemoverNota = false;
    int novaNota = -1;

    if (notaTexto == "-") {
        vaiAlterarNota = false;
    } else if (notaTexto.empty()) {
        vaiAlterarNota = true;
        vaiRemoverNota = true;
    } else {
        int valor;
        if (!interpretarNota(notaTexto, valor)) {
            return NOTA_INVALIDA;
        }
        vaiAlterarNota = true;
        novaNota = valor;
    }

    std::string comentarioTexto = Texto::aparar(entradaComentario);
    bool vaiAlterarComentario = false;
    bool vaiRemoverComentario = false;
    std::string novoComentario;

    if (comentarioTexto == "-") {
        vaiAlterarComentario = false;
    } else if (comentarioTexto.empty()) {
        vaiAlterarComentario = true;
        vaiRemoverComentario = true;
    } else {
        vaiAlterarComentario = true;
        novoComentario = comentarioTexto;
    }

    if (vaiAlterarNota) {
        if (vaiRemoverNota) {
            encontrado->removerNota();
        } else {
            encontrado->definirNota(novaNota);
        }
    }
    if (vaiAlterarComentario) {
        if (vaiRemoverComentario) {
            encontrado->removerComentario();
        } else {
            encontrado->definirComentario(novoComentario);
        }
    }

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
