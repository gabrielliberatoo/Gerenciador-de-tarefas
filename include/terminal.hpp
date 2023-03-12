#ifndef TERMINAL_HPP
#define TERMINAL_HPP

#include "grupo.hpp"
#include "prioridade.hpp"
#include "storage.hpp"
#include "usuario.hpp"
#include "file_util.hpp"
#include "excecoes/exc_usuario.hpp"
#include "excecoes/exc_comando.hpp"
#include "excecoes/exc_grupo.hpp"
#include "excecoes/exc_pagina.hpp"
#include "excecoes/exc_prioridade.hpp"
#include "excecoes/exc_tarefa.hpp"
#include "excecoes/exc_cancelamento.hpp"
#include <sys/ioctl.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cstddef>
#include <exception>
#include <iostream>
#include <string>
#include <limits>
#include <ios>
#include <ctime>
#include <chrono>
#include <vector>

#define TEXTO_VERMELHO "\033[31m"
#define TEXTO_VERDE "\033[32m"
#define TEXTO_AMARELO "\033[33m"
#define TEXTO_AZUL "\033[34m"
#define TEXTO_ROXO "\033[35m"
#define TEXTO_CIANO "\033[36m"
#define TEXTO_CINZA "\033[37m"
#define TEXTO_GRIFADO "\033[4m"
#define TEXTO_NORMAL "\033[0m"

using namespace std;

class Terminal {
    private:
        int _largura_terminal;//!< guarda a largura do terminal.
        int _altura_terminal;//!< guarda a altura do terminal.
        int _padding_y;//!< guarda o espaço entre os cantos superiores e inferiores entre a tela e o texto
        int _padding_x;//!< guarda o espaço entre os cantos laterais entre a tela e o texto.
        unsigned int _tarefas_por_pagina;//!< guarda o numero de tarefas por pagina.
        Usuario _usuario;//!< guarda o nome de usuario.
        Storage* _storage;//!< Objeto que guarda o Storage.
        unsigned int _pagina_atual;//!< guarad a pagina dee tarefas que está aberta.
    public:
        Terminal();
       
        /**Função com dois parametros que printa mensagem junto a quebra de linha.
            * /param msg string que guarda a mensagem a ser impressa.
            * /param quebra_de_linha bool que retorna true se a operação foi concluida e false se ela falhou.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
         */
        bool print(string msg, bool quebra_de_linha);
        
        /**Função com um parametro printa mensagem indicando sucesso junto a quebra de linha.
            * /param msg_sucesso string que guarda a mensagem de sucesso.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
         */
        bool print_sucesso(string msg_sucesso);
       
        /**Função com um parametro printa mensagem indicando erro junto a quebra de linha.
            * /param msg_errp string que guarda a mensagem de erro.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
         */
        bool print_erro(string msg_erro);
        
        /**Função com um parametro que imprime texto centralizado.
            * /param texto string que guarda o texto que será centralizado.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
         */
        bool print_centralizado(string texto);
        
        /**Função com um parametro que imprime espaços na lateral direita
            * /param texto string que guarda o texto em que as margens serão aplicadas.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
         */
        bool print_direita(string texto);
        
        ///função que limpra o terminal, e retorna true.    
        bool limpa_terminal();
       
        ///Retorna o espaço vertical da interface no terminal.
        string espaco_vertical();
       
        ///Retorna o espaço horizontal da interface no terminal.
        string espaco_horizontal();
       
        ///Retorna uma string com os dados do input.
        string get_entrada();
        
        ///Função que pergunta se o usuário deseja fazer login ou registrar um novo usuário, retorna true.
        bool login_ou_registre();
        
        /**Função com um parametro que cria um usuario.
            * /param entradas um vetor do tipo string onde cada elemento é um comando.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
         */
        bool cria_usuario(vector<string> entradas = vector<string>());
        
        /**Função com um parametro que faz o login de um usuario.
            * /param entradas um vetor do tipo string onde cada elemento é um comando.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
         */
        bool faz_login(vector<string> entradas = vector<string>());
        
        ///Função que retorna true se o usuario logado possui tarefas e false se não.
        bool tem_tarefas();
        
        ///Função que divide a quantidade de tarefas pelo máximo por página e retorna o numero de paginas.
        int get_qtd_paginas();
        
        ///Função que divide a quantidade de grupos pelo máximo por página e retorna o numero de paginas.
        int get_qtd_paginas_grupos();
        
        ///Função que divide a quantidade de prioridades pelo máximo por página e retorna o numero de paginas.
        int get_qtd_paginas_prd();
        
        ///Função que exibe a lista de comandos do terminal.
        bool print_ajuda();
        
        /**Função com um parametro que exibe a pagina selecionada das tarefas.
            * /param pagina_atual um inteiro que contém o numero referente a pagina atual.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
         */
        bool print_paginacao(int pagina_atual);
        
        /**Função com um parametro que exibe a pagina selecionada dos gruposs.
            * /param pagina_atual um inteiro que contém o numero referente a pagina atual.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
         */
        bool print_paginacao_grupos(int pagina_atual);
        
        /**Função com um parametro que exibe a pagina selecionada das prioridades.
            * /param pagina_atual um inteiro que contém o numero referente a pagina atual.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
         */
        bool print_paginacao_prd(int pagina_atual);
        
        ///Função que imprime cabeçalho de informações sobre as tarefas e retorna true.
        bool print_cabecalho();
        
        ///Função que imprime cabeçalho de informações sobre os grupos.
        bool print_cabecalho_grupo();
        
        ///Função que imprime cabeçalho de informações sobre as prioridades.
        bool print_cabecalho_prd();
        
        /**Função com um parametro que checa se uma pagina de tarefas existe.
            * /param pagina um inteiro que contém o numero referente a pagina de tarefas.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
         */
        bool pagina_existe(int pagina);
        
        /**Função com um parametro que checa se uma pagina de grupos existe.
            * /param pagina um inteiro que contém o numero referente a pagina de grupos.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
         */
        bool pagina_grupo_existe(int pagina);
        
        /**Função com um parametro que checa se uma pagina de prioridades existe.
            * /param pagina um inteiro que contém o numero referente a pagina de prioridades.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
         */
        bool pagina_prd_existe(int pagina);
        
        /**Função com dois parametros que imprime tarefa na tela.
            * /param tarefa objeto Grupo com as iformações da tarefa.
            * /param id interiro que representa a identificação da tarefa.
            * /return bool que retorna true.
         */
        bool print_tarefa(Tarefa* tarefa, int id);
        
        /**Função com um parametro que define quais vão ser a primeira e a última tarefa mostradas na página e printa a pagina.
            * /param pagina um inteiro que contém o numero referente a pagina de tarefas.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
         */
        bool print_tarefas(int pagina);
        
        /**Função com um parametro que printa a tarefa escolhida pelo id e suas informações na tela.
            * /param comando string que contem o comando a ser executado.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
         */
        bool exibe_tarefa(string comando);
        
        /**Função com dois parametros quem imprime grupo na tela.
            * /param grupo objeto Grupo com as iformações do grupo.
            * /return bool que retorna true.
         */
        bool print_grupo(Grupo* grupo, int id);
        
        /**Função com um parametro que define quais vão ser o primeiro e ultimo grupo mostrados na página e printa a pagina.
            * /param pagina um inteiro que contém o numero referente a pagina de grupos.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
         */
        bool print_grupos(int pagina);
        
        /**Função com dois parametros quem imprime prioridade na tela.
            * /param prioridade objeto Prioridade com as iformações da prioridade.
            * /return bool que retorna true.
         */
        bool print_prioridade(Prioridade* prioridade, int id);
        
        /**Função com um parametro que define quais vão ser a primeira e a última prioridade mostradas na página e printa a pagina.
            * /param pagina um inteiro que contém o numero referente a pagina de prioridades.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
         */
        bool print_prioridades(int pagina);
        
        /**Função que checa se o prazo digitado é valido.
            * /return false se o prazo não tiver o número certo de caracteres.
            * /return false caso dia, mês e ano não sejam separados por barras.
            * /return false se mês é inválido.
            * /return false se o dia do mês não existe.
            * /return true caso seja válido.
        
        */
        bool prazo_valido(string prazo);
        
        /**Função com um parametro que cria uma nova tarefa.
            * /param entradas um vetor do tipo string onde cada elemento é um comando.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
        */
        bool nova_tarefa(vector<string> entradas = vector<string>());
        
        /**Função com um parametro que cria um novo grupo.
            * /param entradas um vetor do tipo string onde cada elemento é um comando.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
        */
        bool novo_grupo(vector<string> entradas = vector<string>());
       
        /**Função com um parametro que cria uma nova prioridade.
            * /param entradas um vetor do tipo string onde cada elemento é um comando.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
        */
        bool nova_prioridade(vector<string> entradas = vector<string>());
        
        /**Função com dois parametros que edita o titulo da tarefa.
            * /param comando string que contem o comando a ser executado.
            * /param entradas um vetor do tipo string onde cada elemento é um comando.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
        */
        bool editar_titulo_tarefa(string comando, vector<string> entradas = vector<string>());
        
        /**Função com dois parametros que edita a descrição da tarefa.
            * /param comando string que contem o comando a ser executado.
            * /param entradas um vetor do tipo string onde cada elemento é um comando.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
        */
        bool editar_desc_tarefa(string comando, vector<string> entradas = vector<string>());
        
        /**Função com dois parametros que edita o prazo da tarefa.
            * /param comando string que contem o comando a ser executado.
            * /param entradas um vetor do tipo string onde cada elemento é um comando.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
        */
        bool editar_prazo_tarefa(string comando, vector<string> entradas = vector<string>());
        
        /**Função com dois parametros que edita o grupo da tarefa.
            * /param comando string que contem o comando a ser executado.
            * /param entradas um vetor do tipo string onde cada elemento é um comando.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
        */
        bool editar_grupo_tarefa(string comando, vector<string> entradas = vector<string>());
        
        /**Função com dois parametros que edita a prioridade da tarefa.
            * /param comando string que contem o comando a ser executado.
            * /param entradas um vetor do tipo string onde cada elemento é um comando.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
        */
        bool editar_prd_tarefa(string comando, vector<string> entradas = vector<string>());
        
        /**Função com dois parametros que marca tarefa como conlcuida.
            * /param comando string que contem o comando a ser executado.
            * /param entradas um vetor do tipo string onde cada elemento é um comando.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
        */
        bool marcar_concluida(string comando, vector<string> entradas = vector<string>());
        
        /**Função com dois parametros que altera o status da tarefa para não concluida
            * /param comando string que contem o comando a ser executado.
            * /param entradas um vetor do tipo string onde cada elemento é um comando.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
        */
        bool desmarcar_concluida(string comando, vector<string> entradas = vector<string>());
        
        /**Função com dois parametros que edita o titulo do grupo.
            * /param comando string que contem o comando a ser executado.
            * /param entradas um vetor do tipo string onde cada elemento é um comando.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
        */
        bool editar_nome_grupo(string comando, vector<string> entradas = vector<string>());
        
        /**Função com dois parametros que edita a descrição do grupo.
            * /param comando string que contem o comando a ser executado.
            * /param entradas um vetor do tipo string onde cada elemento é um comando.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
        */
        bool editar_desc_grupo(string comando, vector<string> entradas = vector<string>());
        
        /**Função com dois parametros que edita o titulo da prioridade.
            * /param comando string que contem o comando a ser executado.
            * /param entradas um vetor do tipo string onde cada elemento é um comando.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
        */
        bool editar_nome_prd(string comando, vector<string> entradas = vector<string>());
        
        /**Função com dois parametros que edita o nivel da prioridade.
            * /param comando string que contem o comando a ser executado.
            * /param entradas um vetor do tipo string onde cada elemento é um comando.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
        */
        bool editar_nivel_prd(string comando, vector<string> entradas = vector<string>());
        
        /**Função com um parametro que deleta uma tarefa.
            * /param comando string que contem o comando a ser executado.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
        */
        bool deletar_tarefa(string comando);
        
        /**Função com um parametro que deleta um grupo.
            * /param comando string que contem o comando a ser executado.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
        */
        bool deletar_grupo(string comando);
        
        /**Função com um parametro que deleta uma prioridade.
            * /param comando string que contem o comando a ser executado.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
        */
        bool deletar_prioridade(string comando);
        
        ///Função que faz o logout do usuario.
        void logout();
        
        /**Função com um parametro que ativa comando do terminal digitado pelo usuario.
            * /param entradas um vetor do tipo string onde cada elemento é um comando.
            * /return bool que retorna true se a operação foi concluida e false se ela falhou.
        */
        bool pede_comando(vector<string> entradas = vector<string>());
        
        /**Função que inicial o terminal.
            pede para o usuario se logar ou registrar-se.
            exibe a primeira pagina de tarefas do usuario logado.
        */
        bool inicia();
};

#endif // TERMINAL_HPP