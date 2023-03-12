#ifndef PRIORIDADE_HPP
#define PRIORIDADE_HPP

#include "etiqueta.hpp"
#include <string>

using namespace std;

/**
 * @brief Classe que gera e manipula todos atributos de uma Prioridade.
 */
class Prioridade : public Etiqueta {
    private: 
        int _nivel_prioridade; //!< guarda um inteiro que representa o nivel de uma prioridade.
    public:
        /**Método construtor da classe Prioridade que recebe três parametros.
            * /param nome_etiqueta uma string que contem o da prioridade.
            * /param tipo uma string que contem o tipo da prioridade.
            * /param tipo um inteiro que contem o nivel da prioridade.
        */
        Prioridade(string nome_etiqueta, string tipo, int nivel_prioridade);
        ///Função que retorna o nivel da etiqueta.
        int get_nivel_prioridade();
       /**Função com um parametro que edita o nivel da prioridade.
            * /param new_prioridade um inteiro que contem o novo nivel de prioridade.
            * /return void Sem retorno
         */         
        void alterar_valor(int new_prioridade);
        ///Função que retorna o titulo da prioridade.
        string get_nome();
        /**Função com um parametro que edita o titulo da prioridade.
            * /param new_nome um inteiro que contem o novo titulo da prioridade.
            * /return void Sem retorno
         */         
        void editar_prioridade(string new_nome);
        
};

#endif // PRIORIDADE_HPP