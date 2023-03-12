#include "prioridade.hpp"

Prioridade::Prioridade(string nome_etiqueta, string tipo, int nivel_prioridade):Etiqueta(nome_etiqueta, tipo) {
    _nome_etiqueta = nome_etiqueta;
    _tipo = tipo;
    _nivel_prioridade = nivel_prioridade;
}

int Prioridade::get_nivel_prioridade() {
    return _nivel_prioridade;
}

void Prioridade::alterar_valor(int new_prioridade) {
    _nivel_prioridade = new_prioridade;
}

string Prioridade::get_nome() {
    return _nome_etiqueta;
}

void Prioridade::editar_prioridade(string new_nome) {
    _nome_etiqueta =  new_nome;
}