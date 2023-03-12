#include "string_util.hpp"

vector<string> StringUtil::split_str(string str, string delimitador) {
    int start = 0;
    int end = str.find(delimitador);
    vector<string> data;
    while (end != -1) {
        data.push_back( str.substr(start, end - start) );
        start = end + delimitador.size();
        end = str.find(delimitador, start);   
    }
    data.push_back(str.substr(start, end - start));
    return data;
}

string StringUtil::centraliza_str(string str, int espaco) {
    if(StringUtil::tamanho(str) < espaco) {
        int espaco_esquerda = (espaco - tamanho(str)) / 2;
        int espaco_direita = espaco - espaco_esquerda - tamanho(str);
        string centralizado = string(espaco_esquerda, ' ') + str + string(espaco_direita, ' ');
        return centralizado;
    }
    else {
        return str;
    }
}

bool StringUtil::str_numerica(string str) {
    // Retorna false se a string for vazia
    if(str.empty()) return false;
     // Retorna false se houver algum caractere não numérico
    for(char const &c : str) {
        if(std::isdigit(c) == 0)
            return false;
    }
    return true;
}

int StringUtil::tamanho(string str) {
    int char_normais = 0;
    int char_especiais = 0;
    for(char const &c : str) {
        // Byte positivo -> caractere normal
        if(c > 0) char_normais++;
        // Byte negativo -> caractere especial
        else char_especiais++;
    }
    int qtd_chars = char_normais + (char_especiais / 2);
    return qtd_chars;
}