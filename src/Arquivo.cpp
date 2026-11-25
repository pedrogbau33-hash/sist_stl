#include "Arquivo.hpp"

#include <cerrno>
#include <cstdio>
#include <fstream>
#include <limits>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

#include "Texto.hpp"

static bool interpretarInteiro(const std::string& texto, int& valor) {
    if (texto.empty()) {
        return false;
    }

    std::size_t i = 0;
    bool negativo = false;
    if (texto[i] == '-') {
        negativo = true;
        ++i;
        if (i == texto.size()) {
            return false;
        }
    }

    int acumulado = 0;
    const int limite = std::numeric_limits<int>::max();

    for (; i < texto.size(); ++i) {
        char c = texto[i];
        if (c < '0' || c > '9') {
            return false;
        }
        int digito = c - '0';
        if (acumulado > (limite - digito) / 10) {
            return false;
        }
        acumulado = acumulado * 10 + digito;
    }

    valor = negativo ? -acumulado : acumulado;
    return true;
}

static bool verificarExistencia(const std::string& nome, bool& existe) {
    errno = 0;
    std::FILE* arquivo = std::fopen(nome.c_str(), "rb");

    if (arquivo != NULL) {
        std::fclose(arquivo);
        existe = true;
        return true;
    }

    if (errno == ENOENT) {
        existe = false;
        return true;
    }

    return false;
}

static bool removerSeExistir(const std::string& nome) {
    errno = 0;
    if (std::remove(nome.c_str()) == 0) {
        return true;
    }

    return errno == ENOENT;
}

static void fecharERemoverTemporario(std::ofstream& saida, const std::string& temporario, std::string& aviso) {
    if (saida.is_open()) {
        saida.close();
    }

    if (!removerSeExistir(temporario)) {
        aviso = " Nao foi possivel remover o arquivo temporario \"" + temporario + "\".";
    }
}

Arquivo::Arquivo(const std::string& nomeArquivo)
    : nomeArquivo_(nomeArquivo) {
}

std::string Arquivo::escapar(const std::string& texto) {
    std::string resultado;

    for (std::size_t i = 0; i < texto.size(); ++i) {
        char c = texto[i];
        if (c == '\\' || c == ';') {
            resultado += '\\';
        }
        resultado += c;
    }

    return resultado;
}

bool Arquivo::desescapar(const std::string& texto, std::string& resultado) {
    resultado.clear();

    for (std::size_t i = 0; i < texto.size(); ++i) {
        char c = texto[i];
        if (c == '\\') {
            ++i;
            if (i >= texto.size()) {
                return false;
            }
            char escapado = texto[i];
            if (escapado != '\\' && escapado != ';') {
                return false;
            }
            resultado += escapado;
        } else {
            resultado += c;
        }
    }

    return true;
}

bool Arquivo::dividir(const std::string& linha, std::vector<std::string>& campos) {
    campos.clear();
    std::string atual;

    for (std::size_t i = 0; i < linha.size(); ++i) {
        char c = linha[i];
        if (c == '\\') {
            atual += c;
            ++i;
            if (i >= linha.size()) {
                return false;
            }
            atual += linha[i];
        } else if (c == ';') {
            campos.push_back(atual);
            atual.clear();
        } else {
            atual += c;
        }
    }

    campos.push_back(atual);
    return true;
}

bool Arquivo::livroDaLinha(const std::string& linha, Livro& livro, std::string& motivo) {
    std::vector<std::string> campos;

    if (!dividir(linha, campos)) {
        motivo = "escape incompleto";
        return false;
    }
    if (campos.size() != 8) {
        motivo = "esperados 8 campos";
        return false;
    }

    std::string titulo;
    std::string autor;
    std::string genero;
    std::string comentario;

    if (!desescapar(campos[0], titulo)) {
        motivo = "escape invalido no titulo";
        return false;
    }
    if (!desescapar(campos[1], autor)) {
        motivo = "escape invalido no autor";
        return false;
    }
    if (!desescapar(campos[2], genero)) {
        motivo = "escape invalido no genero";
        return false;
    }
    if (!desescapar(campos[6], comentario)) {
        motivo = "escape invalido no comentario";
        return false;
    }

    bool lido;
    if (campos[3] == "lido") {
        lido = true;
    } else if (campos[3] == "por ler") {
        lido = false;
    } else {
        motivo = "status invalido";
        return false;
    }

    int temNotaValor;
    if (!interpretarInteiro(campos[4], temNotaValor) || (temNotaValor != 0 && temNotaValor != 1)) {
        motivo = "temNota invalido";
        return false;
    }
    bool temNota = temNotaValor == 1;

    int nota;
    if (!interpretarInteiro(campos[5], nota)) {
        motivo = "nota invalida";
        return false;
    }
    if (temNota) {
        if (nota < 0 || nota > 10) {
            motivo = "nota fora de 0..10";
            return false;
        }
    } else if (nota != -1) {
        motivo = "nota deve ser -1 quando nao existe";
        return false;
    }

    int ordem;
    if (!interpretarInteiro(campos[7], ordem) || ordem <= 0) {
        motivo = "ordem invalida";
        return false;
    }

    if (Texto::aparar(titulo).empty()) {
        motivo = "titulo vazio";
        return false;
    }
    if (Texto::aparar(autor).empty()) {
        motivo = "autor vazio";
        return false;
    }
    if (Texto::aparar(genero).empty()) {
        motivo = "genero vazio";
        return false;
    }
    if (!lido && temNota) {
        motivo = "livro por ler nao pode ter nota";
        return false;
    }
    if (!lido && !comentario.empty()) {
        motivo = "livro por ler nao pode ter comentario";
        return false;
    }

    livro = Livro(titulo, autor, genero, ordem);
    livro.definirLido(lido);
    if (lido && temNota) {
        livro.definirNota(nota);
    }
    if (lido && !comentario.empty()) {
        livro.definirComentario(comentario);
    }

    return true;
}

std::string Arquivo::linhaDoLivro(const Livro& livro) {
    std::string linha;

    linha += escapar(livro.titulo());
    linha += ';';
    linha += escapar(livro.autor());
    linha += ';';
    linha += escapar(livro.genero());
    linha += ';';
    linha += livro.lido() ? "lido" : "por ler";
    linha += ';';
    linha += livro.temNota() ? '1' : '0';
    linha += ';';
    linha += std::to_string(livro.temNota() ? livro.nota() : -1);
    linha += ';';
    linha += escapar(livro.comentario());
    linha += ';';
    linha += std::to_string(livro.ordem());

    return linha;
}

void Arquivo::carregar(std::vector<Livro>& livros) const {
    errno = 0;
    std::FILE* teste = std::fopen(nomeArquivo_.c_str(), "r");

    if (teste == NULL) {
        if (errno == ENOENT) {
            livros.clear();
            return;
        }
        throw std::runtime_error("Nao foi possivel abrir o arquivo \"" + nomeArquivo_ + "\" para leitura.");
    }

    std::fclose(teste);

    std::ifstream entrada(nomeArquivo_.c_str());
    if (!entrada.is_open()) {
        throw std::runtime_error("Nao foi possivel abrir o arquivo \"" + nomeArquivo_ + "\" para leitura.");
    }

    std::vector<Livro> lidos;
    std::set<int> ordensVistas;
    std::set<std::string> titulosVistos;
    std::string linha;
    int numeroLinha = 0;

    while (std::getline(entrada, linha)) {
        ++numeroLinha;

        if (!linha.empty() && linha[linha.size() - 1] == '\r') {
            linha.erase(linha.size() - 1);
        }

        Livro livro("", "", "", 0);
        std::string motivo;
        if (!livroDaLinha(linha, livro, motivo)) {
            throw std::runtime_error(
                "Arquivo \"" + nomeArquivo_ + "\", linha " + std::to_string(numeroLinha) + ": " + motivo + "."
            );
        }

        if (!ordensVistas.insert(livro.ordem()).second) {
            throw std::runtime_error(
                "Arquivo \"" + nomeArquivo_ + "\", linha " + std::to_string(numeroLinha) +
                ": ordem de cadastro " + std::to_string(livro.ordem()) + " duplicada."
            );
        }

        const std::string tituloNormalizado = Texto::normalizar(livro.titulo());
        if (!titulosVistos.insert(tituloNormalizado).second) {
            throw std::runtime_error(
                "Arquivo \"" + nomeArquivo_ + "\", linha " + std::to_string(numeroLinha) +
                ": titulo duplicado: \"" + livro.titulo() + "\"."
            );
        }

        lidos.push_back(livro);
    }

    if (entrada.bad()) {
        throw std::runtime_error("Falha de leitura no arquivo \"" + nomeArquivo_ + "\".");
    }

    livros.swap(lidos);
}

void Arquivo::salvar(const std::vector<Livro>& livros) const {
    const std::string temporario = nomeArquivo_ + ".tmp";
    const std::string backup = nomeArquivo_ + ".bak";

    bool backupExiste = false;
    if (!verificarExistencia(backup, backupExiste)) {
        throw std::runtime_error("Nao foi possivel verificar o arquivo de backup \"" + backup + "\".");
    }
    if (backupExiste) {
        throw std::runtime_error("O arquivo de backup \"" + backup + "\" ja existe. Verifique a recuperacao antes de salvar.");
    }

    bool temporarioExiste = false;
    if (!verificarExistencia(temporario, temporarioExiste)) {
        throw std::runtime_error("Nao foi possivel verificar o arquivo temporario \"" + temporario + "\".");
    }
    if (temporarioExiste && !removerSeExistir(temporario)) {
        throw std::runtime_error("Nao foi possivel remover o arquivo temporario antigo \"" + temporario + "\".");
    }

    std::ofstream saida(temporario.c_str(), std::ios::out | std::ios::trunc);
    if (!saida.is_open()) {
        throw std::runtime_error("Nao foi possivel abrir o arquivo temporario \"" + temporario + "\" para escrita. O arquivo original nao foi alterado.");
    }

    for (std::size_t i = 0; i < livros.size(); ++i) {
        saida << linhaDoLivro(livros[i]) << '\n';
        if (!saida) {
            std::string aviso;
            fecharERemoverTemporario(saida, temporario, aviso);
            throw std::runtime_error("Falha ao escrever no arquivo temporario \"" + temporario + "\". O arquivo original nao foi alterado." + aviso);
        }
    }

    saida.flush();
    if (!saida) {
        std::string aviso;
        fecharERemoverTemporario(saida, temporario, aviso);
        throw std::runtime_error("Falha ao finalizar a escrita no arquivo temporario \"" + temporario + "\". O arquivo original nao foi alterado." + aviso);
    }

    saida.close();
    if (saida.fail()) {
        std::string aviso;
        fecharERemoverTemporario(saida, temporario, aviso);
        throw std::runtime_error("Falha ao fechar o arquivo temporario \"" + temporario + "\". O arquivo original nao foi alterado." + aviso);
    }

    bool tinhaOriginal = false;
    if (!verificarExistencia(nomeArquivo_, tinhaOriginal)) {
        std::string aviso;
        if (!removerSeExistir(temporario)) {
            aviso = " Nao foi possivel remover o arquivo temporario \"" + temporario + "\".";
        }
        throw std::runtime_error("Nao foi possivel verificar o arquivo \"" + nomeArquivo_ + "\". O arquivo original nao foi alterado." + aviso);
    }

    if (tinhaOriginal && std::rename(nomeArquivo_.c_str(), backup.c_str()) != 0) {
        std::string aviso;
        if (!removerSeExistir(temporario)) {
            aviso = " Nao foi possivel remover o arquivo temporario \"" + temporario + "\".";
        }
        throw std::runtime_error("Nao foi possivel mover o arquivo original \"" + nomeArquivo_ + "\" para o backup \"" + backup + "\". O arquivo original nao foi alterado." + aviso);
    }

    if (std::rename(temporario.c_str(), nomeArquivo_.c_str()) != 0) {
        bool restaurado = false;
        if (tinhaOriginal && std::rename(backup.c_str(), nomeArquivo_.c_str()) == 0) {
            restaurado = true;
        }

        if (restaurado) {
            std::string aviso;
            if (!removerSeExistir(temporario)) {
                aviso = " Nao foi possivel remover o arquivo temporario \"" + temporario + "\".";
            }
            throw std::runtime_error("Falha ao substituir o arquivo \"" + nomeArquivo_ + "\" pelo temporario. O arquivo original foi restaurado." + aviso);
        }

        if (tinhaOriginal) {
            throw std::runtime_error("Falha ao substituir o arquivo \"" + nomeArquivo_ + "\" pelo temporario e falha ao restaurar o original. Backup em \"" + backup + "\" e novo arquivo em \"" + temporario + "\".");
        }

        std::string aviso;
        if (!removerSeExistir(temporario)) {
            aviso = " Nao foi possivel remover o arquivo temporario \"" + temporario + "\".";
        }
        throw std::runtime_error("Falha ao substituir o arquivo \"" + nomeArquivo_ + "\" pelo temporario. O arquivo original nao existia e continua ausente." + aviso);
    }

    if (tinhaOriginal && !removerSeExistir(backup)) {
        throw std::runtime_error("Dados salvos em \"" + nomeArquivo_ + "\", mas nao foi possivel remover o backup \"" + backup + "\". O backup precisa ser removido manualmente.");
    }
}
