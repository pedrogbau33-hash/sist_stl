#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <string>

#include "Estante.hpp"

class Interface {
public:
    explicit Interface(Estante& estante);

    void executar();

private:
    Estante& estante;

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
