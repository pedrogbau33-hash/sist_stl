#include <iostream>
#include <string>
#include <vector>
#include <limits>

#include "Estante.hpp"
#include "Texto.hpp"

static int lerOpcao() {
    int opcao;

    if (std::cin >> opcao) {
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(), '\n'
        );
        return opcao;
    }

    if (std::cin.eof()) {
        return 0;
    }

    std::cin.clear();
    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(), '\n'
    );

    return -1;
}

static std::string lerLinha(const std::string& rotulo) {
    std::cout << rotulo;
    std::string valor;
    std::getline(std::cin, valor);
    return valor;
}

static void imprimir(const Livro& livro) {
    std::cout << livro.titulo() << " | " << livro.autor() << " | " << livro.genero()
              << " | " << (livro.lido() ? "lido" : "por ler")
              << " | nota: " << (livro.temNota() ? std::to_string(livro.nota()) : std::string("-"))
              << " | comentario: " << (livro.comentario().empty() ? std::string("-") : livro.comentario())
              << "\n";
}

static void cadastrar(Estante& estante) {
    std::string titulo = lerLinha("Titulo: ");
    std::string autor = lerLinha("Autor: ");
    std::string genero = lerLinha("Genero: ");

    Resultado resultado = estante.cadastrar(titulo, autor, genero);
    if (resultado == OK) {
        std::cout << "Cadastro realizado.\n";
    } else if (resultado == CAMPO_VAZIO) {
        std::cout << "Preencha todos os campos obrigatorios.\n";
    } else if (resultado == JA_EXISTE) {
        std::cout << "Parece que voce ja cadastrou esse livro.\n";
    }
}

static void listar(const Estante& estante) {
    if (estante.vazia()) {
        std::cout << "A estante esta vazia.\n";
        return;
    }
    std::vector<Livro> livros = estante.listar();
    for (std::size_t i = 0; i < livros.size(); ++i) {
        std::cout << (i + 1) << ") ";
        imprimir(livros[i]);
    }
}

static void remover(Estante& estante) {
    std::string titulo = lerLinha("Titulo: ");
    Resultado resultado = estante.remover(titulo);
    if (resultado == OK) {
        std::cout << "Livro removido.\n";
    } else if (resultado == ESTANTE_VAZIA) {
        std::cout << "A estante esta vazia.\n";
    } else if (resultado == NAO_ENCONTRADO) {
        std::cout << "Parece que esse livro nao existe.\n";
    }
}

static void alterarStatus(Estante& estante) {
    std::string titulo = lerLinha("Titulo: ");
    std::string status = lerLinha("Novo status (lido / por ler): ");

    std::string chave = Texto::normalizar(status);
    bool lido;
    if (chave == "lido") {
        lido = true;
    } else if (chave == "por ler") {
        lido = false;
    } else {
        std::cout << "Essa opcao nao existe.\n";
        return;
    }

    Resultado resultado = estante.definirStatus(titulo, lido);
    if (resultado == OK) {
        std::cout << "Status atualizado.\n";
    } else if (resultado == ESTANTE_VAZIA) {
        std::cout << "A estante esta vazia.\n";
    } else if (resultado == NAO_ENCONTRADO) {
        std::cout << "Parece que esse livro nao existe.\n";
    }
}

int main() {
    Estante estante;
    bool executando = true;

    while (executando) {
        std::cout << "\n1 - Cadastrar livro\n2 - Listar livros\n3 - Remover livro\n4 - Alterar status de leitura\n0 - Sair\n";
        int opcao = lerOpcao();

        if (opcao == 1) {
            cadastrar(estante);
        } else if (opcao == 2) {
            listar(estante);
        } else if (opcao == 3) {
            remover(estante);
        } else if (opcao == 4) {
            alterarStatus(estante);
        } else if (opcao == 0) {
            executando = false;
        } else {
            std::cout << "Essa opcao nao existe.\n";
        }
    }

    return 0;
}
