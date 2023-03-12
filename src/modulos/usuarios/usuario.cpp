#include "usuario.hpp"

Usuario::Usuario() {
    _nome_usuario = "";
}

Usuario::Usuario(string usuario, string senha) {
    _storage = new Storage();
    // Confere se existe um usuário com o nome informado
    if(!_storage->usuario_existe(usuario)) {
        throw UsuarioNaoEncontrado();
    }

    if(_storage->confere_senha(usuario, senha)) {
        _nome_usuario = usuario;
    }
    else {
        throw SenhaIncorreta();
    }
}

string Usuario::get_nome() {
    return _nome_usuario;
}

Storage* Usuario::get_storage() {
    return _storage;
}