#ifndef EXCECOES_TAREFA
#define EXCECOES_TAREFA

#include <exception>
#include <string>
using namespace std;

/**
 * @brief Exceção gerada ao tentar fornecer um nome de tarefa com mais que 30 caracteres
 */
class NomeTarefaMuitoLongo : public exception {
    private:
        string mensagem = "O nome da tarefa não pode ter mais que 30 caracteres. Tente novamente";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

/**
 * @brief Exceção gerada ao tentar acessar uma tarefa fornecendo um ID que não existe
 */
class IdTarefaNaoExiste : public exception {
    private:
        string mensagem = "Não existe nenhuma tarefa com esse ID. Tente novamente";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

/**
 * @brief Exceção gerada ao tentar fornecer um prazo no formato inválido ou de uma data que não existe
 */
class PrazoInvalido : public exception {
    private:
        string mensagem = "Prazo inválido. Tente novamente";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

/**
 * @brief Exceção gerada ao tentar criar ou editar uma tarefa fornecendo um título que já existe
 */
class TarefaJaExiste : public exception {
    private:
        string mensagem = "Já existe uma tarefa com esse título. Tente novamente";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

#endif // EXCECOES_TAREFA