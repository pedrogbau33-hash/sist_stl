#ifndef ESTANTE_HPP
#define ESTANTE_HPP

#include <list>
#include <string>
#include <vector>

#include "Livro.hpp"

enum Resultado {
    OK,
    NAO_ENCONTRADO,
    JA_EXISTE,
    CAMPO_VAZIO,
    NAO_LIDO,
    NOTA_INVALIDA,
    ESTANTE_VAZIA,
    SEM_LIDOS,
    SEM_AVALIADOS
};

struct GeneroEstatistica {
    std::string nome;
    int quantidade;
};

class Estante {
public:
    Estante();

    Resultado cadastrar(const std::string& titulo, const std::string& autor, const std::string& genero);
    Resultado definirStatus(const std::string& titulo, bool lido);
    Resultado avaliar(const std::string& titulo, const std::string& entradaNota, const std::string& entradaComentario);
    Resultado remover(const std::string& titulo);

    bool vazia() const;
    bool temLidos() const;
    bool temAvaliados() const;
    std::vector<Livro> listar() const;
    std::vector<Livro> livrosDoAutor(const std::string& autor) const;
    std::vector<GeneroEstatistica> generosMaisLidos() const;
    std::vector<Livro> livrosMaisBemAvaliados() const;

private:
    std::list<Livro> livros;
    int proximaOrdem;

    std::list<Livro>::iterator buscar(const std::string& titulo);
};

#endif
