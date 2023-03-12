#include "excecoes/exc_comando.hpp"
#include "excecoes/exc_grupo.hpp"
#include "excecoes/exc_prioridade.hpp"
#include "excecoes/exc_tarefa.hpp"
#include "excecoes/exc_usuario.hpp"
#include "prioridade.hpp"
#include "storage.hpp"
#include "terminal.hpp"

// Importando o doctest
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"


/*-----------------------------------------------------------------------------------------
Testes no armazenamento
-----------------------------------------------------------------------------------------*/
Storage storage = Storage();

TEST_CASE("Testando criação de usuário que não existe") {
    // Exclui o usuário testes caso ele exista
    storage.exclui_usuario("usuario_teste");

    CHECK(storage.cria_usuario("usuario_teste", "123"));
}

TEST_CASE("Testando criação de usuário que já existe") {
    CHECK_THROWS_AS(storage.cria_usuario("usuario_teste", "123"), UsuarioJaExiste);
}

TEST_CASE("Testando login no storage") {
    // Tenta logar com a senha errada
    CHECK_FALSE(storage.confere_senha("usuario_teste", "1234"));
    // Tenta logar com um usuário que não existe
    CHECK_THROWS_AS(storage.confere_senha("usuario_inexistente", "123"), UsuarioNaoEncontrado);
    // Tenta logar com a senha certa
    CHECK(storage.confere_senha("usuario_teste", "123"));
}

TEST_CASE("Testando elementos zerados em usuário recém criado") {
    CHECK(storage.get_qtd_tarefas() == 0);
    CHECK(storage.get_qtd_grupos() == 0);
    CHECK(storage.get_qtd_prioridades() == 0);
}

TEST_CASE("Testando criação de grupos no storage") {
    CHECK(storage.cria_grupo("Faculdade", "Tarefas da faculdade"));
    CHECK(storage.cria_grupo("Casa", "Tarefas da faculdade"));
    CHECK_THROWS_AS(storage.cria_grupo("Faculdade", "Trabalhos, atividades"), GrupoJaExiste);
}

TEST_CASE("Testando criação de prioridades no storage") {
    CHECK(storage.cria_prioridade("Urgente", 1));
    CHECK(storage.cria_prioridade("Baixa", 1));
    CHECK_THROWS_AS(storage.cria_prioridade("Urgente", 3), PrioridadeJaExiste);
}

TEST_CASE("Testando criação de tarefas no storage") {
    CHECK(storage.cria_tarefa("Fazer trabalho de PDS", "Fazer documentação", "01/12/2022", "Faculdade", "Urgente"));
    CHECK(storage.cria_tarefa("Fazer lista de economia", "Fazer documentação", "01/12/2022", "Faculdade", "Urgente"));
    CHECK_THROWS_AS(storage.cria_tarefa("Fazer trabalho de PDS", "Escrever código", "01/12/2021", "Faculdade", "Urgente"), TarefaJaExiste);
    CHECK_THROWS_AS(storage.cria_tarefa("Estudar para MD", "Ler slides", "12/12/2021", "Estudos", "Urgente"), GrupoNaoExiste);
    CHECK_THROWS_AS(storage.cria_tarefa("Estudar para MD", "Ler slides", "12/12/2021", "Faculdade", "Alta"), PrioridadeNaoExiste);
}

TEST_CASE("Testando getter de tarefa no storage") {
    CHECK(storage.get_tarefa(1) != nullptr);
    CHECK(storage.get_tarefa("Fazer trabalho de PDS") != nullptr);
    CHECK(storage.get_tarefa(5) == nullptr);
    CHECK(storage.get_tarefa("Fazer lista de ALC") == nullptr);
}

TEST_CASE("Testando getter de grupo no storage") {
    CHECK(storage.get_grupo(1) != nullptr);
    CHECK(storage.get_grupo("Faculdade") != nullptr);
    CHECK(storage.get_grupo(5) == nullptr);
    CHECK(storage.get_grupo("Trabalho") == nullptr);
}

TEST_CASE("Testando getter de prioridade no storage") {
    CHECK(storage.get_prioridade(1) != nullptr);
    CHECK(storage.get_prioridade("Urgente") != nullptr);
    CHECK(storage.get_prioridade(5) == nullptr);
    CHECK(storage.get_prioridade("Alta") == nullptr);
}

TEST_CASE("Testando edição de tarefa no storage") {
    CHECK(storage.editar_titulo(1, "Terminar trabalho de PDS"));
    CHECK(storage.editar_descricao(1, "Escrever testes"));
    CHECK(storage.editar_prazo(1, "06/12/2022"));
    CHECK(storage.editar_grupo(1, "Casa"));
    CHECK(storage.editar_prioridade(1, "Baixa"));
    CHECK(storage.alterar_concluido(1, true));

    CHECK_THROWS_AS(storage.editar_titulo(1, "Fazer lista de economia"), TarefaJaExiste);
    CHECK_THROWS_AS(storage.editar_prazo(1, "06/12/22"), PrazoInvalido);
    CHECK_THROWS_AS(storage.editar_grupo(1, "Trabalho"), GrupoNaoExiste);
    CHECK_THROWS_AS(storage.editar_prioridade(1, "Alta"), PrioridadeNaoExiste);

    CHECK_THROWS_AS(storage.editar_titulo(5, "Estudar cálculo"), IdTarefaNaoExiste);
    CHECK_THROWS_AS(storage.editar_descricao(5, "Instalar o doctest"), IdTarefaNaoExiste);
    CHECK_THROWS_AS(storage.editar_prazo(5, "10/12/2022"), IdTarefaNaoExiste);
    CHECK_THROWS_AS(storage.editar_grupo(5, "Faculdade"), IdTarefaNaoExiste);
    CHECK_THROWS_AS(storage.editar_prioridade(5, "Urgente"), IdTarefaNaoExiste);
    CHECK_THROWS_AS(storage.alterar_concluido(5, true), IdTarefaNaoExiste);
}

TEST_CASE("Testando edição de grupo no storage") {
    CHECK(storage.alterar_grupo(1, "Academia"));
    CHECK(storage.alterar_des_grupo(1, "Exercícios e alimentação"));

    CHECK_THROWS_AS(storage.alterar_grupo(1, "Casa"), GrupoJaExiste);

    CHECK_THROWS_AS(storage.alterar_grupo(5, "Casa"), IdGrupoNaoExiste);
    CHECK_THROWS_AS(storage.alterar_des_grupo(5, "Tarefas da academia"), IdGrupoNaoExiste);
}

TEST_CASE("Testando edição de prioridade no storage") {
    CHECK(storage.alterar_prioridade(2, "Média"));
    CHECK(storage.alterar_nivel(2, 2));

    CHECK_THROWS_AS(storage.alterar_prioridade(2, "Urgente"), PrioridadeJaExiste);

    CHECK_THROWS_AS(storage.alterar_prioridade(5, "Baixa"), IdPrioridadeNaoExiste);
    CHECK_THROWS_AS(storage.alterar_nivel(5, 4), IdPrioridadeNaoExiste);
}

TEST_CASE("Testando a exclusão de tarefa no storage") {
    CHECK(storage.apaga_tarefa(1));

    CHECK_THROWS_AS(storage.apaga_tarefa(5), IdTarefaNaoExiste);
}

TEST_CASE("Testando a exclusão de grupo no storage") {
    // Apaga o grupo academia
    CHECK(storage.apaga_grupo(1));

    CHECK_THROWS_AS(storage.apaga_grupo(5), IdGrupoNaoExiste);
    // Tenta apagar o grupo Casa que tem tarefas vinculadas a ele
    CHECK_THROWS_AS(storage.apaga_grupo(1), ErroAoApagarGrupo);
}

TEST_CASE("Testando a exclusão de prioridade no storage") {
    // Apaga a prioridade Urgente
    CHECK(storage.apaga_prioridade(1));

    CHECK_THROWS_AS(storage.apaga_prioridade(5), IdPrioridadeNaoExiste);
    // Tenta apagar a prioridade Média que tem tarefas vinculadas a ele
    CHECK_THROWS_AS(storage.apaga_prioridade(1), ErroAoApagarPrioridade);
}

TEST_CASE("Testando a exclusão de usuário no storage") {
    CHECK(storage.exclui_usuario("usuario_teste"));
    CHECK_FALSE(storage.exclui_usuario("usuario_inexistente"));
}

/*-----------------------------------------------------------------------------------------
Testes no terminal
-----------------------------------------------------------------------------------------*/
Terminal terminal = Terminal();

TEST_CASE("Testando a criação de usuário no terminal") {
    CHECK(terminal.cria_usuario(vector<string>{"usuario_teste", "123", "123"}));
}

TEST_CASE("Testando o login no terminal") {
    CHECK(terminal.faz_login(vector<string>{"usuario_teste", "123"}));
}

TEST_CASE("Testando a criação de grupo no terminal") {
    CHECK(terminal.novo_grupo(vector<string>{"Faculdade", "Tarefas da faculdade"}));
}

TEST_CASE("Testando a criação de prioridade no terminal") {
    CHECK(terminal.nova_prioridade(vector<string>{"Urgente", "1"}));
}

TEST_CASE("Testando a criação de tarefa no terminal") {
    CHECK(terminal.nova_tarefa(vector<string>{"Escrever testes", "Escrever testes no trabalho de PDS", "06/12/2022", "Urgente", "Faculdade"}));
}

TEST_CASE("Testando a edição de título de tarefa no terminal") {
    CHECK_THROWS_AS(terminal.editar_titulo_tarefa("ett 76", vector<string>{"Criar testes"}), IdTarefaNaoExiste);
    CHECK_THROWS_AS(terminal.editar_titulo_tarefa("ett Escrever testes", vector<string>{"Criar testes"}), ComandoEditarTarefaErrado);
    CHECK_THROWS_AS(terminal.editar_titulo_tarefa("ett", vector<string>{"Criar testes"}), ComandoEditarTarefaErrado);

    CHECK(terminal.editar_titulo_tarefa("ett 1", vector<string>{"Criar testes"}));
}

TEST_CASE("Testando a edição de descrição de tarefa no terminal") {
    CHECK_THROWS_AS(terminal.editar_desc_tarefa("edt 76", vector<string>{"Criar testes"}), IdTarefaNaoExiste);
    CHECK_THROWS_AS(terminal.editar_desc_tarefa("edt Criar testes", vector<string>{"Criar testes"}), ComandoEditarTarefaErrado);
    CHECK_THROWS_AS(terminal.editar_desc_tarefa("edt", vector<string>{"Criar testes"}), ComandoEditarTarefaErrado);

    CHECK(terminal.editar_desc_tarefa("edt 1", vector<string>{"Criar testes"}));
}

TEST_CASE("Testando a edição de prazo de tarefa no terminal") {
    CHECK_THROWS_AS(terminal.editar_prazo_tarefa("epzt 76", vector<string>{"30/12/2022"}), IdTarefaNaoExiste);
    CHECK_THROWS_AS(terminal.editar_prazo_tarefa("epzt Criar testes", vector<string>{"30/12/2022"}), ComandoEditarTarefaErrado);
    CHECK_THROWS_AS(terminal.editar_prazo_tarefa("epzt", vector<string>{"30/12/2022"}), ComandoEditarTarefaErrado);

    CHECK(terminal.editar_prazo_tarefa("epzt 1", vector<string>{"30/12/2022"}));
}

TEST_CASE("Testando a edição de prioridade de tarefa no terminal") {
    CHECK_THROWS_AS(terminal.editar_prd_tarefa("ept 76", vector<string>{"Urgente"}), IdTarefaNaoExiste);
    CHECK_THROWS_AS(terminal.editar_prd_tarefa("ept Criar testes", vector<string>{"Urgente"}), ComandoEditarTarefaErrado);
    CHECK_THROWS_AS(terminal.editar_prd_tarefa("ept", vector<string>{"Urgente"}), ComandoEditarTarefaErrado);

    CHECK(terminal.editar_prd_tarefa("ept 1", vector<string>{"Urgente"}));
}

TEST_CASE("Testando a edição de grupo de tarefa no terminal") {
    CHECK_THROWS_AS(terminal.editar_grupo_tarefa("egt 76", vector<string>{"Faculdade"}), IdTarefaNaoExiste);
    CHECK_THROWS_AS(terminal.editar_grupo_tarefa("egt Criar testes", vector<string>{"Faculdade"}), ComandoEditarTarefaErrado);
    CHECK_THROWS_AS(terminal.editar_grupo_tarefa("egt", vector<string>{"Faculdade"}), ComandoEditarTarefaErrado);

    CHECK(terminal.editar_grupo_tarefa("egt 1", vector<string>{"Faculdade"}));
}

TEST_CASE("Testando a edição marcar tarefa como concluída no terminal") {
    CHECK_THROWS_AS(terminal.marcar_concluida("check 76"), IdTarefaNaoExiste);
    CHECK_THROWS_AS(terminal.marcar_concluida("check Criar testes"), ComandoEditarTarefaErrado);
    CHECK_THROWS_AS(terminal.marcar_concluida("check"), ComandoEditarTarefaErrado);

    CHECK(terminal.marcar_concluida("check 1"));
}

TEST_CASE("Testando a edição desmarcar tarefa como concluída no terminal") {
    CHECK_THROWS_AS(terminal.desmarcar_concluida("check 76"), IdTarefaNaoExiste);
    CHECK_THROWS_AS(terminal.desmarcar_concluida("check Criar testes"), ComandoEditarTarefaErrado);
    CHECK_THROWS_AS(terminal.desmarcar_concluida("check"), ComandoEditarTarefaErrado);

    CHECK(terminal.marcar_concluida("uncheck 1"));
}

TEST_CASE("Testando a edição de nome de grupo no terminal") {
    CHECK_THROWS_AS(terminal.editar_nome_grupo("eng 61", vector<string>{"Universidade"}), IdGrupoNaoExiste);
    CHECK_THROWS_AS(terminal.editar_nome_grupo("eng Faculdade", vector<string>{"Universidade"}), ComandoEditarGrupoErrado);
    CHECK_THROWS_AS(terminal.editar_nome_grupo("eng", vector<string>{"Universidade"}), ComandoEditarGrupoErrado);

    CHECK(terminal.editar_nome_grupo("eng 1", vector<string>{"Universidade"}));
}

TEST_CASE("Testando a edição de descrição de grupo no terminal") {
    CHECK_THROWS_AS(terminal.editar_desc_grupo("edg 61", vector<string>{"Tarefas da universidade"}), IdGrupoNaoExiste);
    CHECK_THROWS_AS(terminal.editar_desc_grupo("edg Universidade", vector<string>{"Tarefas da universidade"}), ComandoEditarGrupoErrado);
    CHECK_THROWS_AS(terminal.editar_desc_grupo("edg", vector<string>{"Tarefas da universidade"}), ComandoEditarGrupoErrado);

    CHECK(terminal.editar_desc_grupo("edg 1", vector<string>{"Tarefas da universidade"}));
}

TEST_CASE("Testando a edição de nome de prioridade no terminal") {
    CHECK_THROWS_AS(terminal.editar_nome_prd("enp 61", vector<string>{"Crucial"}), IdPrioridadeNaoExiste);
    CHECK_THROWS_AS(terminal.editar_nome_prd("enp Urgente", vector<string>{"Crucial"}), ComandoEditarPrioridadeErrado);
    CHECK_THROWS_AS(terminal.editar_nome_prd("enp", vector<string>{"Crucial"}), ComandoEditarPrioridadeErrado);

    CHECK(terminal.editar_nome_prd("enp 1", vector<string>{"Crucial"}));
}

TEST_CASE("Testando a edição de nível de prioridade no terminal") {
    CHECK_THROWS_AS(terminal.editar_nivel_prd("evp 61", vector<string>{"2"}), IdPrioridadeNaoExiste);
    CHECK_THROWS_AS(terminal.editar_nivel_prd("evp Urgente", vector<string>{"2"}), ComandoEditarPrioridadeErrado);
    CHECK_THROWS_AS(terminal.editar_nivel_prd("evp", vector<string>{"2"}), ComandoEditarPrioridadeErrado);

    CHECK(terminal.editar_nivel_prd("evp 1", vector<string>{"2"}));
}

TEST_CASE("Testando a exclusão de tarefa no terminal") {
    CHECK_THROWS_AS(terminal.deletar_tarefa("dt 15"), IdTarefaNaoExiste);
    CHECK_THROWS_AS(terminal.deletar_tarefa("dt Criar testes"), ComandoExcluirTarefaErrado);
    CHECK_THROWS_AS(terminal.deletar_tarefa("dt"), ComandoExcluirTarefaErrado);

    CHECK(terminal.deletar_tarefa("dt 1"));
}

TEST_CASE("Testando a exclusão de prioridade no terminal") {
    CHECK_THROWS_AS(terminal.deletar_prioridade("dp 32"), IdPrioridadeNaoExiste);
    CHECK_THROWS_AS(terminal.deletar_prioridade("dp Crucial"), ComandoExcluirPrioridadeErrado);
    CHECK_THROWS_AS(terminal.deletar_prioridade("dp"), ComandoExcluirPrioridadeErrado);

    CHECK(terminal.deletar_prioridade("dp 1"));
}

TEST_CASE("Testando a exclusão de grupo no terminal") {
    CHECK_THROWS_AS(terminal.deletar_grupo("dg 27"), IdGrupoNaoExiste);
    CHECK_THROWS_AS(terminal.deletar_grupo("dg Universidade"), ComandoExcluirGrupoErrado);
    CHECK_THROWS_AS(terminal.deletar_grupo("dg"), ComandoExcluirGrupoErrado);

    CHECK(terminal.deletar_grupo("dg 1"));
}

TEST_CASE("Excluindo arquivo de teste") {
    storage.confere_senha("usuario_teste", "123");
    storage.exclui_usuario("usuario_teste");
    system("clear");
}