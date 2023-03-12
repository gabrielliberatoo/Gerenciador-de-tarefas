#ifndef EXCECOES_PRIORIDADE
#define EXCECOES_PRIORIDADE

#include <exception>
#include <string>
using namespace std;

/**
 * @brief Exceção gerada ao tentar fornecer um nome de prioridade com mais que 15 caracteres
 */
class NomePrioridadeMuitoLongo : public exception {
    private:
        string mensagem = "O nome da prioridade não pode ter mais que 15 caracteres. Tente novamente";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

/**
 * @brief Exceção gerada ao tentar criar ou editar uma prioridade fornecendo um nome que já existe
 */
class PrioridadeJaExiste : public exception {
    private:
        string mensagem = "Já existe uma prioridade com esse nome. Tente novamente";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

/**
 * @brief Exceção gerada ao tentar fornecer um valor de prioridade que não seja um número positivo
 */
class ValorPrioridadeInvalido : public exception {
    private:
        string mensagem = "O valor da prioridade deve ser um número positivo. Tente novamente";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

/**
 * @brief Exceção gerada ao tentar acessar uma prioridade fornecendo um nome que não existe
 */
class PrioridadeNaoExiste : public exception {
    private:
        string mensagem = "Não existe nenhuma prioridade com esse nome. Tente novamente";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

/**
 * @brief Exceção gerada ao tentar acessar uma prioridade fornecendo um ID que não existe
 */
class IdPrioridadeNaoExiste : public exception {
    private:
        string mensagem = "Não existe nenhuma prioridade com esse ID. Tente novamente";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

/**
 * @brief Exceção gerada ao tentar apagar uma prioridade que tem tarefas associadas a ele
 */
class ErroAoApagarPrioridade : public exception {
    private:
        string mensagem = "Não é possivel apagar essa prioridade pois existem tarefas associadas a ela.";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

#endif // EXCECOES_PRIORIDADE