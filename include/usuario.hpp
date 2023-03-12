#ifndef USUARIO_HPP
#define USUARIO_HPP

#include "storage.hpp"
#include <string>
#include <vector>
#include <string>

using namespace std;

/**
 * @brief Classe que gera e manipula todos atributos do Usuario.
 */
class Usuario {
    private:
        string _nome_usuario; //!< guarda o nome de usuario.
        Storage* _storage; //!< guarda o Objeto _storage.
    public:
        ///Método construtor da classe usuario.
        Usuario();
        ///Função que registra usuario.
        Usuario(string usuario, string senha);
        ///Função que retorna o nome de ususario.
        string get_nome();
        ///Função que retorna o Objeto storege
        Storage* get_storage();
};

#endif // USUARIO_HPP