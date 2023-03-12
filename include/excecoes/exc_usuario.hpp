#ifndef EXCECOES_CRIACAO_USUARIO
#define EXCECOES_CRIACAO_USUARIO

#include <exception>
#include <string>
using namespace std;

/**
 * @brief Exceção gerada ao tentar criar um usuário com nome que contém caracteres inválidos
 */
class CaractereInvalido : public exception {
    private:
        string mensagem = "Nome de usuário inválido. Tente novamente.";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

/**
 * @brief Exceção gerada ao tentar criar um usuário com um nome que já existe
 */
class UsuarioJaExiste : public exception {
    private:
        string mensagem = "Já existe um usuário com esse nome. Tente novamente.";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

/**
 * @brief Exceção gerada ao tentar acessar um usuário que não existe
 */
class UsuarioNaoEncontrado : public exception {
    private:
        string mensagem = "Usuário não encontrado. Tente novamente.";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

/**
 * @brief Exceção gerada ao tentar acessar um usuário com uma senha incorreta
 */
class SenhaIncorreta : public exception {
    private:
        string mensagem = "Senha incorreta. Tente novamente.";
    public:
        const char* what() const noexcept override {
            return mensagem.c_str();
        }
};

#endif // EXCECOES_CRIACAO_USUARIO