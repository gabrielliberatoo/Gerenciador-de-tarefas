#ifndef CLASSIFICACAO_HPP
#define CLASSIFICACAO_HPP

#include <string>

using namespace std;

/**
 * @brief Classe que gera e manipula todos atributos de uma Etiqueta.
 */
class Etiqueta {
    protected: 
        string _nome_etiqueta; //!< guarda o titulo de uma etiqueta.
        string _tipo; //!< guarda o tipo de uma etiqueta.
        int _qtd_membros; //!< guarda a quantidade de membros em uma etiqueta.
    public:
        /**Método construtor da classe Etiqueta que recebe dois parametros.
            * /param nome_etiqueta uma string que contem o da etiqueta.
            * /param tipo uma string que contem o tipo da etiqueta.
        */
        Etiqueta(string nome_etiqueta, string tipo);
        ///Função que retorna o titulo da etiqueta. 
        string get_nome_etiqueta();
        ///Função que retorna o tipo da etiqueta. 
        string get_tipo();
        ///Função que adiciona tarefa de _qtd_membros. 
        void incrementa_tarefa();
        ///Função que remove tarefa de _qtd_membros.  
        void decrementa_tarefa();
        ///Função que retorna a quantidade de membros. 
        int get_qtd_membros();
};

#endif // CLASSIFICACAO_HPP