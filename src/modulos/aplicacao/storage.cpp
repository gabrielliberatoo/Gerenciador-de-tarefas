#include "storage.hpp"
#include "excecoes/exc_arquivo.hpp"

Storage::Storage() {
    // Cria a pasta storage caso ela não exista
    if(!FileUtil::folder_exists("")) {
        system("mkdir storage");
    }
    _logado = false;
}

string Storage::get_nome_arquivo(string usuario) {
    string nome_arquivo = "storage/" + usuario + ".txt";
    return nome_arquivo;
}

bool Storage::usuario_existe(string usuario) {
    string nome_arquivo = get_nome_arquivo(usuario);
    if(FileUtil::file_exists(nome_arquivo)) {
        return true;
    }
    else {
        return false;
    }
}

bool Storage::cria_usuario(string usuario, string senha) {
    // Verifica se já existe um usuário com esse nome
    if(usuario_existe(usuario)) {
        throw UsuarioJaExiste();
    }
    else {
        // Cria arquivo e armazena a senha na primeira linha
        _file.open(get_nome_arquivo(usuario), ios::out);
        if(_file.is_open()) {
            _file << senha << "\n";
            _file.close();
        }
        else {
            throw ErroAoAbrirArquivo();
        }
        _file.close();
    }
    return true;
}

bool Storage::exclui_usuario(string usuario) {
    if(usuario_existe(usuario)) {
        string comando = "rm " + get_nome_arquivo(usuario);
        system(comando.c_str());
        return true;
    }
    else {
        return false;
    }
}

bool Storage::confere_senha(string usuario, string senha) {
    if(!usuario_existe(usuario)) {
        throw UsuarioNaoEncontrado();
    }
    fstream file;
    _file.open(get_nome_arquivo(usuario), ios::in);
    if(_file.is_open()) {
        string senha_arquivo;
        // Pega a senha do usuário na primeira linha do arquivo
        getline(_file, senha_arquivo);
        if(senha == senha_arquivo) {
            _logado = true;
            _usuario_logado = usuario;
            _file.close();
            le_conteudo();
            return true;
        }
        else {
            _file.close();
            return false;
        }
    }
    else {
        throw ErroAoAbrirArquivo();
    }
    return false;
}

bool Storage::ordena_tarefas() {
    // Ordena o vetor por ordem de prioridade
    sort(_tarefas.begin(), _tarefas.end(), Tarefa::compara_prioridade);
    return true;
}

bool Storage::parametro_invalido(string parametro) {
    if(parametro == "[tarefa]") return true;
    if(parametro == "[grupo]") return true;
    if(parametro == "[prioridade]") return true;
    if(parametro == "") return true;
    if(parametro == " ") return true;
    return false;
}

bool Storage::le_grupo() {
    string parametro;
    // Lê os 2 parâmetros do grupo
    string nome_grupo, descricao;
    getline(_file, nome_grupo);
    getline(_file, descricao);

    // Trata erro de arquivo no formato inválido
    if(parametro_invalido(nome_grupo)) throw ErroAoAbrirArquivo();
    if(parametro_invalido(descricao)) throw ErroAoAbrirArquivo();

    _grupos.push_back(new Grupo(nome_grupo, "grupo", descricao));
    return true;
}

bool Storage::le_prioridade() {
    string parametro;
    // Lê os 2 parâmetros da prioridade
    string nome_prioridade;
    getline(_file, nome_prioridade);
    int valor_prioridade;
    _file >> valor_prioridade;

    // Trata erro de arquivo no formato inválido
    if(parametro_invalido(nome_prioridade)) throw ErroAoAbrirArquivo();

    _prioridades.push_back(new Prioridade(nome_prioridade, "prioridade", valor_prioridade));
    return true;
}

bool Storage::le_tarefa() {
    string parametro;
    // Lê os 6 parâmetros da tarefa
    string nome, descricao, prioridade, grupo, prazo;
    bool concluido;
    getline(_file, nome);
    getline(_file, descricao);
    getline(_file, prioridade);
    getline(_file, grupo);
    getline(_file, prazo);
    _file >> concluido;

    // Trata erro de arquivo no formato inválido
    if(parametro_invalido(nome)) throw ErroAoAbrirArquivo();
    if(parametro_invalido(descricao)) throw ErroAoAbrirArquivo();
    if(parametro_invalido(prioridade)) throw ErroAoAbrirArquivo();
    if(parametro_invalido(grupo)) throw ErroAoAbrirArquivo();
    if(parametro_invalido(prazo)) throw ErroAoAbrirArquivo();

    if(get_prioridade(prioridade) == nullptr || get_grupo(grupo) == nullptr) {
        throw ErroAoAbrirArquivo();
    }
    _tarefas.push_back(new Tarefa(nome, descricao, prazo, concluido, get_grupo(grupo), get_prioridade(prioridade)));
    return true;
}

bool Storage::le_conteudo() {
    if(_logado) {
        // Lê os grupos
        _file.open(get_nome_arquivo(_usuario_logado), ios::in);
        if(_file.is_open()) {
            string linha;
            // Pula a linha da senha
            getline(_file, linha);
            while (getline(_file, linha)) {
                if(linha == "[prioridade]") {
                    le_prioridade();
                }
            }
        }
        else { 
            throw ErroAoAbrirArquivo();
        }
        _file.close();

        // Lê as prioridades
        _file.open(get_nome_arquivo(_usuario_logado), ios::in);
        if(_file.is_open()) {
            string linha;
            // Pula a linha da senha
            getline(_file, linha);
            while (getline(_file, linha)) {
                if(linha == "[grupo]") {
                    le_grupo();
                }
            }
        }
        else { 
            throw ErroAoAbrirArquivo();
        }
        _file.close();

        // Lê as tarefas
        _file.open(get_nome_arquivo(_usuario_logado), ios::in);
        if(_file.is_open()) {
            string linha;
            // Pula a linha da senha
            getline(_file, linha);
            while (getline(_file, linha)) {
                if(linha == "[tarefa]") {
                    le_tarefa();
                }
            }
        }
        else { 
            throw ErroAoAbrirArquivo();
        }
        _file.close();

        ordena_tarefas();
    }
    return true;
}

int Storage::get_qtd_tarefas() {
    return _tarefas.size();
}

int Storage::get_qtd_grupos() {
    return _grupos.size();
}

int Storage::get_qtd_prioridades() {
    return _prioridades.size();
}

Prioridade* Storage::get_prioridade(string nome_prioridade) {
    // Procura uma prioridade com o nome informado
    for(auto& prioridade : _prioridades) {
        if(prioridade->get_nome_etiqueta() == nome_prioridade) {
            return prioridade;
        }
    }
    // Retorna um ponteiro nulo caso não tenha uma prioridade com esse nome
    return nullptr;
}

Prioridade* Storage::get_prioridade(int indice_prioridade) {
    if(indice_prioridade > (int) _prioridades.size()) {
        return nullptr;
    }
    else {
        return _prioridades[indice_prioridade - 1];
    }
}

Grupo* Storage::get_grupo(string nome_grupo) {
    // Procura um grupo com o nome informado
    for(auto& grupo : _grupos) {
        if(grupo->get_nome_etiqueta() == nome_grupo) {
            return grupo;
        }
    }
    // Retorna um ponteiro nulo caso não tenha um grupo com esse nome
    return nullptr;
}

Grupo* Storage::get_grupo(int indice_grupo) {
    if(indice_grupo > (int) _grupos.size()) {
        return nullptr;
    }
    else {
        return _grupos[indice_grupo - 1];
    }
}

Tarefa* Storage::get_tarefa(string titulo_tarefa) {
    // Procura uma tarefa com o nome informado
    for(auto& tarefa : _tarefas) {
        if(tarefa->get_titulo() == titulo_tarefa) {
            return tarefa;
        }
    }
    // Retorna um ponteiro nulo caso não tenha um grupo com esse nome
    return nullptr;
}

Tarefa* Storage::get_tarefa(int indice_tarefa) {
    if(indice_tarefa > (int) _tarefas.size()) {
        return nullptr;
    }
    else {
        return _tarefas[indice_tarefa - 1];
    }
}

bool Storage::registra_tarefa(Tarefa *tarefa) {
    ofstream arquivo;

    arquivo.open(get_nome_arquivo(_usuario_logado), ios::app);
    arquivo << endl << "[tarefa]";
    arquivo << endl << tarefa->get_titulo();
    arquivo << endl << tarefa->get_descricao();
    arquivo << endl << tarefa->get_nome_prioridade();
    arquivo << endl << tarefa->get_nome_grupo();
    arquivo << endl << tarefa->get_prazo();
    arquivo << endl << tarefa->get_concluido();
    return true;
}


bool Storage::cria_tarefa(string titulo, string descricao, string prazo, string grupo_tarefa, string prioridade_tarefa) {
    if(tarefa_existe(titulo)) {
        throw TarefaJaExiste();
    }
    if(!grupo_existe(grupo_tarefa)) {
        throw GrupoNaoExiste();
    }
    if(!prioridade_existe(prioridade_tarefa)) {
        throw PrioridadeNaoExiste();
    }
    bool concluido = false;
    _tarefas.push_back(new Tarefa(titulo, descricao, prazo, concluido, get_grupo(grupo_tarefa), get_prioridade(prioridade_tarefa)));
    registra_tarefa(_tarefas.back());
    ordena_tarefas();
    return true;
}

bool Storage::registra_grupo(Grupo *grupo) {
    ofstream arquivo;

    arquivo.open(get_nome_arquivo(_usuario_logado), ios::app);
    arquivo << endl << "[grupo]";
    arquivo << endl << grupo->get_nome_etiqueta();
    arquivo << endl << grupo->get_descricao();

    return true;
}

bool Storage::cria_grupo(string nome_etiqueta, string descricao) {
    if(grupo_existe(nome_etiqueta)) {
        throw GrupoJaExiste();
    }
    string tipo = "grupo";
    _grupos.push_back(new Grupo(nome_etiqueta, tipo, descricao));
    registra_grupo(_grupos.back());
    return true;
}

bool Storage::registra_prioridade(Prioridade *prioridade) {
    ofstream arquivo;

    arquivo.open(get_nome_arquivo(_usuario_logado), ios::app);
    arquivo << endl << "[prioridade]";
    arquivo << endl << prioridade->get_nome();
    arquivo << endl << prioridade->get_nivel_prioridade();

    return true;
}

bool Storage::cria_prioridade(string nome_etiqueta, int nivel_prioridade) {
    if(prioridade_existe(nome_etiqueta)) {
        throw PrioridadeJaExiste();
    }
    string tipo ="prioridade";
    _prioridades.push_back(new Prioridade(nome_etiqueta, tipo, nivel_prioridade));
    registra_prioridade(_prioridades.back());
    return true;
}

bool Storage::tarefa_existe(string titulo_tarefa) {
    if(get_tarefa(titulo_tarefa) != nullptr) {
        return true;
    }
    else {
        return false;
    }
}

bool Storage::grupo_existe(string nome_grupo) {
    if(get_grupo(nome_grupo) != nullptr) {
        return true;
    }
    else {
        return false;
    }
}

bool Storage::prioridade_existe(string nome_prioridade) {
    if(get_prioridade(nome_prioridade) != nullptr) {
        return true;
    }
    else {
        return false;
    }
}

bool Storage::editar_titulo(int indice_tarefa, string novo_titulo) {
    if(get_tarefa(indice_tarefa) == nullptr) {
        throw IdTarefaNaoExiste();
    }
    if(tarefa_existe(novo_titulo)) {
        throw TarefaJaExiste();
    }

    Tarefa *t1 = get_tarefa(indice_tarefa);
    string titulo_antigo = t1->get_titulo();
    t1->editar_titulo(novo_titulo);
    
    ifstream arquivo;
    ofstream novo_arquivo;
    
    arquivo.open(get_nome_arquivo(_usuario_logado));
    
    vector<string> lines;
    string line;
    
    while(getline(arquivo, line)) {
        lines.push_back(line);
    }
    
    novo_arquivo.open("novo", ios::app);

    for(int i=0; i < (int) lines.size() ;i++) {         
        if(lines[i] == "[tarefa]") {
            if(lines[i+1] == titulo_antigo) {
                lines[i+1] = t1->get_titulo();
            }
        }
        novo_arquivo << lines[i] << endl;
    }
    novo_arquivo.close();
    arquivo.close();    
    //Substitui arquivo
    string comando = "mv novo " + get_nome_arquivo(_usuario_logado);
    system(comando.c_str());   
    return true;
}
   
bool Storage::editar_descricao(int indice_tarefa, string nova_descricao) {
    if(get_tarefa(indice_tarefa) == nullptr) {
        throw IdTarefaNaoExiste();
    }
    Tarefa *t1 = get_tarefa(indice_tarefa);
    string descricao_antiga = t1->get_descricao();
    t1->editar_descricao(nova_descricao);
    
    ifstream arquivo;
    ofstream novo_arquivo;
    
    arquivo.open(get_nome_arquivo(_usuario_logado));
    
    vector<string> lines;
    string line;
    
    while(getline(arquivo, line)) {
        lines.push_back(line);
    }
    
    novo_arquivo.open("novo", ios::app);

    for(int i = 0; i < (int) lines.size(); i++) { 
        if(lines[i] == "[tarefa]") {
            if(lines[i+2]==descricao_antiga) {
                lines[i+2]=t1->get_descricao();
            }
        }
        novo_arquivo << lines[i] << endl;
    }
    novo_arquivo.close();
    arquivo.close();    
    //Substitui arquivo
    string comando = "mv novo " + get_nome_arquivo(_usuario_logado);
    system(comando.c_str());   
    return true;
}

bool Storage::editar_prazo(int indice_tarefa, string novo_prazo) {
    if(get_tarefa(indice_tarefa) == nullptr) {
        throw IdTarefaNaoExiste();
    }
    if(novo_prazo.length() != 10) {
        throw PrazoInvalido();
    }
    Tarefa *t1 = get_tarefa(indice_tarefa);
    string prazo_antigo = t1->get_prazo();
    t1->editar_prazo(novo_prazo);
    
    ifstream arquivo;
    ofstream novo_arquivo;
    
    arquivo.open(get_nome_arquivo(_usuario_logado));
    
    vector<string> lines;
    string line;
    
    while(getline(arquivo, line)) {
        lines.push_back(line);
    }
    
    novo_arquivo.open("novo", ios::app);

    for(int i = 0; i < (int) lines.size(); i++) {      
        if(lines[i] == "[tarefa]") {
            if(lines[i+5] == prazo_antigo) {
                lines[i+5] = t1->get_prazo();
            }
        }
        novo_arquivo << lines[i] << endl;
    }
    novo_arquivo.close();
    arquivo.close();    
    //Substitui arquivo
    string comando = "mv novo " + get_nome_arquivo(_usuario_logado);
    system(comando.c_str());  
    return true; 
}

bool Storage::editar_grupo(int indice_tarefa, string novo_grupo) {
    if(get_tarefa(indice_tarefa) == nullptr) {
        throw IdTarefaNaoExiste();
    }
    if(!grupo_existe(novo_grupo)) {
        throw GrupoNaoExiste();
    }
    Tarefa *t1 = get_tarefa(indice_tarefa);
    string titulo_tarefa = t1->get_titulo();
    Grupo* grupo = get_grupo(novo_grupo);
    grupo->incrementa_tarefa();
    Grupo *grupo_antigo = t1->get_grupo();
    grupo_antigo->decrementa_tarefa();
    t1->editar_grupo(grupo);
    
    ifstream arquivo;
    ofstream novo_arquivo;
    
    arquivo.open(get_nome_arquivo(_usuario_logado));
    
    vector<string> lines;
    string line;
    
    while(getline(arquivo, line)) {
        lines.push_back(line);
    }
    
    novo_arquivo.open("novo", ios::app);

    for(int i = 0; i < (int) lines.size(); i++) {        
        if(lines[i] == "[tarefa]") {
            if(lines[i+1] == titulo_tarefa) {
                lines[i+4] = novo_grupo;
            }
        }
        novo_arquivo << lines[i] << endl;
    }
    novo_arquivo.close();
    arquivo.close();    
    //Substitui arquivo
    string comando = "mv novo " + get_nome_arquivo(_usuario_logado);
    system(comando.c_str());   
    return true;
}

bool Storage::editar_prioridade(int indice_tarefa, string nova_prioridade) {
    if(get_tarefa(indice_tarefa) == nullptr) {
        throw IdTarefaNaoExiste();
    }
    if(!prioridade_existe(nova_prioridade)) {
        throw PrioridadeNaoExiste();
    }
    Tarefa *t1 = get_tarefa(indice_tarefa);
    string titulo_tarefa = t1->get_titulo();
    Prioridade* prioridade = get_prioridade(nova_prioridade);
    prioridade->incrementa_tarefa();
    Prioridade *prioridade_antiga = t1->get_prioridade();
    prioridade_antiga->decrementa_tarefa();
    t1->editar_prioridade(prioridade);

    ifstream arquivo;
    ofstream novo_arquivo;
    
    arquivo.open(get_nome_arquivo(_usuario_logado));
    
    vector<string> lines;
    string line;
    
    while(getline(arquivo, line)) {
        lines.push_back(line);
    }
    
    novo_arquivo.open("novo", ios::app);

    for(int i = 0; i < (int) lines.size(); i++) {     
        if(lines[i] == "[tarefa]") {
            if(lines[i+1] == titulo_tarefa) {
                lines[i+3] = nova_prioridade;
            }
        }
        novo_arquivo << lines[i] << endl;
    }
    novo_arquivo.close();
    arquivo.close();    
    //Substitui arquivo
    string comando = "mv novo " + get_nome_arquivo(_usuario_logado);
    system(comando.c_str());    

    ordena_tarefas(); 
    return true;
}

bool Storage::alterar_grupo(int indice_grupo, string novo_grupo) {
    if(get_grupo(indice_grupo) == nullptr) {
        throw IdGrupoNaoExiste();
    }
    if(grupo_existe(novo_grupo)) {
        throw GrupoJaExiste();
    }
    Grupo *g1 = get_grupo(indice_grupo);
    string antigo_grupo = g1->get_nome_etiqueta();
    g1->alterar(novo_grupo);
    
    ifstream arquivo;
    ofstream novo_arquivo;
    
    arquivo.open(get_nome_arquivo(_usuario_logado));
    
    vector<string> lines;
    string line;
    
    while(getline(arquivo, line)) {
        lines.push_back(line);
    }
    
    novo_arquivo.open("novo", ios::app);

    for(int i = 0; i < (int) lines.size(); i++) { 
        if(lines[i] == "[grupo]") {
            if(lines[i+1] == antigo_grupo) {
                lines[i+1] = novo_grupo;
            }
        }
        if(lines[i] == "[tarefa]") {
            if(lines[i+4] == antigo_grupo) {
                lines[i+4] = novo_grupo;
            }
        }
        novo_arquivo << lines[i] << endl;
    }
    novo_arquivo.close();
    arquivo.close();    
    //Substitui arquivo
    string comando = "mv novo " + get_nome_arquivo(_usuario_logado);
    system(comando.c_str());
    return true;
}

bool Storage::alterar_des_grupo(int indice_grupo, string nova_descricao) {
    if(get_grupo(indice_grupo) == nullptr) {
        throw IdGrupoNaoExiste();
    }
    Grupo *g1 = get_grupo(indice_grupo);
    string descricao_antiga = g1->get_descricao();
    g1->alterar_descricao(nova_descricao);
    
    ifstream arquivo;
    ofstream novo_arquivo;
    
    arquivo.open(get_nome_arquivo(_usuario_logado));
    
    vector<string> lines;
    string line;
    
    while(getline(arquivo, line)) {
        lines.push_back(line);
    }
    
    novo_arquivo.open("novo", ios::app);

    for(int i = 0; i < (int) lines.size(); i++) {  
        if(lines[i] == "[grupo]") {
            if(lines[i+2] == descricao_antiga) {
                lines[i+2] = g1->get_descricao();
            }
        }
        novo_arquivo << lines[i] << endl;
    }
    novo_arquivo.close();
    arquivo.close();    
    //Substitui arquivo
    string comando = "mv novo " + get_nome_arquivo(_usuario_logado);
    system(comando.c_str());    
    return true;
}

bool Storage::alterar_prioridade(int indice_prioridade, string nova_prioridade) {
    if(get_prioridade(indice_prioridade) == nullptr) {
        throw IdPrioridadeNaoExiste();
    }
    if(prioridade_existe(nova_prioridade)) {
        throw PrioridadeJaExiste();
    }

    Prioridade *p1=get_prioridade(indice_prioridade);
    string antiga_prioridade = p1->get_nome_etiqueta();
    p1->editar_prioridade(nova_prioridade);

    ifstream arquivo;
    ofstream novo_arquivo;
    
    arquivo.open(get_nome_arquivo(_usuario_logado));
    
    vector<string> lines;
    string line;
    
    while(getline(arquivo, line)) {
        lines.push_back(line);
    }
    
    novo_arquivo.open("novo", ios::app);

    for(int i = 0; i < (int) lines.size(); i++) {           
        if(lines[i] == "[prioridade]") {
            if(lines[i+1] == antiga_prioridade) {
                lines[i+1] = p1->get_nome_etiqueta();
            }
        }
        if(lines[i] == "[tarefa]") {
            if(lines[i+3] == antiga_prioridade) {
                lines[i+3] = nova_prioridade;
            }
        }
        novo_arquivo << lines[i] << endl;
    }
    novo_arquivo.close();
    arquivo.close();    
    //Substitui arquivo
    string comando = "mv novo " + get_nome_arquivo(_usuario_logado);
    system(comando.c_str());   
    return true; 
}

bool Storage::alterar_nivel(int indice_prioridade, int novo_nivel) {
    if(get_prioridade(indice_prioridade) == nullptr) {
        throw IdPrioridadeNaoExiste();
    }

    Prioridade *p1=get_prioridade(indice_prioridade);
    string nome = p1->get_nome_etiqueta();
    p1->alterar_valor(novo_nivel);

    ifstream arquivo;
    ofstream novo_arquivo;
    
    arquivo.open(get_nome_arquivo(_usuario_logado));
    
    vector<string> lines;
    string line;
    
    while(getline(arquivo, line)) {
        lines.push_back(line);
    }
    
    novo_arquivo.open("novo", ios::app);

    for(int i = 0; i < (int) lines.size(); i++) {           
        if(lines[i] == "[prioridade]") {
            if(lines[i+1] == nome) {
                lines[i+2] = to_string(p1->get_nivel_prioridade());
            }
        }
        novo_arquivo << lines[i] << endl;
    }
    novo_arquivo.close();
    arquivo.close();    
    //Substitui arquivo
    string comando = "mv novo " + get_nome_arquivo(_usuario_logado);
    system(comando.c_str());    

    ordena_tarefas();
    return true;
}

bool Storage::alterar_concluido(int indice_tarefa, bool novo_status) {
    if(get_tarefa(indice_tarefa) == nullptr) {
        throw IdTarefaNaoExiste();
    }

    Tarefa *t1 = get_tarefa(indice_tarefa);
    string titulo_tarefa = t1->get_titulo();
    t1->editar_status(novo_status);
    
    ifstream arquivo;
    ofstream novo_arquivo;
    
    arquivo.open(get_nome_arquivo(_usuario_logado));
    
    vector<string> lines;
    string line;
    
    while(getline(arquivo, line)) {
        lines.push_back(line);
    }
    
    novo_arquivo.open("novo", ios::app);

    for(int i = 0; i < (int) lines.size(); i++) {   
        if(lines[i] == "[tarefa]") {
            if(lines[i+1] == titulo_tarefa) {
                lines[i+6] = to_string(t1->get_concluido());
            }
        }
        novo_arquivo << lines[i] << endl;
    }
    novo_arquivo.close();
    arquivo.close();    
    //Substitui arquivo
    string comando = "mv novo " + get_nome_arquivo(_usuario_logado);
    system(comando.c_str());
    return true;
}

bool Storage::apaga_tarefa(int indice_tarefa) {
    if(get_tarefa(indice_tarefa) == nullptr) {
        throw IdTarefaNaoExiste();
    }

    Tarefa *tarefa = get_tarefa(indice_tarefa);
    tarefa->get_prioridade()->decrementa_tarefa();
    tarefa->get_grupo()->decrementa_tarefa();
    string titulo = tarefa->get_titulo();
    for(int i = 0; i < (int) _tarefas.size(); i++) {
        if(_tarefas[i] == tarefa) {
            _tarefas.erase(_tarefas.begin()+i);
        }
    }
    
    ifstream arquivo;
    ofstream novo_arquivo;
    
    arquivo.open(get_nome_arquivo(_usuario_logado));
    
    vector<string> lines;
    string line;
    
    while(getline(arquivo, line)) {
        lines.push_back(line);
    }
    int i=0;
    vector<string> new_lines;
    while(i < (int) lines.size()) {
        if(lines[i] == "[tarefa]" && lines[i+1] == titulo) {
            for(int j = 0; j < 7; j++) {
                lines.erase(lines.begin()+i);
            }
        }
        else{
            new_lines.push_back(lines[i]);
        }
        i++;
    }
     
    novo_arquivo.open("novo", ios::app);

    for(int i = 0; i < (int) lines.size(); i++) {
        novo_arquivo << lines[i] << endl;
    }
    novo_arquivo.close();
    arquivo.close();    
    
    //Substitui arquivo
    string comando = "mv novo " + get_nome_arquivo(_usuario_logado);
    system(comando.c_str());
    return true;
}

bool Storage::apaga_grupo(int indice_grupo) {
    if(get_grupo(indice_grupo) == nullptr) {
        throw IdGrupoNaoExiste();
    }

    Grupo *grupo = get_grupo(indice_grupo);
    
    if(grupo->get_qtd_membros()!= 0) {
        throw ErroAoApagarGrupo();
    }
    
    string titulo = grupo->get_nome_etiqueta();
    for(int i = 0; i < (int) _grupos.size(); i++) {
        if(_grupos[i] == grupo) {
            _grupos.erase(_grupos.begin()+i);
        }
    }
    
    ifstream arquivo;
    ofstream novo_arquivo;
    
    arquivo.open(get_nome_arquivo(_usuario_logado));
    
    vector<string> lines;
    string line;
    
    while(getline(arquivo, line)) {
        lines.push_back(line);
    }
    int i=0;
    vector<string> new_lines;
    while(i < (int) lines.size()) {
        if(lines[i] == "[grupo]" && lines[i+1] == titulo) {
            for(int j = 0; j < 3;j++) {
                lines.erase(lines.begin()+i);
            }
        }
        else{
            new_lines.push_back(lines[i]);
        }
        i++;
    }
     
    novo_arquivo.open("novo", ios::app);

    for(int i = 0; i < (int) lines.size(); i++) {
        novo_arquivo << lines[i] << endl;
    }
    novo_arquivo.close();
    arquivo.close();    
    
    //Substitui arquivo
    string comando = "mv novo " + get_nome_arquivo(_usuario_logado);
    system(comando.c_str());
    return true;
}

bool Storage::apaga_prioridade(int indice_prioridade) {
    if(get_prioridade(indice_prioridade) == nullptr) {
        throw IdPrioridadeNaoExiste();
    }

    Prioridade *prioridade = get_prioridade(indice_prioridade);

    if(prioridade->get_qtd_membros() != 0) {
        throw ErroAoApagarPrioridade();
    }

    string titulo = prioridade->get_nome_etiqueta();
    for(int i = 0; i < (int) _prioridades.size(); i++) {
        if(_prioridades[i] == prioridade) {
            _prioridades.erase(_prioridades.begin()+i);
        }
    }
    
    ifstream arquivo;
    ofstream novo_arquivo;
    
    arquivo.open(get_nome_arquivo(_usuario_logado));
    
    vector<string> lines;
    string line;
    
    while(getline(arquivo, line)) {
        lines.push_back(line);
    }
    int i=0;
    vector<string> new_lines;
    while(i < (int) lines.size()) {
        if(lines[i] == "[prioridade]" && lines[i+1] == titulo) {
            for(int j = 0; j < 3; j++) {
                lines.erase(lines.begin()+i);
            }
        }
        else{
            new_lines.push_back(lines[i]);
        }
        i++;
    }
     
    novo_arquivo.open("novo", ios::app);

    for(int i = 0; i < (int) lines.size(); i++) {
        novo_arquivo << lines[i] << endl;
    }
    novo_arquivo.close();
    arquivo.close();    
    
    //Substitui arquivo
    string comando = "mv novo " + get_nome_arquivo(_usuario_logado);
    system(comando.c_str());

    return true;
}

bool Storage::finaliza_storage() {
    for(auto& tarefa : _tarefas) {
        delete tarefa;
    }
    
    for(auto& prioridade : _prioridades) {
        delete prioridade;
    }

    for(auto& grupo : _grupos) {
        delete grupo;
    }

    return true;
}