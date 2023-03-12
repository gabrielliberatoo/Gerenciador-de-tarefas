#ifndef EXCECOES_CANCELAMENTO
#define EXCECOES_CANCELAMENTO

#include <exception>
#include <string>
using namespace std;

/**
 * @brief Exceção gerada ao cancelar a criação de uma tarefa no terminal
 */
class CriacaoTarefaCancelada : public exception {
    private:
        string mensagem = "Criação de tarefa cancelada";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

/**
 * @brief Exceção gerada ao cancelar a criação de um grupo no terminal
 */
class CriacaoGrupoCancelada : public exception {
    private:
        string mensagem = "Criação de grupo cancelada";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

/**
 * @brief Exceção gerada ao cancelar a criação de uma prioridade no terminal
 */
class CriacaoPrioridadeCancelada : public exception {
    private:
        string mensagem = "Criação de prioridade cancelada";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

/**
 * @brief Exceção gerada ao cancelar a edição de algum parâmetro de uma tarefa no terminal
 */
class EdicaoTarefaCancelada : public exception {
    private:
        string mensagem = "Edição de tarefa cancelada";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

/**
 * @brief Exceção gerada ao cancelar a edição de algum parâmetro de um grupo no terminal
 */
class EdicaoGrupoCancelada : public exception {
    private:
        string mensagem = "Edição de grupo cancelada";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

/**
 * @brief Exceção gerada ao cancelar a edição de algum parâmetro de uma prioridade no terminal
 */
class EdicaoPrioridadeCancelada : public exception {
    private:
        string mensagem = "Edição de prioridade cancelada";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

#endif // EXCECOES_CANCELAMENTO