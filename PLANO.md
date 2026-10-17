# Plano de implementação — Estante de Leituras

Este plano segue `DESCRICAO.md` e as regras de `AGENTS.md`. C++11, só a
biblioteca padrão, uma classe por par `include/*.hpp` e `src/*.cpp`, lógica
separada da interface (sem `cin`/`cout` fora da interface).

## 1. Classes

### `Texto` (auxiliar) — `include/Texto.hpp`, `src/Texto.cpp`
Classe só com métodos `static`, sem estado. Evita repetir o mesmo código em
vários lugares.
- `static std::string aparar(const std::string&)` — remove espaços no começo
  e no fim.
- `static std::string normalizar(const std::string&)` — `aparar` + deixa em
  minúsculas (para comparar títulos, autores e gêneros ignorando
  maiúsculas/minúsculas). A conversão é **só ASCII** (A–Z ↔ a–z); bytes de
  caracteres acentuados/multibyte são preservados sem alteração. O motivo e o
  alcance estão detalhados em **4.4**.

> Alternativa mais simples: colocar esses dois métodos como `static` dentro
> de `Livro` e não criar `Texto`. Fica menos código, mas mistura "texto" com
> "livro". Posso trocar se você preferir.

### `Livro` — `include/Livro.hpp`, `src/Livro.cpp`
Guarda os dados de um livro.

Campos:
- `std::string titulo`, `autor`, `genero` (já aparados; espaços internos
  preservados).
- `bool lido` (começa `false` = "por ler").
- `bool temNota` + `int nota` (0 a 10; separado porque a nota pode estar
  vazia).
- `std::string comentario` (vazio = sem comentário).
- `int ordem` (número de cadastro, crescente; usado no desempate "mais
  recente primeiro").

Métodos:
- construtor `Livro(titulo, autor, genero, ordem)`.
- getters: `titulo()`, `autor()`, `genero()`, `lido()`, `temNota()`,
  `nota()`, `comentario()`, `ordem()`.
- `definirLido(bool)`, `definirNota(int)`, `removerNota()`,
  `definirComentario(string)`, `removerComentario()`.
- `bool operator==(const Livro&) const` — compara **título normalizado**
  (regra de não duplicar/localizar título). Usado pelo `std::find`.
- `bool operator<(const Livro&) const` — ordem "melhor avaliado primeiro"
  (nota decrescente; empate: `ordem` maior primeiro). Usado pelo `std::sort`.

### `Estante` — `include/Estante.hpp`, `src/Estante.cpp`
Guarda a coleção e aplica as regras; não imprime nada.

Campos:
- `std::list<Livro> livros` — os livros na ordem de cadastro.
- `int proximaOrdem` — contador para o `ordem` de cada livro novo.

Resultados: uma operação pode falhar por motivos diferentes, cada um com uma
mensagem própria. Por isso os métodos devolvem um `enum Resultado` em vez de
`bool`:
`OK, NAO_ENCONTRADO, JA_EXISTE, CAMPO_VAZIO, NAO_LIDO, NOTA_INVALIDA,
ESTANTE_VAZIA, SEM_LIDOS, SEM_AVALIADOS`.

Métodos:
- `Resultado cadastrar(titulo, autor, genero)` — apara, checa vazio, checa
  duplicado, cria como "por ler". **Nunca** devolve `ESTANTE_VAZIA`: é a
  operação que insere o primeiro livro.
- `Resultado definirStatus(titulo, bool lido)` — se voltar para "por ler",
  apaga nota e comentário.
- `Resultado avaliar(titulo, entradaNota, entradaComentario)` — recebe as
  entradas cruas e **valida tudo antes de alterar o livro**: primeiro localiza
  o livro e checa o estado (`ESTANTE_VAZIA`, `NAO_ENCONTRADO`, `NAO_LIDO`),
  depois interpreta e valida as duas entradas (`"-"` mantém, vazio remove,
  valor válido substitui; nota fora de 0–10 é `NOTA_INVALIDA`). Só se não
  houver nenhum erro aplica as mudanças; caso contrário devolve o erro sem
  tocar em nota nem comentário (ver **4.3**).
- `Resultado remover(titulo)`.
- `bool vazia()`, `bool temLidos()`, `bool temAvaliados()` — para a interface
  escolher a mensagem certa das consultas.

`ESTANTE_VAZIA` não é uma trava geral: só é devolvido pelas operações e
consultas em que a estante vazia exige a mensagem "A estante esta vazia."
(`definirStatus`, `avaliar`, `remover` e as consultas). As operações que
procuram um livro existente checam a estante vazia **antes** de procurar o
título; `cadastrar` não entra nessa lista.
- `std::vector<Livro> listar() const` (ordem de cadastro).
- `std::vector<Livro> livrosDoAutor(autor) const`.
- `std::vector<GeneroEstatistica> generosMaisLidos() const` — com
  `struct GeneroEstatistica { std::string nome; int quantidade; }` no
  `Estante.hpp`.
- `std::vector<Livro> livrosMaisBemAvaliados() const`.
- privado: `buscar(titulo)` (com `std::find`).

### `Interface` — `include/Interface.hpp`, `src/Interface.cpp`
Único lugar com `cin`/`cout`. Guarda uma referência para a `Estante`.
- `explicit Interface(Estante&)`.
- `void executar()` — laço com menu numerado, lê a opção (tratando entrada
  não numérica), chama um método privado por opção e imprime as mensagens.
- privado: um método por opção + um auxiliar para ler `int` com segurança.

`src/main.cpp` — só cria `Estante` e `Interface` e chama `executar()`.

## 2. Onde uso a STL

| Onde (classe) | O quê | Guarda | Por quê |
| --- | --- | --- | --- |
| `Estante` | `std::list<Livro>` | os livros na ordem de cadastro | insiro no fim e removo no meio mantendo a ordem; ao apagar um item, os demais iteradores continuam válidos |
| `Estante` (`generosMaisLidos`) | `std::map<std::string, GeneroEstatistica>` | por gênero normalizado: a grafia a exibir e a quantidade de livros lidos | associa chave→valor e agrupa grafias iguais sob a mesma chave; buscar/inserir é O(log n) |
| `Estante` (`buscar`) | `std::find` | procura um `Livro` pelo título | usa `Livro::operator==`, que compara título normalizado |
| `Estante` (`livrosMaisBemAvaliados`) | `std::sort` | ordena um `std::vector<Livro>` | usa `Livro::operator<` (nota decrescente, empate mais recente) |
| `Estante` (retornos) | `std::vector<Livro>` / `std::vector<GeneroEstatistica>` | cópias para exibir/ordenar | armazenamento contíguo e fácil de ordenar |

Observação (confirmado): o título é localizado com `std::find` (O(n)) em vez
de `map`. Como a `list` já guarda a ordem de cadastro exigida e a estante tem
poucos livros, o O(n) é aceitável e evita manter duas estruturas para
localizar títulos. O `map` fica onde ele é mais natural: a contagem por
gênero.

## 3. Decisões confirmadas

1. **Mensagens de sucesso** (o resto é mensagem de erro, exatamente como na
   `DESCRICAO.md`):
   - cadastro → `"Cadastro realizado."`
   - remoção → `"Livro removido."`
   - alteração de status → `"Status atualizado."`
   - edição de nota/comentário → `"Avaliacao atualizada."` (uma única
     mensagem, impressa quando o resultado é `OK`, sem depender de qual campo
     mudou — funciona com só a nota, só o comentário, ou os dois).
2. **`enum Resultado` em vez de `bool`**, porque uma operação pode falhar por
   vários motivos, cada um com mensagem diferente. A `Estante` só devolve o
   resultado (sem imprimir nada); a `Interface` traduz cada valor na mensagem
   correspondente.
3. **`std::find` na `std::list<Livro>`** para localizar livros. O O(n) é
   aceitável e evita estruturas duplicadas para achar títulos.
4. Quem valida campo obrigatório vazio é a `Estante` (é regra do sistema); a
   interface só imprime a mensagem.
5. `ESTANTE_VAZIA` (mensagem "A estante esta vazia.") é devolvido apenas
   pelas operações e consultas em que a estante vazia exige essa mensagem.
   As operações que procuram um livro existente (`definirStatus`, `avaliar`,
   `remover`) checam a estante vazia **antes** de procurar o título. O
   `cadastrar` **não** devolve `ESTANTE_VAZIA`, pois é justamente a operação
   que insere o primeiro livro.
6. Sobre `"-"` no comentário: `"-"` sempre significa "manter"; nunca é
   gravado. A edição distingue com clareza **manter** (`"-"`), **remover**
   (entrada vazia) e **substituir** (valor válido), campo a campo, e não
   aplica alteração parcial: valida as duas entradas antes e só então grava
   (ver **4.3**). O único campo que pode ser inválido é a nota.
7. **`Texto::normalizar` é ASCII-only**: compara A–Z com a–z e trata espaços
   apenas no começo/fim; letras acentuadas (multibyte UTF-8) são comparadas
   byte a byte, sem equivalência de caixa. Motivo em **4.4**.

## 4. Detalhes de implementação

### 4.1 Ordenar os mais bem avaliados (`std::sort` sobre `std::vector<Livro>`)

`std::sort` exige iteradores de acesso aleatório. O `std::list` só oferece
iteradores bidirecionais, então `std::sort` **não** pode ser aplicada
diretamente à lista (para isso existiria o `list::sort`, que é outra
ferramenta). Por isso copiamos para um `std::vector<Livro>`:

1. Percorrer `livros` do começo ao fim (a `list` já está na ordem de
   cadastro) e, para cada livro `lido()` e `temNota()`, fazer
   `v.push_back(*it)`. O `push_back` **copia** o `Livro`; a cópia padrão
   funciona porque os campos são `std::string`, `int` e `bool`.
2. `std::sort(v.begin(), v.end())` ordena o vetor usando `Livro::operator<`.

`operator<` (no `Livro.cpp`):

```cpp
bool Livro::operator<(const Livro& outro) const {
    if (nota_ != outro.nota_) {
        return nota_ > outro.nota_;   // maior nota primeiro
    }
    return ordem_ > outro.ordem_;     // empate: cadastro mais recente primeiro
}
```

Como `std::sort` ordena de forma crescente segundo `operator<`, definimos
`a < b` como "a deve vir antes de b". Assim o primeiro elemento do vetor é o
de **maior nota**. No empate de nota, quem tem `ordem_` maior (cadastrado
mais recentemente) vem antes. A comparação é uma ordem estrita fraca válida:
primeiro pela nota e, no empate, por `ordem_`, que é única por livro. Um
livro sem nota é filtrado no passo 1; ainda assim `operator<` funciona, pois
`nota_` vale -1 quando não há nota.

### 4.2 Gêneros mais lidos (`std::map` com chave normalizada, sem perder a grafia)

Ideia: usar um `std::map<std::string, GeneroEstatistica>` em que a **chave**
é o gênero normalizado (agrupa grafias diferentes) e o **valor** guarda a
grafia a exibir (`nome`) e a quantidade de livros **lidos** (`quantidade`).

São duas coisas distintas — a grafia vem de **todos** os livros, a contagem
só dos **lidos** — então o algoritmo trata cada uma no seu momento, numa única
passada pelos livros na ordem de cadastro (do mais antigo para o mais
recente):

1. Percorrer **todos** os livros, independentemente do status.
2. Para cada livro, `chave = Texto::normalizar(livro.genero())`.
   - Se a chave ainda não existe no map, inserir
     `{nome = livro.genero(), quantidade = 0}`. Como estamos indo do mais
     antigo para o mais novo, a primeira vez que um gênero aparece é o
     **livro mais antigo ainda cadastrado daquele gênero** — é dele que vem
     a grafia exibida, **mesmo que esse livro não esteja lido**.
   - Se a chave já existe, **não** alterar `nome`.
3. Logo em seguida, se `livro.lido()`, incrementar `quantidade` daquela
   chave. Assim a contagem considera só os lidos (e só gêneros com pelo menos
   um livro lido terminam com `quantidade > 0`).
4. Copiar os valores do map para um `std::vector<GeneroEstatistica>`,
   **descartar** os de `quantidade == 0` (a descrição não mostra gêneros sem
   livros lidos) e ordenar com uma função de comparação (`quantidade`
   decrescente; empate pelo `nome` em ordem alfabética). Aqui o `std::sort`
   usa um comparador porque gêneros não têm `operator<`; o requisito de
   ordenar objetos de uma classe nossa já é atendido por `Livro::operator<`.

Ao remover o livro mais antigo de um gênero, a próxima consulta recalcula
tudo do zero e a grafia passa a ser a do próximo mais antigo ainda
cadastrado — exatamente a regra da descrição.

Descartado: ordenar direto o `map` pelo contador — o `map` ordena pela chave,
não pelo valor; e um `std::set` não guardaria a grafia nem a quantidade.

### 4.3 Avaliação de nota e comentário (sem alteração parcial)

As entradas cruas são interpretadas campo a campo: `"-"` = **manter**,
entrada **vazia** = **remover**, qualquer outro valor = **substituir**.

Fase 1 — validação, **sem tocar no livro**:
1. Estante vazia → `ESTANTE_VAZIA`.
2. `std::find` não encontra o título → `NAO_ENCONTRADO`.
3. Livro não está lido → `NAO_LIDO`.
4. Interpretar `entradaNota`: `"-"` mantém; vazia remove; senão converter para
   inteiro (texto não numérico é inválido) e exigir 0–10; qualquer falha →
   `NOTA_INVALIDA`.
5. Interpretar `entradaComentario`: `"-"` mantém; vazia remove; senão
   substitui (não existe comentário inválido, porque `"-"` nunca é gravado).
6. Guardar as decisões em variáveis locais (o que fazer com a nota e com o
   comentário), sem aplicá-las ainda.

Fase 2 — aplicação: **só se** a fase 1 terminou sem erro, aplicar as decisões
no livro. Se qualquer validação falhou, retornar o resultado de erro
imediatamente, sem chamar nenhum `setter` — assim **não há alteração
parcial**.

A ordem das checagens define a mensagem quando há mais de um problema:
vazio → não encontrado → não lido → nota inválida. A `Interface` não precisa
saber qual campo mudou: em `OK` ela imprime a única mensagem
`"Avaliacao atualizada."`, que já vale para só nota, só comentário ou os dois.

### 4.4 Normalização de texto (C++11 e UTF-8)

Em C++11, `std::string` é uma sequência de **bytes**; em UTF-8 um caractere
acentuado ocupa 2 ou mais bytes.

- `aparar` (trim): remove bytes de espaço **ASCII** no começo e no fim
  (`' '`, `'\t'`, `'\n'`, `'\r'`, `'\v'`, `'\f'`), detectados com
  `std::isspace` aplicado a um `unsigned char`. É seguro em UTF-8: os bytes de
  continuação têm valor `>= 0x80` e nunca são espaço ASCII, então o corte não
  parte um caractere multibyte.
- minúsculas: aplicar `std::tolower(static_cast<unsigned char>(c))` **apenas**
  aos bytes `< 0x80` (ASCII); os bytes `>= 0x80` são copiados sem alteração.
- **Por que não** passar todos os bytes em `std::tolower`: um caractere
  acentuado ocupa vários bytes e `std::tolower` age num único byte; no locale
  `"C"` o comportamento para bytes `>= 0x80` é indefinido e pode corromper a
  sequência UTF-8.
- **Alcance declarado**: a comparação ignora maiúsculas/minúsculas **só para
  letras ASCII** (A–Z). Caracteres acentuados e demais caracteres não ASCII
  são comparados byte a byte, sem equivalência de caixa. Portanto `"ç"` e
  `"Ç"` (ou `"ã"` e `"Ã"`) **não** coincidem, e `"Ficcao"` não é igual a
  `"Ficção"`. Na prática, títulos, autores e gêneros com acento precisam ser
  digitados sempre na mesma forma.
- Alternativas com apenas a biblioteca padrão (C++11), e por que não:
  - `std::toupper` com um `std::locale` configurado continua byte a byte e
    depende do locale instalado no sistema; não resolve acentos de UTF-8 de
    forma confiável.
  - `<codecvt>` / `std::wstring_convert` + `std::towupper` fariam conversão
    para wide e permitiriam mapear caixa, mas dependem de locale, são
    verbosos, e o `<codecvt>` foi depreciado no C++17 e removido no C++26 —
    frágil para um projeto didático.
  - Uma tabela de equivalências Unicode exigiria dados externos, o que não é
    "somente a biblioteca padrão".

Conclusão: mantemos a solução **ASCII-only**, compatível com a restrição de
usar só a biblioteca padrão, e **declaramos a limitação**. Sugestão (a
decisão é sua): registrar essa limitação na `DESCRICAO.md`.

## 5. Etapas (cada uma compila e dá para testar)

Comando de compilação (já está no `README.md`, cobre arquivos novos):
`g++ -std=c++11 -Wall -Wextra src/*.cpp -Iinclude -o sistema`

**Etapa 1 — Base de texto, `Livro` e `main` de teste**
- Criar `include/Texto.hpp`, `src/Texto.cpp`, `include/Livro.hpp`,
  `src/Livro.cpp`, `src/main.cpp`.
- O `main` só testa: cria dois `Livro`, imprime os campos e confere
  `operator==` e `operator<`.
- Testar: compilar e rodar `./sistema`; ver a saída do teste.

**Etapa 2 — `Estante` com cadastrar/listar/remover + menu inicial**
- Criar `include/Estante.hpp`, `src/Estante.cpp` (`Resultado`, `cadastrar`,
  `remover`, `listar`, `vazia`, `buscar`).
- Criar `include/Interface.hpp`, `src/Interface.cpp` (menu: cadastrar, listar,
  remover, sair + leitura segura da opção). Ajustar `src/main.cpp`.
- Testar: cadastrar dois livros, conferir a ordem; título repetido; remover;
  listar estante vazia.

**Etapa 3 — Status lido / por ler**
- `Estante::definirStatus` (limpa nota e comentário ao voltar para "por ler")
  e opção no menu (aceita maiúsculas/minúsculas; inválido →
  "Essa opcao nao existe.").
- Testar: por ler → lido → por ler.

**Etapa 4 — Nota e comentário**
- `Estante::avaliar` (valida tudo antes de alterar; `"-"` mantém, vazio
  remove, 0–10, sem alteração parcial, só lido) e opção no menu.
- Testar: manter, substituir, remover; nota inválida cancelando **os dois**
  campos mesmo com comentário válido; só um campo alterado; livro ainda não
  lido.

**Etapa 5 — Consultas e estatísticas**
- `livrosDoAutor`, `generosMaisLidos` (`map`), `livrosMaisBemAvaliados`
  (`sort` + `operator<`), `temLidos`, `temAvaliados` + opções no menu.
- Testar: autor existente/inexistente; desempate alfabético dos gêneros;
  grafia vinda do livro mais antigo ainda cadastrado mesmo que ele esteja
  "por ler"; contagem considerando só os lidos e ocultando gêneros sem lidos;
  desempate "mais recente" nas notas.

**Etapa 6 — Acabamento e exemplos**
- Revisar todas as mensagens e os casos de estante vazia; criar arquivos em
  `exemplos/` com roteiros de entrada; conferir `README.md` (comando e lista
  de arquivos; a seção "Onde uso a STL" é sua, escrita à mão).
- Testar rodando todos os `exemplos/*.txt`.
