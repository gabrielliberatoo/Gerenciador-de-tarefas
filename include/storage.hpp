#ifndef STORAGE_HPP
#define STORAGE_HPP

#include "etiqueta.hpp"
#include "grupo.hpp"
#include "string"
#include "file_util.hpp"
#include "tarefa.hpp"
#include "prioridade.hpp"
#include "excecoes/exc_usuario.hpp"
#include "excecoes/exc_grupo.hpp"
#include "excecoes/exc_prioridade.hpp"
#include "excecoes/exc_arquivo.hpp"
#include "excecoes/exc_tarefa.hpp"
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

/**
 * @brief Classe que armazena, genrencia e modifica tarefas, grupos e prioridades.
 */
class Storage {
    private:
        fstream _file;//!< guarda o arquvio do usuario.
        bool _logado;//!< guarda o status do usuario logado(true) ou não(false).
        string _usuario_logado;//!< guarda nome de usuario logado
        vector<Tarefa*> _tarefas;//!< vetor que armazena todas tarefas do usuario.
        vector<Grupo*> _grupos;//!< vetor que armazena todos os grupos do usuario.
        vector<Prioridade*> _prioridades;//!< vetor que armazena todas as prioridades do usuario.
    public:
        ///Método construtor da classe Storage. 
        Storage();
        
        /**Função com um parametro retorna o nome do arquivo do usuario
            * /param usuario uma string que contem o nome de usuario.
            * /return string nome do arquivo.
         */     
        string get_nome_arquivo(string usuario);
        
        /**Função que checa se um usuario já existe.
            * /param usuario uma string que contem o nome de usuario.
            * /return bool true se o usuario já existe e false se não existe.
         */
        bool usuario_existe(string usuario);
        
        /**Função com dois parametros que cria um usuario.
            * /param usuario uma string que contem o nome de usuario.
            * /param senha uma string que contem a senha do usuario.
            * /return bool true caso a operação seja bem sucedida
         */        
        bool cria_usuario(string usuario, string senha);

        bool exclui_usuario(string usuario);
       
        /**Função com dois parametros que confere se a senha do usuario está correta.
            * /param usuario uma string que contem o nome de usuario.
            * /param senha uma string que contem a senha do usuario.
            * /retun bool true caso a operação seja bem sucedida
         */        
        bool confere_senha(string usuario, string senha);

        ///Função que ordena as tarefas dentro do vetor _tarefas do valor de maior prioridade para o menor.
        bool ordena_tarefas();
        
        /**Função que recebe um parâmetro e define se ele é inválido
            * /param parametro
            * /retun bool true caso o parâmetro seja vazio ou seja demarcação de tarefa, grupo ou prioridade
        */    
        bool parametro_invalido(string parametro);

        ///Função que lê uma tarefa no arquvio e guarda no vetor _tarefas.
        bool le_tarefa();
        
        ///Função que lê um grupo no arquivo e guarda no vetor _grupos.
        bool le_grupo();
        
        ///Função que lê uma prioridade no arquivo e guarda no vetor _prioridades.
        bool le_prioridade();
        
        ///Função que lê todo o arquvio.
        bool le_conteudo();
       
        ///Função que retorna um inteiro que representa a quantidade de tarefas.
        int get_qtd_tarefas();
        
        ///Função que retorna um inteiro que representa a quantidade de grupos.
        int get_qtd_grupos();
        
        ///Função que retorna um inteiro que representa a quantidade de prioridades.
        int get_qtd_prioridades();
        
        /**Função procura uma prioridade com o nome informado.
            * /param nome_prioridade string que contem o nome da prioridade.
            * /return um objeto Prioridade
            * /return ponteiro nulo caso não tenha uma prioridade com esse nome.
         */        
        Prioridade* get_prioridade(string nome_prioridade);

        /**Função que procura a prioridade com o indice informado no vetor _prioridades.
            * /param indice_prioridade inteiro contem o indice da prioridade.
            * /return um objeto Prioridade.
         */      
        Prioridade* get_prioridade(int indice_prioridade);
        
        /**Função procura um grupo com o nome informado.
            * /param nome_grupo string que contem o nome do grupo.
            * /return um objeto Grupo
            * /return ponteiro nulo caso não tenha um grupo com esse nome.
         */
        Grupo* get_grupo(string nome_grupo);
        
        /**Função que procura o grupo com o indice informado no vetor _grupos.
            * /param indice_grupo inteiro contem o indice do grupo.
            * /return um objeto Grupo.
         */   
        Grupo* get_grupo(int indice_grupo);
        
        /**Função procura uma tarefa com o nome informado.
            * /param nome_tarefa string que contem o nome do grupo.
            * /return um objeto Tarefa
            * /return ponteiro nulo caso não tenha uma tarefa com esse nome.
         */
        Tarefa* get_tarefa(string titulo_tarefa);
        
        /**Função que procura a tarefa com o indice informado no vetor _tarefas.
            * /param indice_tarefa inteiro contem o indice da tarefa.
            * /return um objeto Tarefa.
         */ 
        Tarefa* get_tarefa(int indice_tarefa);
        
        /**Função com um parametro que registra uma tarefa no arquivo.
            * /param tarefa um objeto Tarefa.
            * /return bool Sem retorno
         */
        bool registra_tarefa(Tarefa *tarefa);

        /**Função com cinco parametros qeu cria uma tarefa no vetor _tarefas.
            * /param titulo uma string que contem o titulo da tarefa.
            * /param descricao uma string que contem a descrição da tarefa.
            * /param prazo uma string que contem a data do prazo da tarefa.
            * /param grupo_tarefa uma string que contem o titulo do grupo associoado a tarefa.
            * /param prioridade_tarefa uma string que contem o titulo da prioridade associoada a tarefa.
            * /return bool Sem retorno
         */
        bool cria_tarefa(string titulo, string descricao,  string prazo, string grupo_tarefa, string prioridade_tarefa);
        
        /**Função com um parametro que registra um grupo no arquivo.
            * /param grupo um objeto Grupo.
            * /return bool Sem retorno
         */
        bool registra_grupo(Grupo *grupo);
        
        /**Função com dois parametros que registra um grupo no vetor _grupos.
            * /param nome_etiqueta string que contem o titulo do grupo.
            * /param descricao string que contem a descrição do grupo.
            * /return bool Sem retorno
         */
        bool cria_grupo(string nome_etiqueta, string descricao);
        
        /**Função com um parametro que registra uma prioridade no arquivo.
            * /param prioridade um objeto Prioridade.
            * /return bool Sem retorno
         */
        bool registra_prioridade(Prioridade *prioridade);
        
        /**Função com dois parametros que registra uma prioridade no vetor _prioridades.
            * /param nome_etiqueta string que contem o titulo da prioridade.
            * /param nivel_prioridade inteiro que contem o valor referente ao nivel de prioridade.
            * /return bool Sem retorno
         */
        bool cria_prioridade(string nome_etiqueta,int nivel_prioridade);
        
        ///Função que checa se uma tarefa existe.
        bool tarefa_existe(string titulo_tarefa);
        
        ///função que checa se um grupo existe
        bool grupo_existe(string nome_grupo);

        ///Função que checa se uma prioridade existe;
        bool prioridade_existe(string nome_prioridade);
        
        /**Função com dois parametros que edita o titulo da tarefa no vetor e no arquivo.
            * /param indice_tarefa um inteiro que contem o indice da tarefa
            * /param novo_titulo string que contem o novo titulo que será colocado na tarefa.
            * /return bool Sem retorno
         */
        bool editar_titulo(int indice_tarefa, string novo_titulo);
        
        /**Função com dois parametros que edita a descrição da tarefa no vetor e no arquivo.
            * /param indice_tarefa um inteiro que contem o indice da tarefa
            * /param nova_descricao string que contem a nova descrição que será colocado na tarefa.
            * /return bool Sem retorno
         */
        bool editar_descricao(int indice_tarefa, string nova_descricao);
        
        /**Função com dois parametros que edita o prazo da tarefa no vetor e no arquivo.
            * /param indice_tarefa um inteiro que contem o indice da tarefa
            * /param novo_prazo string que contem o novo prazo que será colocado na tarefa.
            * /return bool Sem retorno
         */
        bool editar_prazo(int indice_tarefa, string novo_prazo);
        
        /**Função com dois parametros que edita o grupo da tarefa.
            * /param indice_tarefa um inteiro que contem o indice da tarefa
            * /param novo_grupo string que contem o novo grupo que será colocado na tarefa.
            * /return bool Sem retorno
         */
        bool editar_grupo(int indice_tarefa, string novo_grupo);
        
        /**Função com dois parametros que edita a prioridade da tarefa no vetor e no arquivo.
            * /param indice_tarefa um inteiro que contem o indice da tarefa
            * /param nova_prioridade string que contem a nova prioridade que será colocado na tarefa.
            * /return bool Sem retorno
         */
        bool editar_prioridade(int indice_tarefa, string nova_prioridade);
        
        /**Função com dois parametros que edita o titulo do grupo no vetor e no arquivo.
            * /param indice_grupo um inteiro que contem o indice do grupo
            * /param novo_grupo string que contem o novo titulo que será dado ao grupo.
            * /return bool Sem retorno
         */
        bool alterar_grupo(int indice_grupo, string novo_grupo);

        /**Função com dois parametros que edita a descrição do grupo no vetor e no arquivo.
            * /param indice_grupo um inteiro que contem o indice do grupo
            * /param nova_descricao string que contem a nova descrição que será dada ao grupo.
            * /return bool Sem retorno
         */
        bool alterar_des_grupo(int indice_grupo, string nova_descricao);
        
        /**Função com dois parametros que edita o titulo da prioridade no vetor e no arquivo.
            * /param indice_prioridade um inteiro que contem o indice da prioridade
            * /param nova_prioridade string que contem o novo titulo que será dada a prioridade.
            * /return bool Sem retorno
         */
        bool alterar_prioridade(int indice_prioridade, string nova_prioridade);
       
        /**Função com dois parametros que edita o nivel da prioridade no vetor e no arquivo.
            * /param indice_prioridade um inteiro que contem o indice da prioridade
            * /param nova_nivel um inteiro que contem o novo nivel que será dado a prioridade.
            * /return bool Sem retorno
         */
        bool alterar_nivel(int indice_prioridade, int novo_nivel);
        
        /**Função com dois parametros que marca uma tarefa como concluida no vetor e no arquivo.
            * /param indice_tarefa um inteiro que contem o indice da tarefa
            * /param novo_status bool que contem true.
            * /return bool Sem retorno
         */
        bool alterar_concluido(int indice_tarefa, bool novo_status);
        
        /**Função com um parametro que apaga a tarefa do arquvio e do vetor.
            * /param indice_tarefa um inteiro que contem o indice da tarefa
            * /return bool Sem retorno
        
        */
        bool apaga_tarefa(int indice_tarefa);
        
        /**Função com um parametro que apaga o grupo do arquvio e do vetor.
            * /param indice_grupo um inteiro que contem o indice do grupo
            * /return bool Sem retorno
        
        */
        bool apaga_grupo(int indice_grupo);
        
        /**Função com um parametro que apaga a prioridade do arquvio e do vetor.
            * /param indice_prioridade um inteiro que contem o indice da prioridade
            * /return bool Sem retorno
        
        */
        bool apaga_prioridade(int indice_prioridade);

        /**Função sem parâmetros que desaloca os ponteiros das tarefas, prioridades e grupos.
            * /param void Sem parâmetro
            * /return bool Verdadeiro caso a operação tenha sido feita com sucesso
        
        */
        bool finaliza_storage();
};

#endif // STORAGE_HPP