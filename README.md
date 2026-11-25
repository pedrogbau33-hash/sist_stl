# Estante de Leituras

Atividade da Unidade 3 de Linguagem de Programação I. Leia `ENUNCIADO.md` antes de começar.

Sistema em C++11 que registra livros lidos e por ler, com nota e comentário, e oferece consultas: livros de um autor, gêneros mais lidos e livros mais bem avaliados. A descrição completa está em `DESCRICAO.md`.

## Arquivos

```text
.
├── AGENTS.md       regras que a IA lê antes de cada pedido
├── DESCRICAO.md    descrição do sistema
├── ENUNCIADO.md    enunciado da atividade
├── PLANO.md        plano de implementação
├── README.md       este arquivo
├── exemplos/       arquivos de entrada para testar
├── include/        arquivos .hpp das classes
└── src/            arquivos .cpp das classes e da interface
```

## Compilar e executar

Para compilar o programa:

```bash
g++ -std=c++11 -Wall -Wextra src/*.cpp -Iinclude -o sistema
```

Para executar:

```bash
./sistema
```

O programa salva os dados em `estante.txt`, no diretório principal do projeto. Esse arquivo é criado automaticamente durante a execução e mantém os livros cadastrados entre diferentes execuções.

### Executar os exemplos

Para testar cada exemplo a partir de uma estante vazia, remova o arquivo `estante.txt` antes de executar cada comando:

```bash
rm -f estante.txt
./sistema < exemplos/01_fluxo_principal.txt

rm -f estante.txt
./sistema < exemplos/02_campos_duplicado_inexistente.txt

rm -f estante.txt
./sistema < exemplos/03_leitura_e_avaliacao.txt

rm -f estante.txt
./sistema < exemplos/04_consultas_sem_resultado.txt

rm -f estante.txt
./sistema < exemplos/05_entradas_invalidas.txt
```

Se os comandos de compilação ou execução mudarem, esta seção deverá ser atualizada.

## Evolução

### Unidade 3 — Persistência em arquivos

Nesta etapa, implementei a persistência dos livros em um arquivo de texto, permitindo que os dados cadastrados continuem disponíveis mesmo depois que o programa é encerrado. Para isso, criei a classe `Arquivo`, responsável exclusivamente por salvar e carregar os dados. Também implementei validações para identificar registros inválidos, tratamento de erros por meio de exceções e cuidados para evitar que um arquivo com dados incorretos carregue parcialmente a estante. Por fim, realizei testes para verificar a gravação, a recuperação dos dados e o comportamento do programa diante de arquivos inexistentes ou informações inválidas.

Durante a revisão, identifiquei um problema no salvamento: os dados originais poderiam ser perdidos caso ocorresse uma falha durante a gravação. Para reduzir esse risco, implementei o uso de um arquivo temporário e de um backup, permitindo tentar restaurar os dados anteriores caso a substituição do arquivo principal falhe. Também identifiquei que o carregamento não verificava títulos duplicados. Para resolver isso, acrescentei uma validação que compara os títulos normalizados e impede que registros repetidos sejam carregados.

## Onde uso a STL

| Onde | O quê | Por quê |
| --- | --- | --- |
| `Estante` (campo `livros`) | `std::list<Livro>` | Armazena os livros na ordem de cadastro. Permite inserir e remover elementos sem invalidar os iteradores dos demais elementos. Não oferecendo acesso direto por índice.|
| `Estante::buscar` | `std::find` sobre a `list`, usando `Livro::operator==` | Localiza um livro pelo título normalizado. A busca linear é suficiente para o tamanho esperado da estante. |
| `Estante::generosMaisLidos` | `std::map<std::string, GeneroEstatistica>` | Agrupa os gêneros pela chave normalizada, mantendo a grafia e a quantidade de livros lidos. Busca e inserção em O(log n). |
| `Estante` (`listar`, `livrosDoAutor`, `livrosMaisBemAvaliados`, `generosMaisLidos`) | `std::vector<Livro>` / `std::vector<GeneroEstatistica>` | Armazena cópias dos resultados das consultas, facilitando a exibição e a ordenação. |
| `Estante::livrosMaisBemAvaliados` | `std::sort` sobre `std::vector<Livro>` | Ordena os livros usando `Livro::operator<`, colocando as maiores notas primeiro e utilizando a ordem de cadastro como critério de desempate. |
| `Livro` | `Livro::operator==` | Compara os títulos normalizados e permite localizar livros com `std::find`. |
| `Livro` | `Livro::operator<` | Define a ordem dos livros por avaliação e é utilizado pelo `std::sort`. |
| `Arquivo::carregar` | `std::vector<Livro>` | Armazena temporariamente os livros lidos do arquivo, permitindo atualizar a estante somente depois que todos os registros forem validados. |
| `Arquivo::carregar` | `std::set<int>` | Armazena as ordens de cadastro encontradas e permite identificar valores duplicados. |
| `Arquivo::carregar` | `std::set<std::string>` | Armazena os títulos normalizados para identificar livros com títulos duplicados. |

## Como usei a IA

- **Modelo usado:** OpenCode DeepSeek V4.1.
- **Um pedido que deu certo, e por que você acha que deu certo:** Solicitei um plano de implementação antes de começar a programar. O arquivo `PLANO.md` foi commitado antes das alterações no código (`1dd8fab`), e cada etapa foi desenvolvida separadamente. Isso ajudou a organizar o trabalho, compreender as mudanças necessárias e verificar cada parte antes de prosseguir.
- **Um pedido que deu errado, o que veio errado e como você mudou o pedido:** Durante a implementação, a IA modificou partes do README que deveriam ser preservadas, como a seção sobre o uso da STL. Para corrigir o problema, consultei o histórico do Git, restaurei o conteúdo original e passei a fazer pedidos mais específicos, indicando quais arquivos e trechos poderiam ser alterados.
- **Uma coisa que a IA sugeriu e você recusou, e por quê:** Recusei a ideia de exigir que todo livro com nota tivesse um comentário e vice-versa. Considerei essa regra limitante e pouco prática, pois o usuário deve poder atribuir uma nota sem escrever um comentário, escrever um comentário sem atribuir uma nota ou fornecer ambos.
