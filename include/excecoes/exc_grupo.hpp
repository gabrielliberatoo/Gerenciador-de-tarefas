#ifndef EXCECOES_GRUPO
#define EXCECOES_GRUPO

#include <exception>
#include <string>
using namespace std;

/**
 * @brief Exceção gerada ao tentar fornecer um nome de grupo com mais que 15 caracteres
 */
class NomeGrupoMuitoLongo : public exception {
    private:
        string mensagem = "O nome do grupo não pode ter mais que 15 caracteres. Tente novamente";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

/**
 * @brief Exceção gerada ao tentar acessar um grupo fornecendo um ID que não existe
 */
class IdGrupoNaoExiste : public exception {
    private:
        string mensagem = "Não existe nenhum grupo com esse ID. Tente novamente";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

/**
 * @brief Exceção gerada ao tentar acessar um grupo fornecendo um nome que não existe
 */
class GrupoNaoExiste : public exception {
    private:
        string mensagem = "Não existe nenhum grupo com esse nome. Tente novamente";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

/**
 * @brief Exceção gerada ao tentar criar ou editar um grupo fornecendo um nome que já existe
 */
class GrupoJaExiste : public exception {
    private:
        string mensagem = "Já existe um grupo com esse nome. Tente novamente";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

/**
 * @brief Exceção gerada ao tentar apagar um grupo que tem tarefas associadas a ele
 */
class ErroAoApagarGrupo : public exception {
    private:
        string mensagem = "Não é possivel apagar esse grupo pois existem tarefas associadas a ele.";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

#endif // EXCECOES_GRUPO