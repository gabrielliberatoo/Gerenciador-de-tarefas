#ifndef EXCECOES_ARQUIVO
#define EXCECOES_ARQUIVO

#include <exception>
#include <string>
using namespace std;

/**
 * @brief Exceção gerada ao tentar ler arquivos corrompidos ou no formato inválido
 */
class ErroAoAbrirArquivo : public exception {
    private:
        string mensagem = "Erro ao abrir arquivo do usuário.";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

#endif // EXCECOES_ARQUIVO