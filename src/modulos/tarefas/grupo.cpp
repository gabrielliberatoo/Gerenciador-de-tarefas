#include "grupo.hpp"

Grupo::Grupo(string nome_etiqueta, string tipo, string descricao):Etiqueta(nome_etiqueta, tipo) {
    _nome_etiqueta = nome_etiqueta;
    _tipo = tipo;  
    _descricao = descricao;
}

int Grupo::get_qtd_membros() {
    return _qtd_membros;
}

string Grupo::get_descricao() {
    return _descricao;
}

void Grupo::alterar(string new_grupo) {
    _nome_etiqueta = new_grupo;
}

void Grupo::alterar_descricao(string new_descricao) {
    _descricao = new_descricao;
}