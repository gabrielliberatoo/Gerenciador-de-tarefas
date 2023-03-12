#ifndef GRUPO_HPP
#define GRUPO_HPP

#include <string>
#include "etiqueta.hpp"

using namespace std;

/**
 * @brief Classe que gera e manipula todos atributos de um grupo.
 */
class Grupo: public Etiqueta {
    private: 
        string _descricao; //!< guarda a descrição de um grupo.
    public:
    /**Método construtor da classe Grupo que recebe três parametros.
        * /param nome_etiqueta uma string que contem o titulo do grupo.
        * /param tipo uma string que contem o tipo do grupo.
        * /param descricao uma string que contem a descrição do grupo.
    */ 
        Grupo(string nome_etiqueta, string tipo, string descricao);
        ///Função que retorna a quantidade de tarefas de um grupo.
        int get_qtd_membros();
        ///Função que retorna a descrição de um grupo.
        string get_descricao();
        /**Função com um parametro que edita o titulo do grupo.
            * /param new_titulo uma string que contem o novo titulo do grupo.
            * /return void Sem retorno
         */
        void alterar(string new_grupo);
       /**Função com um parametro que edita a descrição da tarefa.
            * /param new_descrição uma string que contem a nova descrição da tarefa.
            * /return void Sem retorno
         */
        void alterar_descricao(string new_descricao);
};

#endif // GRUPO_HPP