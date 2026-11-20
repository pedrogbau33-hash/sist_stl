#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <string>

#include "Arquivo.hpp"
#include "Estante.hpp"

class Interface {
public:
    Interface(Estante& estante, Arquivo& arquivo);

    void executar();

private:
    Estante& estante;
    Arquivo& arquivo;

    bool carregarDados();
    void salvarDados();
    void menu();

    int lerOpcao();
    std::string lerLinha(const std::string& rotulo);
    void imprimir(const Livro& livro);

    void cadastrar();
    void listar();
    void remover();
    void alterarStatus();
    void avaliar();
    void listarPorAutor();
    void mostrarGeneros();
    void mostrarAvaliados();
};

#endif
