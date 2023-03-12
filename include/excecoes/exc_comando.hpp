#ifndef EXCECOES_COMANDO
#define EXCECOES_COMANDO

#include <exception>
#include <string>
using namespace std;

/**
 * @brief Exceção gerada ao tentar editar uma tarefa que não existe no terminal
 */
class ComandoEditarTarefaErrado : public exception {
    private:
        string mensagem = "Você deve informar o ID da tarefa que deseja editar. Tente novamente";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

/**
 * @brief Exceção gerada ao tentar excluir uma tarefa que não existe no terminal
 */
class ComandoExcluirTarefaErrado : public exception {
    private:
        string mensagem = "Você deve informar o ID da tarefa que deseja excluir. Tente novamente";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

/**
 * @brief Exceção gerada ao tentar exibir uma tarefa que não existe no terminal
 */
class ComandoExibirTarefaErrado : public exception {
    private:
        string mensagem = "Você deve informar o ID da tarefa que deseja exibir. Tente novamente";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

/**
 * @brief Exceção gerada ao tentar editar um grupo que não existe no terminal
 */
class ComandoEditarGrupoErrado : public exception {
    private:
        string mensagem = "Você deve informar o ID do grupo que deseja editar. Tente novamente";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

/**
 * @brief Exceção gerada ao tentar excluir um grupo que não existe no terminal
 */
class ComandoExcluirGrupoErrado : public exception {
    private:
        string mensagem = "Você deve informar o ID do grupo que deseja editar. Tente novamente";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

/**
 * @brief Exceção gerada ao tentar editar uma prioridade que não existe no terminal
 */
class ComandoEditarPrioridadeErrado : public exception {
    private:
        string mensagem = "Você deve informar o ID da prioridade que deseja editar. Tente novamente";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

/**
 * @brief Exceção gerada ao tentar editar uma prioridade que não existe no terminal
 */
class ComandoExcluirPrioridadeErrado : public exception {
    private:
        string mensagem = "Você deve informar o ID da prioridade que deseja excluir. Tente novamente";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

#endif // EXCECOES_COMANDO