#ifndef STRING_UTIL_HPP
#define STRING_UTIL_HPP

#include <string>
#include <vector>
#include <iostream>

using namespace std;

/**
 * @brief Classe com funções uteis para tratar strings
 */
class StringUtil {
    public:
    ///Função que divide strings de acordo com delimitador
    static vector<string> split_str(string str, string deli);
    ///Função que centraliza string em um determinado espaço;
    static string centraliza_str(string str, int espaco);
    ///Função que verifica se a string é numerica
    static bool str_numerica(string str);
    /**Função que verifica o tamanho da string 
    considerando caracteres especiais como um caracter só
    */
    static int tamanho(string str);
};

#endif // STRING_UTIL_HPP