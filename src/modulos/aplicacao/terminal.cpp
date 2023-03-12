#include "terminal.hpp"
#include <algorithm>

Terminal::Terminal() {
    // Define o idioma para português
    setlocale(LC_ALL, "pt_BR.utf8");
    // Pega as dimensões do terminal
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    _largura_terminal = w.ws_col;
    _altura_terminal = w.ws_row;
    _padding_x = _largura_terminal / 40;
    _padding_y = _altura_terminal / 15;
    if((_altura_terminal / 3) > 0) {
        _tarefas_por_pagina = _altura_terminal / 3;
    }
    else {
        _tarefas_por_pagina = 1;
    }
    _usuario = Usuario();
    _storage = new Storage();
    _pagina_atual = 1;
}

bool Terminal::print(string msg, bool quebra_de_linha = true) {
    cout << espaco_horizontal() << msg;
    if(quebra_de_linha) {
        cout << "\n";
    }
    return true;
}

bool Terminal::print_sucesso(string msg_sucesso) {
    cout << espaco_horizontal() << TEXTO_VERDE << msg_sucesso << "\n" << TEXTO_NORMAL;
    return true;
}

bool Terminal::print_erro(string msg_erro) {
    cout << espaco_horizontal() << TEXTO_VERMELHO << msg_erro << "\n" << TEXTO_NORMAL;
    return true;
}

bool Terminal::print_centralizado(string texto) {
    cout << StringUtil::centraliza_str(texto, _largura_terminal) << "\n";
    return true;
}

bool Terminal::print_direita(string texto) {
    int espacos_a_esquerda = (_largura_terminal - StringUtil::tamanho(texto) - _padding_x/2);
    cout << string(espacos_a_esquerda, ' ') << texto << "\n";
    return true;
}

string Terminal::espaco_vertical() {
    return string(_padding_y, '\n');
}

string Terminal::espaco_horizontal() {
    return string(_padding_x, ' ');
}

string Terminal::get_entrada() {
    char input[100];
    cin.getline(input, 100);
    string entrada;
    for(int i = 0; input[i] != 0x00; i++) {
        entrada += input[i];
    }
    return entrada;
}

bool Terminal::limpa_terminal() {
    system("clear");
    cout << espaco_vertical();
    print_centralizado("Gerenciador de Tarefas");
    cout << espaco_vertical();
    return true;
}

bool Terminal::login_ou_registre() {
    // Pergunta se o usuário deseja fazer login ou registrar um novo usuário
    char acao_inicial = ' ';
    while (tolower(acao_inicial) != 'e' && tolower(acao_inicial) != 'r') {
        print("Pressione 'e' para fazer login ou 'r' para registrar um novo usuário: ", false);
        acao_inicial = get_entrada().c_str()[0];;
        if(acao_inicial == 'e') {
            faz_login();
            break;
        }
        else if(acao_inicial == 'r') {
            cria_usuario();
            acao_inicial = ' ';
        }
        else {
            limpa_terminal();
            print_erro("Entrada inválida. Tente novamente.");
        }
    }
    return true;
}

bool Terminal::cria_usuario(vector<string> entradas) {
    limpa_terminal();
    string nome_usuario;
    while(nome_usuario.size() == 0) {
        print("Digite o nome do usuário (sem espaços ou caracteres inválidos)");
        print("Usuário: ", false);
        try {
            if(entradas.size() == 0) {
                nome_usuario = get_entrada();
            }
            else {
                nome_usuario = entradas[0];
            }
            // Caso o nome informado possua algum caractere inválido
            if((int) nome_usuario.find_first_of("\\/:?\"<>| ") != -1) {
                throw CaractereInvalido();
            }
            // Caso o usuário já exista
            if(_storage->usuario_existe(nome_usuario)) {
                throw UsuarioJaExiste();
            }
            string senha, confirmar_senha;
            while(senha.size() == 0) {
                print("Senha: ", false);

                if(entradas.size() == 0) {
                    senha = get_entrada();
                }
                else {
                    senha = entradas[1];
                }

                print("Confirmar senha: ", false);

                if(entradas.size() == 0) {
                    confirmar_senha = get_entrada();
                }
                else {
                    confirmar_senha = entradas[2];
                }

                if(senha == confirmar_senha) {
                    _storage->cria_usuario(nome_usuario, senha);
                    limpa_terminal();
                    print_sucesso("Usuário criado com sucesso.");
                }
                else {
                    senha = "";
                    limpa_terminal();
                    print_erro("As senhas não coincidem. Tente novamente.");
                }
            }
        } catch (const exception& e) {
            nome_usuario = "";
            limpa_terminal();
            print_erro(e.what());
        }
    }
    return true;
}

bool Terminal::faz_login(vector<string> entradas) {
    limpa_terminal();
    while(_usuario.get_nome().size() == 0) {
        print("Usuário: ", false);
        try {
            string nome_usuario;
            if(entradas.size() == 0) {
                nome_usuario = get_entrada();
            }
            else {
                nome_usuario = entradas[0];
            }
            // Caso o usuário não exista
            if(!_storage->usuario_existe(nome_usuario)) {
                throw UsuarioNaoEncontrado();
            }
            print("Senha: ", false);

            string senha;
            if(entradas.size() == 0) {
                senha = get_entrada();
            }
            else {
                senha = entradas[1];
            }

            _usuario = Usuario(nome_usuario, senha);
            limpa_terminal();
            print_sucesso("Login feito com sucesso.\n");
        } catch (const exception& e) {
            limpa_terminal();
            print_erro(e.what());
        }
    }
    _storage = _usuario.get_storage();
    return true;
}

bool Terminal::tem_tarefas() {
    int qtd_tarefas = _storage->get_qtd_tarefas();
    if(qtd_tarefas > 0) return true;
    else return false;
}

int Terminal::get_qtd_paginas() {
    int qtd_tarefas = _storage->get_qtd_tarefas();
    int paginas_total;
    if(qtd_tarefas > 0) {
        // Divide a quantidade de tarefas pelo máximo por página
        paginas_total = qtd_tarefas / _tarefas_por_pagina;
        // Arredonda para cima se necessário
        if(qtd_tarefas % _tarefas_por_pagina) paginas_total++;  
    }
    else {
        paginas_total = 1;
    }
    return paginas_total;
}

int Terminal::get_qtd_paginas_grupos() {
    int qtd_grupos = _storage->get_qtd_grupos();
    int paginas_total;
    if(qtd_grupos > 0) {
        // Divide a quantidade de grupos pelo máximo por página
        paginas_total = qtd_grupos / _tarefas_por_pagina;
        // Arredonda para cima se necessário
        if(qtd_grupos % _tarefas_por_pagina) paginas_total++;
    }
    else {
        paginas_total = 1;
    }
    return paginas_total;
}

int Terminal::get_qtd_paginas_prd() {
    int qtd_prioridades = _storage->get_qtd_prioridades();
    int paginas_total;
    if(qtd_prioridades > 0) {
        // Divide a quantidade de grupos pelo máximo por página
        paginas_total = qtd_prioridades / _tarefas_por_pagina;
        // Arredonda para cima se necessário
        if(qtd_prioridades % _tarefas_por_pagina) paginas_total++;
    }
    else {
        paginas_total = 1;
    }
    return paginas_total;
}

bool Terminal::print_ajuda() {
    limpa_terminal();
    print("Lista de comandos:\n");
    print(string(TEXTO_ROXO) + "Exibir" + string(TEXTO_NORMAL));
    print("\th - Exibir lista de comandos");
    print("\t{número} - Exibir a página {número} das tarefas");
    print("\tpg {número} - Exibir a página {número} dos grupos");
    print("\tpp {número} - Exibir a página {número} das prioridades");
    print("\tt {ID} - Exibir a tarefa {ID}");
    print(string(TEXTO_ROXO) + "Criar" + string(TEXTO_NORMAL));
    print("\tnt - Criar uma nova tarefa");
    print("\tng - Criar um novo grupo");
    print("\tnp - Criar uma nova prioridade");
    print(string(TEXTO_ROXO) + "Editar tarefa" + string(TEXTO_NORMAL));
    print("\tett {ID} - Editar título da tarefa {ID}");
    print("\tedt {ID} - Editar descrição da tarefa {ID}");
    print("\tepzt {ID} - Editar prazo de conclusão da tarefa {ID}");
    print("\tegt {ID} - Editar grupo da tarefa {ID}");
    print("\tept {ID} - Editar prioridade da tarefa {ID}");
    print(string(TEXTO_ROXO) + "Editar grupo" + string(TEXTO_NORMAL));
    print("\teng {ID_GRUPO} - Editar nome do grupo {ID_GRUPO}");
    print("\tedg {ID_GRUPO} - Editar descrição do grupo {ID_GRUPO}");
    print(string(TEXTO_ROXO) + "Editar prioridade" + string(TEXTO_NORMAL));
    print("\tenp {ID_PRIORIDADE} - Editar nome da prioridade {ID_PRIORIDADE}");
    print("\tevp {ID_PRIORIDADE} - Editar nível da prioridade {ID_PRIORIDADE}");
    print(string(TEXTO_ROXO) + "Excluir" + string(TEXTO_NORMAL));
    print("\tdt {ID} - Excluir tarefa {ID}");
    print("\tdg {ID_GRUPO} - Excluir grupo {ID_GRUPO}");
    print("\tdp {ID_PRIORIDADE} - Excluir prioridade {ID_PRIORIDADE}");
    print(string(TEXTO_ROXO) + "Finalizar" + string(TEXTO_NORMAL));
    print("\tout - Logout");
    print("\tq - Finalizar programa\n");
    return true;
}

bool Terminal::print_paginacao(int pagina_atual) {
    string paginacao = "Página " + to_string(pagina_atual) + "/" + to_string(get_qtd_paginas()) +
    " (Digite o número da página que deseja exibir)";
    cout << "\n";
    print_direita(paginacao);
    return true;
}

bool Terminal::print_paginacao_grupos(int pagina_grupo) {
    string paginacao = "Página " + to_string(pagina_grupo) + "/" + to_string(get_qtd_paginas_grupos()) +
    " (Digite pg + o número da página que deseja exibir)";
    cout << "\n";
    print_direita(paginacao);
    return true;
}

bool Terminal::print_paginacao_prd(int pagina_prioridade) {
    string paginacao = "Página " + to_string(pagina_prioridade) + "/" + to_string(get_qtd_paginas_prd()) +
    " (Digite pp + o número da página que deseja exibir)";
    cout << "\n";
    print_direita(paginacao);
    return true;
}

bool Terminal::print_cabecalho() {
    string cabecalho = string(TEXTO_GRIFADO) + "   "
    + StringUtil::centraliza_str("ID", 4) 
    + StringUtil::centraliza_str("Título", 32) 
    + StringUtil::centraliza_str("Prazo", 12) 
    + StringUtil::centraliza_str("Concluída", 11) 
    + StringUtil::centraliza_str("Grupo", 17) 
    + StringUtil::centraliza_str("Prioridade", 17) +
    string(TEXTO_NORMAL);
    print_centralizado("Tarefas");
    cout << "\n";
    print_centralizado(cabecalho);
    cout << "\n";
    return true;
}

bool Terminal::print_cabecalho_grupo() {
    string cabecalho = string(TEXTO_GRIFADO) + "   "
    + StringUtil::centraliza_str("ID_GRUPO", 10) 
    + StringUtil::centraliza_str("Nome", 32) 
    + StringUtil::centraliza_str("Descrição", 51) +
    string(TEXTO_NORMAL);
    print_centralizado("Grupos");
    cout << "\n";
    print_centralizado(cabecalho);
    cout << "\n";
    return true;
}

bool Terminal::print_cabecalho_prd() {
    string cabecalho = string(TEXTO_GRIFADO) + "   "
    + StringUtil::centraliza_str("ID_PRIORIDADE", 19) 
    + StringUtil::centraliza_str("Nome", 32) 
    + StringUtil::centraliza_str("Nível da prioridade", 42) +
    string(TEXTO_NORMAL);
    print_centralizado("Prioridades");
    cout << "\n";
    print_centralizado(cabecalho);
    cout << "\n";
    return true;
}

bool Terminal::pagina_existe(int pagina) {
    if(pagina <= get_qtd_paginas()) return true;
    else return false;
}

bool Terminal::pagina_grupo_existe(int pagina) {
    if(pagina <= get_qtd_paginas_grupos()) return true;
    else return false;
}

bool Terminal::pagina_prd_existe(int pagina) {
    if(pagina <= get_qtd_paginas_prd()) return true;
    else return false;
}

bool Terminal::print_tarefa(Tarefa *tarefa, int id) {
    string str_id = to_string(id);
    string titulo = tarefa->get_titulo();
    string prazo = tarefa->get_prazo();
    string concluido;
    if(tarefa->get_concluido()) {
        concluido = "Sim";
    }
    else {
        concluido = "Não";
    }
    string grupo = tarefa->get_nome_grupo();
    string prioridade = tarefa->get_nome_prioridade();

    // Define qual vai ser a cor da tarefa
    string cor = TEXTO_NORMAL;
    if(tarefa->prazo_passou()) {
        cor = TEXTO_AMARELO;
    }
    // Printa em cinza caso a tarefa tenha sido concluída
    if(tarefa->get_concluido()) {
        cor = TEXTO_CINZA;
    }

    string linha = "   "
    + cor
    + StringUtil::centraliza_str(str_id, 4) 
    + StringUtil::centraliza_str(titulo, 32) 
    + StringUtil::centraliza_str(prazo, 12) 
    + StringUtil::centraliza_str(concluido, 11) 
    + StringUtil::centraliza_str(grupo, 17) 
    + StringUtil::centraliza_str(prioridade, 17)
    + TEXTO_NORMAL;
    // Printa em amarelo caso o prazo tenha passado
    print_centralizado(linha);
    return true;
}

bool Terminal::print_tarefas(int pagina) {
    if(pagina_existe(pagina)) {
        // Define quais vão ser a primeira e a última tarefa mostradas na página
        int primeira_tarefa = _tarefas_por_pagina * (pagina - 1) + 1;
        int ultima_tarefa;
        if(pagina_existe(pagina + 1)) {
            ultima_tarefa = primeira_tarefa + (_tarefas_por_pagina - 1);
        }
        else {
            ultima_tarefa = _storage->get_qtd_tarefas();
        }

        // Printa a página
        print_cabecalho();
        for(int i = primeira_tarefa; i <= ultima_tarefa; i++) {
            print_tarefa(_storage->get_tarefa(i), i);
        }
        print_paginacao(pagina);
    }
    else {
        throw PaginaInvalida();
    }
    return true;
}

bool Terminal::exibe_tarefa(string comando) {
    vector<string> comando_dividido = StringUtil::split_str(comando, " ");

    // Verifica se foi passado um ID válido da tarefa
    string id_tarefa;
    if(comando_dividido.size() > 1) {
        id_tarefa = StringUtil::split_str(comando, " ")[1];
    }
    else {
        throw ComandoExibirTarefaErrado();
    }
    if(!StringUtil::str_numerica(id_tarefa)) {
        throw ComandoExibirTarefaErrado();
    }
    if(stoi(id_tarefa) < 0 || stoi(id_tarefa) > _storage->get_qtd_tarefas()) {
        throw IdTarefaNaoExiste();
    }

    Tarefa* tarefa = _storage->get_tarefa(stoi(id_tarefa));
    string titulo = tarefa->get_titulo();
    string descricao = tarefa->get_descricao();
    string prazo = tarefa->get_prazo();
    string concluido;
    if(tarefa->get_concluido()) {
        concluido = "Sim";
    }
    else {
        concluido = "Não";
    }
    string grupo = tarefa->get_nome_grupo();
    string prioridade = tarefa->get_nome_prioridade();

    limpa_terminal();
    print_centralizado("Tarefa " + id_tarefa + "\n");
    print("Título: " + titulo);
    print("Descrição: " + descricao);
    print("Grupo: " + grupo);
    print("Prioridade: " + prioridade);
    print("Prazo de conclusão: " + prazo);
    print("Concluída: " + concluido + "\n");
    return true;
}

bool Terminal::print_grupo(Grupo* grupo, int id) {
    string str_id = to_string(id);
    string nome = grupo->get_nome_etiqueta();
    string descricao = grupo->get_descricao();

    string linha = "   "
    + string(TEXTO_NORMAL)
    + StringUtil::centraliza_str(str_id, 10) 
    + StringUtil::centraliza_str(nome, 32) 
    + StringUtil::centraliza_str(descricao, 51)
    + TEXTO_NORMAL;

    print_centralizado(linha);
    return true;
}

bool Terminal::print_grupos(int pagina) {
    if(pagina_grupo_existe(pagina)) {
        // Define quais vão ser o primeiro e o último grupos mostrados na página
        int primeiro_grupo = _tarefas_por_pagina * (pagina - 1) + 1;
        int ultimo_grupo;
        if(pagina_existe(pagina + 1)) {
            ultimo_grupo = primeiro_grupo + (_tarefas_por_pagina - 1);
        }
        else {
            ultimo_grupo = _storage->get_qtd_grupos();
        }

        // Printa a página
        print_cabecalho_grupo();
        for(int i = primeiro_grupo; i <= ultimo_grupo; i++) {
            print_grupo(_storage->get_grupo(i), i);
        }
        print_paginacao_grupos(pagina);
    }
    else {
        throw PaginaInvalida();
    }
    return true;
}

bool Terminal::print_prioridade(Prioridade* prioridade, int id) {
    string str_id = to_string(id);
    string nome = prioridade->get_nome_etiqueta();
    string nivel_prioridade = to_string(prioridade->get_nivel_prioridade());

    string linha = "   "
    + string(TEXTO_NORMAL)
    + StringUtil::centraliza_str(str_id, 19)
    + StringUtil::centraliza_str(nome, 32) 
    + StringUtil::centraliza_str(nivel_prioridade, 42)
    + TEXTO_NORMAL;

    print_centralizado(linha);
    return true;
}

bool Terminal::print_prioridades(int pagina) {
    if(pagina_prd_existe(pagina)) {
        // Define quais vão ser o primeiro e o último grupos mostrados na página
        int primeira_prioridade = _tarefas_por_pagina * (pagina - 1) + 1;
        int ultima_prioridade;
        if(pagina_prd_existe(pagina + 1)) {
            ultima_prioridade = primeira_prioridade + (_tarefas_por_pagina - 1);
        }
        else {
            ultima_prioridade = _storage->get_qtd_prioridades();
        }

        // Printa a página
        print_cabecalho_prd();
        for(int i = primeira_prioridade; i <= ultima_prioridade; i++) {
            print_prioridade(_storage->get_prioridade(i), i);
        }
        print_paginacao_prd(pagina);
    }
    else {
        throw PaginaInvalida();
    }
    return true;
}

bool Terminal::prazo_valido(string prazo) {
    // Retorna false se o prazo não tiver o número certo de caracteres
    if(prazo.size() != 10) return false;

    // Retorna false caso dia, mês ou ano não sejam numéricos
    if(!StringUtil::str_numerica(prazo.substr(0, 2))) return false;
    if(!StringUtil::str_numerica(prazo.substr(3, 2))) return false;
    if(!StringUtil::str_numerica(prazo.substr(6, 4))) return false;

    // Retorna false caso dia, mês e ano não sejam separados por barras
    if(prazo[2] != '/' || prazo[5] != '/') return false;

    int dia = stoi(prazo.substr(0, 2));
    int mes = stoi(prazo.substr(3, 2));
    int ano = stoi(prazo.substr(6, 4));
    // Retorna false se mês é inválido
    if(mes > 12) return false;

    // Retorna false se o dia do mês não existe
    int meses_31[] = {1, 3, 5, 7, 8, 10, 12};
    int meses_30[] = {4, 6, 9, 11};
    int fevereiro = 2;
    // Mês de 31 dias
    if(any_of(begin(meses_31), end(meses_31), [&](int n){return n == mes;})) {
        if(dia > 31) return false;
    }
    // Mês de 30 dias
    else if(any_of(begin(meses_30), end(meses_30), [&](int n){return n == mes;})) {
        if(dia > 30) return false;
    }
    // Fevereiro
    else if(mes == fevereiro) {
        // Ano normal
        if(ano % 4) {
            if(dia > 28) return false;
        }
        // Ano bissexto
        else {
            if(dia > 29) return false;
        }
    }

    // Retorna true caso seja válido
    return true;
}

bool Terminal::nova_tarefa(vector<string> entradas) {
    limpa_terminal();
    print_centralizado("Criar nova tarefa ('c' para cancelar)\n");

    // Pergunta o título da tarefa
    print("Digite o título da tarefa (máximo 30 caracteres): ", false);
    string titulo_tarefa;
    while(titulo_tarefa.size() == 0) {
        
        if(entradas.size() == 0) {
            titulo_tarefa = get_entrada();
        }
        else {
            titulo_tarefa = entradas[0];
        }

        if(titulo_tarefa == "c") {
            throw CriacaoTarefaCancelada();
        }
        try {
            if(StringUtil::tamanho(titulo_tarefa) > 30) {
                throw NomeTarefaMuitoLongo();
            }
            if(_storage->tarefa_existe(titulo_tarefa)) {
                throw TarefaJaExiste();
            }
        }
        catch(exception& e) {
            limpa_terminal();
            print_centralizado("Criar nova tarefa\n");
            print_erro(e.what());
            print("Digite o nome da tarefa (máximo 30 caracteres): ", false);
            titulo_tarefa = "";
        }
    }

    // Pergunta a descrição da tarefa
    print("Digite a descrição da tarefa: ", false);
    
    string descricao_tarefa;
    if(entradas.size() == 0) {
        descricao_tarefa = get_entrada();
    }
    else {
        descricao_tarefa = entradas[1];
    }

    if(descricao_tarefa == "c") {
        throw CriacaoTarefaCancelada();
    }

    // Pergunta o prazo do trabalho
    print("Digite o prazo de conclusão da tarefa (DD/MM/AAAA): ", false);
    string prazo_tarefa;
    while(prazo_tarefa.size() == 0) {
        
        if(entradas.size() == 0) {
            prazo_tarefa = get_entrada();
        }
        else {
            prazo_tarefa = entradas[2];
        }

        if(prazo_tarefa == "c") {
            throw CriacaoTarefaCancelada();
        }
        try {
            if(!prazo_valido(prazo_tarefa)) {
                throw PrazoInvalido();
            }
        }
        catch(exception& e) {
            print_erro(e.what());
            print("Digite o prazo de conclusão da tarefa (DD/MM/AAAA): ", false);
            prazo_tarefa = "";
        }
    }

    // Pergunta o nome da prioridade
    print("Digite a prioridade da tarefa: ", false);
    string prioridade_tarefa;
    while(prioridade_tarefa.size() == 0) {
        
        if(entradas.size() == 0) {
            prioridade_tarefa = get_entrada();
        }
        else {
            prioridade_tarefa = entradas[3];
        }

        if(prioridade_tarefa == "c") {
            throw CriacaoTarefaCancelada();
        }
        try {
            if(!_storage->prioridade_existe(prioridade_tarefa)) {
                throw PrioridadeNaoExiste();
            }
        }
        catch(exception& e) {
            print_erro(e.what());
            print("Digite a prioridade da tarefa: ", false);
            prioridade_tarefa = "";
        }
    }

    // Pergunta o nome do grupo
    print("Digite o grupo da tarefa: ", false);
    string grupo_tarefa;
    while(grupo_tarefa.size() == 0) {
        
        if(entradas.size() == 0) {
            grupo_tarefa = get_entrada();
        }
        else {
            grupo_tarefa = entradas[4];
        }

        if(grupo_tarefa == "c") {
            throw CriacaoTarefaCancelada();
        }
        try {
            if(!_storage->grupo_existe(grupo_tarefa)) {
                throw GrupoNaoExiste();
            }
        }
        catch(exception& e) {
            print_erro(e.what());
            print("Digite o grupo da tarefa: ", false);
            grupo_tarefa = "";
        }
    }

    _storage->cria_tarefa(titulo_tarefa, descricao_tarefa, prazo_tarefa, grupo_tarefa, prioridade_tarefa);

    limpa_terminal();
    print_tarefas(_pagina_atual);
    print_sucesso("Tarefa criada com sucesso");
    return true;
}

bool Terminal::novo_grupo(vector<string> entradas) {
    limpa_terminal();
    print_centralizado("Criar novo grupo ('c' para cancelar)\n");

    // Pergunta o nome do grupo
    print("Digite o nome do grupo (máximo 15 caracteres): ", false);
    string nome_grupo;
    while(nome_grupo.size() == 0) {
        
        if(entradas.size() == 0) {
            nome_grupo = get_entrada();
        }
        else {
            nome_grupo = entradas[0];
        }

        if(nome_grupo == "c") {
            throw CriacaoGrupoCancelada();
        }
        try {
            if(StringUtil::tamanho(nome_grupo) > 15) {
                throw NomeGrupoMuitoLongo();
            }
            if(_storage->grupo_existe(nome_grupo)) {
                throw GrupoJaExiste();
            }
        }
        catch(exception& e) {
            limpa_terminal();
            print_centralizado("Criar novo grupo\n");
            print_erro(e.what());
            print("Digite o nome do grupo (máximo 15 caracteres): ", false);
            nome_grupo = "";
        }
    }

    // Pergunta a descrição do grupo
    print("Digite a descrição do grupo: ", false);
    
    string descricao_grupo;
    if(entradas.size() == 0) {
        descricao_grupo = get_entrada();
    }
    else {
        descricao_grupo = entradas[1];
    }

    if(descricao_grupo == "c") {
        throw CriacaoGrupoCancelada();
    }

    _storage->cria_grupo(nome_grupo, descricao_grupo);

    limpa_terminal();
    print_tarefas(_pagina_atual);
    print_sucesso("Grupo criado com sucesso");
    return true;
}

bool Terminal::nova_prioridade(vector<string> entradas) {
    limpa_terminal();
    print_centralizado("Criar nova prioridade ('c' para cancelar)\n");

    // Pergunta o nome da prioridade
    print("Digite o nome da prioridade (máximo 15 caracteres): ", false);
    string nome_prioridade;
    while(nome_prioridade.size() == 0) {

        if(entradas.size() == 0) {
            nome_prioridade = get_entrada();
        }
        else {
            nome_prioridade = entradas[0];
        }

        if(nome_prioridade == "c") {
            throw CriacaoPrioridadeCancelada();
        }
        try {
            if(StringUtil::tamanho(nome_prioridade) > 15) {
                throw NomePrioridadeMuitoLongo();
            }
            if(_storage->prioridade_existe(nome_prioridade)) {
                throw PrioridadeJaExiste();
            }
        }
        catch(exception& e) {
            limpa_terminal();
            print_centralizado("Criar nova prioridade\n");
            print_erro(e.what());
            print("Digite o nome da prioridade (máximo 15 caracteres): ", false);
            nome_prioridade = "";
        }
    }

    // Pergunta o valor do nível da prioridade
    print("Digite o valor do nível da prioridade (número menor -> prioridade maior): ", false);
    unsigned int nivel_prioridade = 0;
    string entrada;
    while(nivel_prioridade == 0) {
        
        if(entradas.size() == 0) {
            entrada = get_entrada();
        }
        else {
            entrada = entradas[1];
        }

        if(entrada == "c") {
            throw CriacaoPrioridadeCancelada();
        }
        try {
            if(!StringUtil::str_numerica(entrada)) {
                throw ValorPrioridadeInvalido();
            }
            nivel_prioridade = stoi(entrada);
            if(nivel_prioridade < 0) {
                throw ValorPrioridadeInvalido();
            }
        }
        catch(exception& e) {
            print_erro(e.what());
            print("Digite o grupo da tarefa: ", false);
            nivel_prioridade = 0;
        }
    }

    _storage->cria_prioridade(nome_prioridade, nivel_prioridade);

    limpa_terminal();
    print_tarefas(_pagina_atual);
    print_sucesso("Prioridade criada com sucesso");
    return true;
}

bool Terminal::editar_titulo_tarefa(string comando, vector<string> entradas) {
    vector<string> comando_dividido = StringUtil::split_str(comando, " ");

    // Verifica se foi passado um ID válido da tarefa
    string id_tarefa;
    if(comando_dividido.size() > 1) {
        id_tarefa = StringUtil::split_str(comando, " ")[1];
    }
    else {
        throw ComandoEditarTarefaErrado();
    }
    if(!StringUtil::str_numerica(id_tarefa)) {
        throw ComandoEditarTarefaErrado();
    }
    if(stoi(id_tarefa) < 0 || stoi(id_tarefa) > _storage->get_qtd_tarefas()) {
        throw IdTarefaNaoExiste();
    }

    string titulo_atual = _storage->get_tarefa(stoi(id_tarefa))->get_titulo();
    limpa_terminal();
    string acao = "Editando tarefa '" + titulo_atual + "' ('c' para cancelar)\n";
    print_centralizado(acao);

    print("Título atual: " + titulo_atual);

    // Pergunta o novo título da tarefa
    print("Digite o novo título da tarefa (máximo 30 caracteres): ", false);
    string novo_titulo;
    while(novo_titulo.size() == 0) {
        
        if(entradas.size() == 0) {
            novo_titulo = get_entrada();
        }
        else {
            novo_titulo = entradas[0];
        }

        if(novo_titulo == "c") {
            throw EdicaoTarefaCancelada();
        }
        try {
            if(StringUtil::tamanho(novo_titulo) > 30) {
                throw NomeTarefaMuitoLongo();
            }
            if(_storage->tarefa_existe(novo_titulo)) {
                throw TarefaJaExiste();
            }
        }
        catch(exception& e) {
            print_erro(e.what());
            print("Digite o novo título da tarefa (máximo 30 caracteres): ", false);
            novo_titulo = "";
        }
    }
    _storage->editar_titulo(stoi(id_tarefa), novo_titulo);
    limpa_terminal();
    print_tarefas(_pagina_atual);
    print_sucesso("Tarefa editada com sucesso.");
    return true;
}

bool Terminal::editar_desc_tarefa(string comando, vector<string> entradas) {
    vector<string> comando_dividido = StringUtil::split_str(comando, " ");

    // Verifica se foi passado um ID válido da tarefa
    string id_tarefa;
    if(comando_dividido.size() > 1) {
        id_tarefa = StringUtil::split_str(comando, " ")[1];
    }
    else {
        throw ComandoEditarTarefaErrado();
    }
    if(!StringUtil::str_numerica(id_tarefa)) {
        throw ComandoEditarTarefaErrado();
    }
    if(stoi(id_tarefa) < 0 || stoi(id_tarefa) > _storage->get_qtd_tarefas()) {
        throw IdTarefaNaoExiste();
    }

    string desc_atual = _storage->get_tarefa(stoi(id_tarefa))->get_descricao();
    limpa_terminal();
    string titulo = _storage->get_tarefa(stoi(id_tarefa))->get_titulo();
    string acao = "Editando tarefa '" + titulo + "' ('c' para cancelar)\n";
    print_centralizado(acao);

    print("Descrição atual: " + desc_atual);

    // Pergunta a nova descrição da tarefa
    print("Digite a nova descrição da tarefa: ", false);
    string nova_desc;

    if(entradas.size() == 0) {
        nova_desc = get_entrada();
    }
    else {
        nova_desc = entradas[0];
    }

    if(nova_desc == "c") {
        throw EdicaoTarefaCancelada();
    }
            
    _storage->editar_descricao(stoi(id_tarefa), nova_desc);
    limpa_terminal();
    print_tarefas(_pagina_atual);
    print_sucesso("Tarefa editada com sucesso.");
    return true;
}

bool Terminal::editar_prazo_tarefa(string comando, vector<string> entradas) {
    vector<string> comando_dividido = StringUtil::split_str(comando, " ");

    // Verifica se foi passado um ID válido da tarefa
    string id_tarefa;
    if(comando_dividido.size() > 1) {
        id_tarefa = StringUtil::split_str(comando, " ")[1];
    }
    else {
        throw ComandoEditarTarefaErrado();
    }
    if(!StringUtil::str_numerica(id_tarefa)) {
        throw ComandoEditarTarefaErrado();
    }
    if(stoi(id_tarefa) < 0 || stoi(id_tarefa) > _storage->get_qtd_tarefas()) {
        throw IdTarefaNaoExiste();
    }

    string prazo_atual = _storage->get_tarefa(stoi(id_tarefa))->get_prazo();
    limpa_terminal();
    string titulo = _storage->get_tarefa(stoi(id_tarefa))->get_titulo();
    string acao = "Editando tarefa '" + titulo + "' ('c' para cancelar)\n";
    print_centralizado(acao);

    print("Prazo atual: " + prazo_atual);

    // Pergunta o prazo do trabalho
    print("Digite o novo prazo de conclusão da tarefa (DD/MM/AAAA): ", false);
    string novo_prazo;
    while(novo_prazo.size() == 0) {
        
        if(entradas.size() == 0) {
            novo_prazo = get_entrada();
        }
        else {
            novo_prazo = entradas[0];
        }

        if(novo_prazo == "c") {
            throw EdicaoTarefaCancelada();
        }
        try {
            if(!prazo_valido(novo_prazo)) {
                throw PrazoInvalido();
            }
        }
        catch(exception& e) {
            print_erro(e.what());
            print("Digite o novo prazo de conclusão da tarefa (DD/MM/AAAA): ", false);
            novo_prazo = "";
        }
    }

    _storage->editar_prazo(stoi(id_tarefa), novo_prazo);
    limpa_terminal();
    print_tarefas(_pagina_atual);
    print_sucesso("Tarefa editada com sucesso.");
    return true;
}

bool Terminal::editar_grupo_tarefa(string comando, vector<string> entradas) {
    vector<string> comando_dividido = StringUtil::split_str(comando, " ");

    // Verifica se foi passado um ID válido da tarefa
    string id_tarefa;
    if(comando_dividido.size() > 1) {
        id_tarefa = StringUtil::split_str(comando, " ")[1];
    }
    else {
        throw ComandoEditarTarefaErrado();
    }
    if(!StringUtil::str_numerica(id_tarefa)) {
        throw ComandoEditarTarefaErrado();
    }
    if(stoi(id_tarefa) < 0 || stoi(id_tarefa) > _storage->get_qtd_tarefas()) {
        throw IdTarefaNaoExiste();
    }

    string grupo_atual = _storage->get_tarefa(stoi(id_tarefa))->get_nome_grupo();
    limpa_terminal();
    string titulo = _storage->get_tarefa(stoi(id_tarefa))->get_titulo();
    string acao = "Editando tarefa '" + titulo + "' ('c' para cancelar)\n";
    print_centralizado(acao);

    print("Grupo atual: " + grupo_atual);

    // Pergunta o nome do novo grupo
    print("Digite o novo grupo da tarefa: ", false);
    string novo_grupo;
    while(novo_grupo.size() == 0) {
        
        if(entradas.size() == 0) {
            novo_grupo = get_entrada();
        }
        else {
            novo_grupo = entradas[0];
        }

        if(novo_grupo == "c") {
            throw EdicaoTarefaCancelada();
        }
        try {
            if(!_storage->grupo_existe(novo_grupo)) {
                throw GrupoNaoExiste();
            }
        }
        catch(exception& e) {
            print_erro(e.what());
            print("Digite o novo grupo da tarefa: ", false);
            novo_grupo = "";
        }
    }

    _storage->editar_grupo(stoi(id_tarefa), novo_grupo);
    limpa_terminal();
    print_tarefas(_pagina_atual);
    print_sucesso("Tarefa editada com sucesso.");
    return true;
}

bool Terminal::editar_prd_tarefa(string comando, vector<string> entradas) {
    vector<string> comando_dividido = StringUtil::split_str(comando, " ");

    // Verifica se foi passado um ID válido da tarefa
    string id_tarefa;
    if(comando_dividido.size() > 1) {
        id_tarefa = StringUtil::split_str(comando, " ")[1];
    }
    else {
        throw ComandoEditarTarefaErrado();
    }
    if(!StringUtil::str_numerica(id_tarefa)) {
        throw ComandoEditarTarefaErrado();
    }
    if(stoi(id_tarefa) < 0 || stoi(id_tarefa) > _storage->get_qtd_tarefas()) {
        throw IdTarefaNaoExiste();
    }

    string prioridade_atual = _storage->get_tarefa(stoi(id_tarefa))->get_nome_prioridade();
    limpa_terminal();
    string titulo = _storage->get_tarefa(stoi(id_tarefa))->get_titulo();
    string acao = "Editando tarefa '" + titulo + "' ('c' para cancelar)\n";
    print_centralizado(acao);

    print("Prioridade atual: " + prioridade_atual);

    // Pergunta o nome da prioridade
    print("Digite a nova prioridade da tarefa: ", false);
    string nova_prioridade;
    while(nova_prioridade.size() == 0) {
        
        if(entradas.size() == 0) {
            nova_prioridade = get_entrada();
        }
        else {
            nova_prioridade = entradas[0];
        }

        if(nova_prioridade == "c") {
            throw EdicaoTarefaCancelada();
        }
        try {
            if(!_storage->prioridade_existe(nova_prioridade)) {
                throw PrioridadeNaoExiste();
            }
        }
        catch(exception& e) {
            print_erro(e.what());
            print("Digite a nova prioridade da tarefa: ", false);
            nova_prioridade = "";
        }
    }

    _storage->editar_prioridade(stoi(id_tarefa), nova_prioridade);
    limpa_terminal();
    print_tarefas(_pagina_atual);
    print_sucesso("Tarefa editada com sucesso.");
    return true;
}

bool Terminal::marcar_concluida(string comando, vector<string> entradas) {
    vector<string> comando_dividido = StringUtil::split_str(comando, " ");

    // Verifica se foi passado um ID válido da tarefa
    string id_tarefa;
    if(comando_dividido.size() > 1) {
        if(StringUtil::str_numerica(comando_dividido[1])) {
            id_tarefa = StringUtil::split_str(comando, " ")[1];
        }
        else {
            throw ComandoEditarTarefaErrado();
        }
    }
    else {
        throw ComandoEditarTarefaErrado();
    }
    if(stoi(id_tarefa) < 0 || stoi(id_tarefa) > _storage->get_qtd_tarefas()) {
        throw IdTarefaNaoExiste();
    }

    _storage->alterar_concluido(stoi(id_tarefa), true);
    limpa_terminal();
    print_tarefas(_pagina_atual);
    return true;
}

bool Terminal::desmarcar_concluida(string comando, vector<string> entradas) {
    vector<string> comando_dividido = StringUtil::split_str(comando, " ");

    // Verifica se foi passado um ID válido da tarefa
    string id_tarefa;
    if(comando_dividido.size() > 1) {
        if(StringUtil::str_numerica(comando_dividido[1])) {
            id_tarefa = StringUtil::split_str(comando, " ")[1];
        }
        else {
            throw ComandoEditarTarefaErrado();
        }
    }
    else {
        throw ComandoEditarTarefaErrado();
    }
    if(stoi(id_tarefa) < 0 || stoi(id_tarefa) > _storage->get_qtd_tarefas()) {
        throw IdTarefaNaoExiste();
    }

    _storage->alterar_concluido(stoi(id_tarefa), false);
    limpa_terminal();
    print_tarefas(_pagina_atual);
    return true;
}

bool Terminal::editar_nome_grupo(string comando, vector<string> entradas) {
    vector<string> comando_dividido = StringUtil::split_str(comando, " ");

    // Verifica se foi passado um ID válido do grupo
    string id_grupo;
    if(comando_dividido.size() > 1) {
        id_grupo = StringUtil::split_str(comando, " ")[1];
    }
    else {
        throw ComandoEditarGrupoErrado();
    }
    if(!StringUtil::str_numerica(id_grupo)) {
        throw ComandoEditarGrupoErrado();
    }
    if(stoi(id_grupo) < 0 || stoi(id_grupo) > _storage->get_qtd_grupos()) {
        throw IdGrupoNaoExiste();
    }

    string nome_atual = _storage->get_grupo(stoi(id_grupo))->get_nome_etiqueta();
    limpa_terminal();
    string acao = "Editando grupo '" + nome_atual + "' ('c' para cancelar)\n";
    print_centralizado(acao);

    print("Nome atual: " + nome_atual);

    // Pergunta o nome do grupo
    print("Digite o novo nome do grupo (máximo 15 caracteres): ", false);
    string novo_nome;
    while(novo_nome.size() == 0) {
        
        if(entradas.size() == 0) {
            novo_nome = get_entrada();
        }
        else {
            novo_nome = entradas[0];
        }

        if(novo_nome == "c") {
            throw EdicaoGrupoCancelada();
        }
        try {
            if(StringUtil::tamanho(novo_nome) > 15) {
                throw NomeGrupoMuitoLongo();
            }
            if(_storage->grupo_existe(novo_nome)) {
                throw GrupoJaExiste();
            }
        }
        catch(exception& e) {
            print_erro(e.what());
            print("Digite o novo nome do grupo (máximo 15 caracteres): ", false);
            novo_nome = "";
        }
    }

    _storage->alterar_grupo(stoi(id_grupo), novo_nome);
    limpa_terminal();
    print_tarefas(_pagina_atual);
    print_sucesso("Grupo editado com sucesso.");
    return true;
}

bool Terminal::editar_desc_grupo(string comando, vector<string> entradas) {
    vector<string> comando_dividido = StringUtil::split_str(comando, " ");

    // Verifica se foi passado um ID válido do grupo
    string id_grupo;
    if(comando_dividido.size() > 1) {
        id_grupo = StringUtil::split_str(comando, " ")[1];
    }
    else {
        throw ComandoEditarGrupoErrado();
    }
    if(!StringUtil::str_numerica(id_grupo)) {
        throw ComandoEditarGrupoErrado();
    }
    if(stoi(id_grupo) < 0 || stoi(id_grupo) > _storage->get_qtd_grupos()) {
        throw IdGrupoNaoExiste();
    }

    string desc_atual = _storage->get_grupo(stoi(id_grupo))->get_descricao();
    limpa_terminal();
    string nome = _storage->get_grupo(stoi(id_grupo))->get_nome_etiqueta();
    string acao = "Editando grupo '" + nome + "' ('c' para cancelar)\n";
    print_centralizado(acao);

    print("Descrição atual: " + desc_atual);

    // Pergunta a nova descrição do grupo
    print("Digite a nova descrição do grupo: ", false);
    
    string nova_descricao;
    if(entradas.size() == 0) {
        nova_descricao = get_entrada();
    }
    else {
        nova_descricao = entradas[0];
    }

    if(nova_descricao == "c") {
        throw EdicaoGrupoCancelada();
    }

    _storage->alterar_des_grupo(stoi(id_grupo), nova_descricao);
    limpa_terminal();
    print_tarefas(_pagina_atual);
    print_sucesso("Grupo editado com sucesso.");
    return true;
}

bool Terminal::editar_nome_prd(string comando, vector<string> entradas) {
    vector<string> comando_dividido = StringUtil::split_str(comando, " ");

    // Verifica se foi passado o ID da prioridade
    string id_prioridade;
    if(comando_dividido.size() > 1) {
        id_prioridade = StringUtil::split_str(comando, " ")[1];
    }
    else {
        throw ComandoEditarPrioridadeErrado();
    }
    if(!StringUtil::str_numerica(id_prioridade)) {
        throw ComandoEditarPrioridadeErrado();
    }
    if(stoi(id_prioridade) < 0 || stoi(id_prioridade) > _storage->get_qtd_prioridades()) {
        throw IdPrioridadeNaoExiste();
    }

    string nome_atual = _storage->get_prioridade(stoi(id_prioridade))->get_nome_etiqueta();
    limpa_terminal();
    string acao = "Editando prioridade '" + nome_atual + "' ('c' para cancelar)\n";
    print_centralizado(acao);

    print("Nome atual: " + nome_atual);

    // Pergunta o nome da prioridade
    print("Digite o novo nome da prioridade (máximo 15 caracteres): ", false);
    string novo_nome;
    while(novo_nome.size() == 0) {
        
        if(entradas.size() == 0) {
            novo_nome = get_entrada();
        }
        else {
            novo_nome = entradas[0];
        }

        if(novo_nome == "c") {
            throw EdicaoPrioridadeCancelada();
        }
        try {
            if(StringUtil::tamanho(novo_nome) > 15) {
                throw NomePrioridadeMuitoLongo();
            }
            if(_storage->grupo_existe(novo_nome)) {
                throw PrioridadeJaExiste();
            }
        }
        catch(exception& e) {
            print_erro(e.what());
            print("Digite o novo nome da prioridade (máximo 15 caracteres): ", false);
            novo_nome = "";
        }
    }

    _storage->alterar_prioridade(stoi(id_prioridade), novo_nome);
    limpa_terminal();
    print_tarefas(_pagina_atual);
    print_sucesso("Prioridade editada com sucesso.");
    return true;
}

bool Terminal::editar_nivel_prd(string comando, vector<string> entradas) {
    vector<string> comando_dividido = StringUtil::split_str(comando, " ");

    // Verifica se foi passado o ID da prioridade
    string id_prioridade;
    if(comando_dividido.size() > 1) {
        id_prioridade = StringUtil::split_str(comando, " ")[1];
    }
    else {
        throw ComandoEditarPrioridadeErrado();
    }
    if(!StringUtil::str_numerica(id_prioridade)) {
        throw ComandoEditarPrioridadeErrado();
    }
    if(stoi(id_prioridade) < 0 || stoi(id_prioridade) > _storage->get_qtd_prioridades()) {
        throw IdPrioridadeNaoExiste();
    }

    int nivel_atual = _storage->get_prioridade(stoi(id_prioridade))->get_nivel_prioridade();
    limpa_terminal();
    string nome = _storage->get_prioridade(stoi(id_prioridade))->get_nome_etiqueta();
    string acao = "Editando prioridade '" + nome + "' ('c' para cancelar)\n";
    print_centralizado(acao);

    print("Nível atual: " + to_string(nivel_atual));

    // Pergunta o valor da prioridade
    print("Digite o valor do novo nível da prioridade (número menor -> prioridade maior): ", false);
    string novo_nivel;
    while(novo_nivel.size() == 0) {
        
        if(entradas.size() == 0) {
            novo_nivel = get_entrada();
        }
        else {
            novo_nivel = entradas[0];
        }

        if(novo_nivel == "c") {
            throw EdicaoPrioridadeCancelada();
        }
        try {
            if(!StringUtil::str_numerica(novo_nivel)) {
                throw ValorPrioridadeInvalido();
            }
        }
        catch(exception& e) {
            print_erro(e.what());
            print("Digite o valor do novo nível da prioridade (número menor -> prioridade maior): ", false);
            novo_nivel = "";
        }
    }

    _storage->alterar_nivel(stoi(id_prioridade), stoi(novo_nivel));
    limpa_terminal();
    print_tarefas(_pagina_atual);
    print_sucesso("Prioridade editada com sucesso.");
    return true;
}

bool Terminal::deletar_tarefa(string comando) {
    vector<string> comando_dividido = StringUtil::split_str(comando, " ");

    // Verifica se foi passado o ID da tarefa
    string id_tarefa;
    if(comando_dividido.size() > 1) {
        id_tarefa = StringUtil::split_str(comando, " ")[1];
    }
    else {
        throw ComandoExcluirTarefaErrado();
    }
    if(!StringUtil::str_numerica(id_tarefa)) {
        throw ComandoExcluirTarefaErrado();
    }
    if(stoi(id_tarefa) < 0 || stoi(id_tarefa) > _storage->get_qtd_tarefas()) {
        throw IdTarefaNaoExiste();
    }

    _storage->apaga_tarefa(stoi(id_tarefa));
    limpa_terminal();
    print_tarefas(_pagina_atual);
    print_sucesso("Tarefa excluída com sucesso.");
    return true;
}

bool Terminal::deletar_grupo(string comando) {
    vector<string> comando_dividido = StringUtil::split_str(comando, " ");

    // Verifica se foi passado o ID do grupo
    string id_grupo;
    if(comando_dividido.size() > 1) {
        id_grupo = StringUtil::split_str(comando, " ")[1];
    }
    else {
        throw ComandoExcluirGrupoErrado();
    }
    if(!StringUtil::str_numerica(id_grupo)) {
        throw ComandoExcluirGrupoErrado();
    }
    if(stoi(id_grupo) < 0 || stoi(id_grupo) > _storage->get_qtd_grupos()) {
        throw IdGrupoNaoExiste();
    }

    _storage->apaga_grupo(stoi(id_grupo));
    limpa_terminal();
    print_tarefas(_pagina_atual);
    print_sucesso("Grupo excluído com sucesso.");
    return true;
}

bool Terminal::deletar_prioridade(string comando) {
    vector<string> comando_dividido = StringUtil::split_str(comando, " ");

    // Verifica se foi passado o ID da prioridade
    string id_prioridade;
    if(comando_dividido.size() > 1) {
        id_prioridade = StringUtil::split_str(comando, " ")[1];
    }
    else {
        throw ComandoExcluirPrioridadeErrado();
    }
    if(!StringUtil::str_numerica(id_prioridade)) {
        throw ComandoExcluirPrioridadeErrado();
    }
    if(stoi(id_prioridade) < 0 || stoi(id_prioridade) > _storage->get_qtd_prioridades()) {
        throw IdPrioridadeNaoExiste();
    }

    _storage->apaga_prioridade(stoi(id_prioridade));
    limpa_terminal();
    print_tarefas(_pagina_atual);
    print_sucesso("Prioridade excluída com sucesso.");
    return true;
}

void Terminal::logout() {
    _storage->finaliza_storage();
    delete _storage;
    _storage = new Storage();
    _usuario = Usuario();
    inicia();
}

bool Terminal::pede_comando(vector<string> entradas) {
    string comando;
    while(comando.size() == 0) {
        print("Comando (h para listar comandos): ", false);
        
        if(entradas.size() == 0) {
            comando = get_entrada();
        }
        else {
            comando = entradas[0];
        }

        if(StringUtil::str_numerica(comando)) {
            try {
                int pagina = stoi(comando);
                limpa_terminal();
                print_tarefas(pagina);
                comando = "";
            }
            catch(exception& e) {
                if(_storage->get_qtd_tarefas() > 0) {
                    limpa_terminal();
                    print_tarefas(_pagina_atual);
                }
                print_erro(e.what());
                comando = "";
            }
        }
        else if(comando == "h") {
            print_ajuda();
            comando = "";
        }
        else if(comando.substr(0, 2) == "pg") {
            try {
                if(comando.size() < 4) {
                    throw PaginaNaoInformada();
                }
                string pagina = comando.substr(3);
                if(StringUtil::str_numerica(pagina)) {
                    limpa_terminal();
                    print_grupos(stoi(pagina));
                }
                else {
                    throw PaginaNaoInformada();
                }
            }
            catch(exception& e) {
                limpa_terminal();
                print_tarefas(_pagina_atual);
                print_erro(e.what());
            }
            comando = "";
        }
        else if(comando.substr(0, 2) == "pp") {
            try {
                if(comando.size() < 4) {
                    throw PaginaNaoInformada();
                }
                string pagina = comando.substr(3);
                if(StringUtil::str_numerica(pagina)) {
                    limpa_terminal();
                    print_prioridades(stoi(pagina));
                }
                else {
                    throw PaginaNaoInformada();
                }
            }
            catch(exception& e) {
                limpa_terminal();
                print_tarefas(_pagina_atual);
                print_erro(e.what());
            }
            comando = "";
        }
        else if(comando.substr(0, 1) == "t") {
            try {
                exibe_tarefa(comando);
            }
            catch(exception& e) {
                limpa_terminal();
                print_tarefas(_pagina_atual);
                print_erro(e.what());
            }
            comando = "";
        }
        else if(comando == "nt") {
            try {
                nova_tarefa();
            }
            catch(exception& e) {
                limpa_terminal();
                print_tarefas(_pagina_atual);
                print_erro(e.what());
            }
            comando = "";
        }
        else if(comando == "ng") {
            try {
                novo_grupo();
            }
            catch(exception& e) {
                limpa_terminal();
                print_tarefas(_pagina_atual);
                print_erro(e.what());
            }
            comando = "";
        }
        else if(comando == "np") {
            try {
                nova_prioridade();
            }
            catch(exception& e) {
                limpa_terminal();
                print_tarefas(_pagina_atual);
                print_erro(e.what());
            }
            comando = "";
        }
        else if(comando.substr(0, 3) == "ett") {
            try {
                editar_titulo_tarefa(comando);
            }
            catch(exception& e) {
                limpa_terminal();
                print_tarefas(_pagina_atual);
                print_erro(e.what());
            }
            comando = "";
        }
        else if(comando.substr(0, 3) == "edt") {
            try {
                editar_desc_tarefa(comando);
            }
            catch(exception& e) {
                limpa_terminal();
                print_tarefas(_pagina_atual);
                print_erro(e.what());
            }
            comando = "";
        }
        else if(comando.substr(0, 4) == "epzt") {
            try {
                editar_prazo_tarefa(comando);
            }
            catch(exception& e) {
                limpa_terminal();
                print_tarefas(_pagina_atual);
                print_erro(e.what());
            }
            comando = "";
        }
        else if(comando.substr(0, 3) == "egt") {
            try {
                editar_grupo_tarefa(comando);
            }
            catch(exception& e) {
                limpa_terminal();
                print_tarefas(_pagina_atual);
                print_erro(e.what());
            }
            comando = "";
        }
        else if(comando.substr(0, 3) == "ept") {
            try {
                editar_prd_tarefa(comando);
            }
            catch(exception& e) {
                limpa_terminal();
                print_tarefas(_pagina_atual);
                print_erro(e.what());
            }
            comando = "";
        }
        else if(comando.substr(0, 5) == "check") {
            try {
                marcar_concluida(comando);
            }
            catch(exception& e) {
                limpa_terminal();
                print_tarefas(_pagina_atual);
                print_erro(e.what());
            }
            comando = "";
        }
        else if(comando.substr(0, 7) == "uncheck") {
            try {
                desmarcar_concluida(comando);
            }
            catch(exception& e) {
                limpa_terminal();
                print_tarefas(_pagina_atual);
                print_erro(e.what());
            }
            comando = "";
        }
        else if(comando.substr(0, 3) == "eng") {
            try {
                editar_nome_grupo(comando);
            }
            catch(exception& e) {
                limpa_terminal();
                print_tarefas(_pagina_atual);
                print_erro(e.what());
            }
            comando = "";
        }
        else if(comando.substr(0, 3) == "edg") {
            try {
                editar_desc_grupo(comando);
            }
            catch(exception& e) {
                limpa_terminal();
                print_tarefas(_pagina_atual);
                print_erro(e.what());
            }
            comando = "";
        }
        else if(comando.substr(0, 3) == "enp") {
            try {
                editar_nome_prd(comando);
            }
            catch(exception& e) {
                limpa_terminal();
                print_tarefas(_pagina_atual);
                print_erro(e.what());
            }
            comando = "";
        }
        else if(comando.substr(0, 3) == "evp") {
            try {
                editar_nivel_prd(comando);
            }
            catch(exception& e) {
                limpa_terminal();
                print_tarefas(_pagina_atual);
                print_erro(e.what());
            }
            comando = "";
        }
        else if(comando.substr(0, 2) == "dt") {
            try {
                deletar_tarefa(comando);
            }
            catch(exception& e) {
                limpa_terminal();
                print_tarefas(_pagina_atual);
                print_erro(e.what());
            }
            comando = "";
        }
        else if(comando.substr(0, 2) == "dg") {
            try {
                deletar_grupo(comando);
            }
            catch(exception& e) {
                limpa_terminal();
                print_tarefas(_pagina_atual);
                print_erro(e.what());
            }
            comando = "";
        }
        else if(comando.substr(0, 2) == "dp") {
            try {
                deletar_prioridade(comando);
            }
            catch(exception& e) {
                limpa_terminal();
                print_tarefas(_pagina_atual);
                print_erro(e.what());
            }
            comando = "";
        }
        else if(comando == "out") {
            break;
        }
        else if(comando == "q") {
            // Finaliza o programa
            break;
        }
        else {
            comando = "";
            limpa_terminal();
            print_tarefas(_pagina_atual);
            print_erro("Comando inválido. Tente novamente.");
        }
    }
    
    // Reinicia o loop caso tenha sido feito logout
    if(comando == "out") {
        logout();
    }
    // Desaloca o storage caso tenha sido feito o quit
    else {
        _storage->finaliza_storage();
        delete _storage;
    }

    return true;
}

bool Terminal::inicia() {
    limpa_terminal();
    login_ou_registre();
    _pagina_atual = 1;
    print_tarefas(_pagina_atual);
    pede_comando();
    return true;
}