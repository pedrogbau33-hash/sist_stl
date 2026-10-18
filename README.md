# Estante de Leituras

Atividade da Unidade 2 de Linguagem de Programação I. Leia `ENUNCIADO.md`
antes de começar.

Sistema em C++11 que registra livros lidos e por ler, com nota e comentário,
e oferece consultas: livros de um autor, gêneros mais lidos e livros mais bem
avaliados. A descrição completa está em `DESCRICAO.md`.

## Arquivos

```text
.
├── AGENTS.md       regras que a IA lê antes de cada pedido
├── DESCRICAO.md    a descrição do sistema, escrita por você
├── ENUNCIADO.md    a atividade
├── PLANO.md        o plano feito com a IA (criado na etapa 4)
├── README.md       este arquivo
├── exemplos/       arquivos de entrada para testar
├── include/        um .hpp por classe
└── src/            um .cpp por classe, mais a interface
```

## Compilar e executar

```bash
g++ -std=c++11 -Wall -Wextra src/*.cpp -Iinclude -o sistema
./sistema
```

Para rodar os exemplos:

```bash
./sistema < exemplos/01_fluxo_principal.txt
./sistema < exemplos/02_campos_duplicado_inexistente.txt
./sistema < exemplos/03_leitura_e_avaliacao.txt
./sistema < exemplos/04_consultas_sem_resultado.txt
./sistema < exemplos/05_entradas_invalidas.txt
```

Se o comando mudar, atualize esta seção. Na correção eu vou usar o que
estiver escrito aqui.

## Onde uso a STL

Escreva esta seção sem IA. Para cada container ou algoritmo da STL que o
sistema usa: em que classe ele está, o que guarda, e por que você escolheu
ele e não outro.

| Onde | O quê | Por quê |
| --- | --- | --- |
| `Estante` (campo `livros`) | `std::list<Livro>` | guarda os livros na ordem de cadastro; insiro no fim e removo no meio mantendo a ordem, e os demais iteradores continuam válidos |
| `Estante::buscar` | `std::find` sobre a `list`, usando `Livro::operator==` (título normalizado) | localiza um livro pelo título; O(n) é aceitável e evita manter estrutura duplicada |
| `Estante::generosMaisLidos` | `std::map<std::string, GeneroEstatistica>` | chave = gênero normalizado → grafia + quantidade de lidos; agrupa grafias iguais, busca/inserção O(log n) |
| `Estante` (`listar`, `livrosDoAutor`, `livrosMaisBemAvaliados`, `generosMaisLidos`) | `std::vector<Livro>` / `std::vector<GeneroEstatistica>` | devolve cópias para exibir/ordenar; armazenamento contíguo e fácil de ordenar |
| `Estante::livrosMaisBemAvaliados` | `std::sort` sobre `std::vector<Livro>` | ordena com `Livro::operator<` (nota decrescente; empate = mais recente) |
| `Livro` | `Livro::operator==` | compara título normalizado; usado pelo `std::find` |
| `Livro` | `Livro::operator<` | "melhor avaliado primeiro"; usado pelo `std::sort` |

## Como usei a IA

Escreva com as suas palavras:

- Modelo usado: <PREENCHER: modelo/ferramenta usada>
- Um pedido que deu certo, e por que você acha que deu certo: o `PLANO.md` foi
  commitado antes de qualquer código (`1dd8fab`) e cada etapa virou um commit
  separado (`e458f6f`…`c55fa0d`). <PREENCHER: por que você acha que deu certo>
- Um pedido que deu errado, o que veio errado e como você mudou o pedido:
  <PREENCHER: não verificável no histórico>
- Uma coisa que a IA sugeriu e você recusou, e por quê: <PREENCHER>
