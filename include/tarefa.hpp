#ifndef TAREFA_HPP
#define TAREFA_HPP

#include "prioridade.hpp"
#include "grupo.hpp"
#include "string_util.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

/**
 * @brief Classe que gera e manipula todos atributos de uma Tarefa.
 */
class Tarefa {
    private:
        string _titulo; //!< guarda o titulo da tarefa.
        string _descricao; //!< guarda a descrição da tarefa.
        string _prazo; //!< guarda a data do prazo da tarefa.
        bool _concluido; //!< bool que retorna true se a tarefa for marcada como concluida.
        Prioridade *_prioridade_tarefa; //!< Guarda o a prioridade atribuida a tarefa.
        Grupo *_grupo_tarefa; //!< guarda o grupo atribuido a tarefa.
    public: 
        ///Método construtor da classe Tarefa. 
        Tarefa(string titulo, string descricao,  string prazo, bool concluido, Grupo *grupo_tarefa, Prioridade *prioridade_tarefa);
        ///Função que retorna o titulo da tarefa. 
        string get_titulo();
        ///Função que retorna a descrição da tarefa. 
        string get_descricao();
        ///Função que retorna a data prazo da tarefa. 
        string get_prazo();
        ///Função que retorna True se a tarefa está concluida e False se não está. 
        bool get_concluido();
        ///Função que retorna o titulo do grupo que a tarefa está associada. 
        string get_nome_grupo();
        ///Função que retorna o objeto Grupo que a tarefa está associada. 
        Grupo* get_grupo();
        ///Função que retorna o nivel de prioridade da tarefa. 
        int get_valor_prioridade();
        ///Função que retorna o objeto Prioridade associado a tarefa. 
        Prioridade* get_prioridade();
        ///Função que retorna o titulo da prioridade que a tarefa está associada.
        string get_nome_prioridade();
        
        /**Função com um parametro que edita o titulo da tarefa.
            * /param novo_titulo uma string que contem o novo titulo da tarefa.
            * /return void Sem retorno
         */
        void editar_titulo(string novo_titulo);
        
         /**Função com um parametro que edita a descrição da tarefa.
            * /param nova_descrição uma string que contem o novo titulo da tarefa.
            * /return void Sem retorno.
         */
        void editar_descricao(string nova_descricao);
        
        /**Função com um parametro que edita o prazo da tarefa.
            * /param novo_prazo uma string que contem o novo prazo da tarefa.
            * /return void Sem retorno
        */
        void editar_prazo(string novo_prazo);
       
        /**Função com um parametro que edita o status da tarefa.
            * /param novo_titulo um bool que contem o status(concluido/pendente) da tarefa.
            * /return void Sem retorno
         */       
        void editar_status(bool novo_status);
       
        /**Função com um parametro que edita a prioridade da tarefa.
            * /param prioridade um objeto Prioridade que contem a nova prioridade da tarefa.
            * /return void Sem retorno
         */       
        void editar_prioridade(Prioridade* prioridade);
        
        /**Função com um parametro que edita o grupo da tarefa.
            * /param grupo um objeto grupo que contem o novo grupo da tarefa.
            * /return void Sem retorno
         */
        void editar_grupo(Grupo* grupo);
       
        //*Função que checa se o prazo da tarefa já expirou. 
        bool prazo_passou();
        
        /**Função com dois parametros que compara o nivel de prioridade de duas tarefas.
            * /param tarefa1 um objeto tarefa referente a primeira tarefa da comparação.
            * /param tarefa2 um objeto tarefa referente a segunda tarefa da comparação.
            * /return bool retorna True se a tarefa1 possui maior maior prioridade em comparação com a tarefa2.
         */
        static bool compara_prioridade(Tarefa *tarefa1, Tarefa *tarefa2);
};

#endif // TAREFA_HPP