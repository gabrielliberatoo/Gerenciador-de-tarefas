#ifndef EXCECOES_PAGINA
#define EXCECOES_PAGINA

#include <exception>
#include <string>
using namespace std;

/**
 * @brief Exceção gerada ao tentar exibir uma página que não existe
 */
class PaginaInvalida : public exception {
    private:
        string mensagem = "Essa página não existe. Escolha uma página válida.";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

/**
 * @brief Exceção gerada ao tentar exibir uma página sem informar seu número
 */
class PaginaNaoInformada : public exception {
    private:
        string mensagem = "Você deve informar o número da página que deseja exibir. Tente novamente";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

#endif // EXCECOES_PAGINA