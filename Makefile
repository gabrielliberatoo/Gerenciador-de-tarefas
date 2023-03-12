COMPILE = g++
CFLAGS = -c -Wall -Werror
INCLUDE = -I include
LFLAGS = -fPIC -fprofile-arcs -ftest-coverage
TAREFAS = src/modulos/tarefas
APLICACAO = src/modulos/aplicacao
USUARIOS = src/modulos/usuarios
UTIL = src/modulos/util
RM = rm -rf

GerenciadorDeTarefas: main Usuario Tarefa Grupo Prioridade Storage Terminal FileUtil Etiqueta StringUtil
	$(COMPILE) $(LFLAGS) build/main.o build/Usuario.o build/Tarefa.o build/Grupo.o build/Prioridade.o build/Storage.o build/Terminal.o build/FileUtil.o build/Etiqueta.o build/StringUtil.o -o GerenciadorDeTarefas.exe

run: GerenciadorDeTarefas
	./GerenciadorDeTarefas.exe

Testes: testes Usuario Tarefa Grupo Prioridade Storage Terminal FileUtil Etiqueta StringUtil
	$(COMPILE) $(LFLAGS) build/testes.o build/Usuario.o build/Tarefa.o build/Grupo.o build/Prioridade.o build/Storage.o build/Terminal.o build/FileUtil.o build/Etiqueta.o build/StringUtil.o -o Testes.exe
	./Testes.exe

report: coverage.info
	genhtml coverage.info --output-directory report

clean:
	$(RM) Testes.exe GerenciadorDeTarefas.exe coverage.info build/ report/


main: src/main.cpp
	mkdir -p ${dir $@}build
	$(COMPILE) $(CFLAGS) $(INCLUDE) $(LFLAGS) src/main.cpp -o build/main.o

testes: src/testes.cpp include/doctest.h
	mkdir -p ${dir $@}build
	$(COMPILE) $(CFLAGS) $(INCLUDE) $(LFLAGS) src/testes.cpp -o build/testes.o

Usuario: $(USUARIOS)/usuario.cpp include/usuario.hpp
	mkdir -p ${dir $@}build
	$(COMPILE) $(CFLAGS) $(INCLUDE) $(LFLAGS) $(USUARIOS)/usuario.cpp -o build/Usuario.o

Tarefa: $(TAREFAS)/tarefa.cpp include/tarefa.hpp
	mkdir -p ${dir $@}build
	$(COMPILE) $(CFLAGS) $(INCLUDE) $(LFLAGS) $(TAREFAS)/tarefa.cpp -o build/Tarefa.o

Etiqueta: $(TAREFAS)/etiqueta.cpp include/etiqueta.hpp
	mkdir -p ${dir $@}build
	$(COMPILE) $(CFLAGS) $(INCLUDE) $(LFLAGS) $(TAREFAS)/etiqueta.cpp -o build/Etiqueta.o

Grupo: $(TAREFAS)/grupo.cpp include/grupo.hpp Etiqueta
	mkdir -p ${dir $@}build
	$(COMPILE) $(CFLAGS) $(INCLUDE) $(LFLAGS) $(TAREFAS)/grupo.cpp -o build/Grupo.o

Prioridade: $(TAREFAS)/prioridade.cpp include/prioridade.hpp Etiqueta
	mkdir -p ${dir $@}build
	$(COMPILE) $(CFLAGS) $(INCLUDE) $(LFLAGS) $(TAREFAS)/prioridade.cpp -o build/Prioridade.o

Storage: $(APLICACAO)/storage.cpp include/storage.hpp
	mkdir -p ${dir $@}build
	$(COMPILE) $(CFLAGS) $(INCLUDE) $(LFLAGS) $(APLICACAO)/storage.cpp -o build/Storage.o

Terminal: $(APLICACAO)/terminal.cpp include/terminal.hpp
	mkdir -p ${dir $@}build
	$(COMPILE) $(CFLAGS) $(INCLUDE) $(LFLAGS) $(APLICACAO)/terminal.cpp -o build/Terminal.o

FileUtil: $(UTIL)/file_util.cpp include/file_util.hpp
	mkdir -p ${dir $@}build
	$(COMPILE) $(CFLAGS) $(INCLUDE) $(LFLAGS) $(UTIL)/file_util.cpp -o build/FileUtil.o

StringUtil: $(UTIL)/string_util.cpp include/string_util.hpp
	mkdir -p ${dir $@}build
	$(COMPILE) $(CFLAGS) $(INCLUDE) $(LFLAGS) $(UTIL)/string_util.cpp -o build/StringUtil.o

gcov:
	$(RM) *.gcov
	gcov build/main.o build/Usuario.o build/Tarefa.o build/Etiqueta.o build/Grupo.o build/Prioridade.o build/Storage.o build/Terminal.o build/FileUtil.o build/StringUtil.o
	mv *.gcov build/

coverage.info: gcov
	lcov --capture --directory build/ --output-file coverage.info
	lcov --remove coverage.info "/usr*" "*doctest.h" --output-file coverage.info
	$(RM) build/*.gcda build/*.gcno