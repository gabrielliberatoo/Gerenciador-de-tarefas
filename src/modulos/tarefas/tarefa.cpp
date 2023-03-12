#include "tarefa.hpp"

Tarefa::Tarefa(string titulo, string descricao, string prazo, bool concluido, Grupo *grupo_tarefa, Prioridade *prioridade_tarefa) {
    _titulo = titulo;
    _descricao = descricao;
    _prazo = prazo;
    _concluido = concluido;
    _prioridade_tarefa = prioridade_tarefa;
    _grupo_tarefa = grupo_tarefa;
    grupo_tarefa->incrementa_tarefa();
    prioridade_tarefa->incrementa_tarefa();
}

string Tarefa::get_descricao() {
    return _descricao;
}

string Tarefa::get_titulo() {
    return _titulo; 
}

string Tarefa::get_prazo() {
    return _prazo;
}

bool Tarefa::get_concluido() {
    return _concluido;
}

int Tarefa::get_valor_prioridade() {
    return _prioridade_tarefa->get_nivel_prioridade();
}

Prioridade* Tarefa::get_prioridade() {
    return _prioridade_tarefa;
}

string Tarefa::get_nome_prioridade() {
    return _prioridade_tarefa->get_nome();
}

string Tarefa::get_nome_grupo() {
    return _grupo_tarefa->get_nome_etiqueta();
}

Grupo* Tarefa::get_grupo() {
    return _grupo_tarefa;
}

void Tarefa::editar_titulo(string novo_titulo) {
    _titulo = novo_titulo;
}

void Tarefa::editar_descricao(string nova_descricao) {
    _descricao = nova_descricao;
}

void Tarefa::editar_prazo(string novo_prazo) {
    _prazo = novo_prazo;
}

void Tarefa::editar_status(bool novo_status) {
    _concluido = novo_status;
}
void Tarefa::editar_prioridade(Prioridade* prioridade) {
    _prioridade_tarefa = prioridade;
}

void Tarefa::editar_grupo(Grupo* novo_grupo) {
    _grupo_tarefa = novo_grupo;
}

bool Tarefa::prazo_passou() {
    // Pega o tempo atual
    time_t t = time(0);   
    tm* now = localtime(&t);
    // Separa o tempo atual em dia, mês e ano
    int dia_atual = now->tm_mday;
    int mes_atual = now->tm_mon + 1;
    int ano_atual = now->tm_year + 1900;

    string resultado = "";
    resultado = resultado + std::to_string(dia_atual) + std::to_string(mes_atual) + std::to_string(ano_atual);

    string delimitador = "/";
    vector<string> data = StringUtil::split_str(_prazo, delimitador);
    int dia_prazo = stoi(data[0]);
    int mes_prazo = stoi(data[1]);
    int ano_prazo = stoi(data[2]);
   
    if(ano_prazo > ano_atual) {
        return false;
    }
    else if(ano_prazo < ano_atual) {
        return true;
    }
    else {
        if(mes_prazo > mes_atual) {
            return false;
        }
        else if(mes_prazo < mes_atual) {
            return true;
        }
        else {
            if(dia_prazo > dia_atual) {
                return false;
            }
            else{
                return true;
            }
        }
    }
}

bool Tarefa::compara_prioridade(Tarefa *tarefa1, Tarefa *tarefa2) {
    int prioridade1 = tarefa1->get_valor_prioridade();
    int prioridade2 = tarefa2->get_valor_prioridade();
    return (prioridade1 < prioridade2);
}