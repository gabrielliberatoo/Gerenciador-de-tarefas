#include "etiqueta.hpp"

Etiqueta::Etiqueta(string nome_etiqueta, string tipo) {
    _nome_etiqueta = nome_etiqueta;
    _tipo= tipo;
    _qtd_membros = 0;
}

string Etiqueta::get_nome_etiqueta() {
    return _nome_etiqueta;
}

string Etiqueta::get_tipo() {
    return _tipo;
}

void Etiqueta::incrementa_tarefa() {
    _qtd_membros = _qtd_membros + 1;
}

void Etiqueta::decrementa_tarefa() {
    _qtd_membros = _qtd_membros - 1;
}

int Etiqueta::get_qtd_membros() {
    return _qtd_membros;
}