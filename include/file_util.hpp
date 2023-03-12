#ifndef FILE_UTIL_HPP
#define FILE_UTIL_HPP

#include <sys/stat.h>
#include <string>
#include <fstream>

using namespace std;

/**
 * @brief Classe de funções uteis para tratar arquivos.
 */
class FileUtil {
    public:
        /// Função que retorna true se a pasta existe e false se não existe
        static bool folder_exists(string folder_name);
        /// função que retorna true se o arquivo existe e false se não existe
        static bool file_exists(string file_name);
};

#endif // FILE_UTIL_HPP