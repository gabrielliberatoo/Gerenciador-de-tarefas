#include "file_util.hpp"

bool FileUtil::folder_exists(string folder_name) {
    struct stat sb;
    if(stat("storage", &sb)) return false;
    else return true;
}

bool FileUtil::file_exists(string file_name) {
    ifstream file(file_name);
    if(!file.is_open()) return false;
    else return true;
}