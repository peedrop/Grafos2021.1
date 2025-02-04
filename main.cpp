#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <utility>
#include <tuple>
#include <iomanip>
#include <stdlib.h>
#include <chrono>
#include <vector>
#include "Grafo.h"
#include "No.h"

using namespace std;

// Inicio da funcao para leitura do arquivo de entrada
Grafo *leitura(ifstream &arquivo, int direcionado, int aresta_ponderado, int no_ponderado) {

    int id_no_origem;
    int id_no_alvo;
    int ordem;

    // Pegando a ordem do grafo
    arquivo >> ordem;

    // Criando objeto grafo
    Grafo *grafo = new Grafo(ordem, direcionado, aresta_ponderado, no_ponderado);

    // Leitura do arquivo
    if (!grafo->getArestaPonderado() && !grafo->getNoPonderado()) {
        // Primeiro caso: aresta sem peso e no sem peso
        float peso_aresta;
        while (arquivo >> id_no_origem >> id_no_alvo) {
            // Aresta sem peso por padrao recebe peso 1
            peso_aresta = 1;
            grafo->inserirAresta(id_no_origem, id_no_alvo, peso_aresta);
        }
    } else if (grafo->getArestaPonderado() && !grafo->getNoPonderado()) {
        // Segundo caso: aresta com peso e no sem peso
        float peso_aresta;
        while (arquivo >> id_no_origem >> id_no_alvo >> peso_aresta) {
            grafo->inserirAresta(id_no_origem, id_no_alvo, peso_aresta);
        }
    } else if (grafo->getNoPonderado() && !grafo->getArestaPonderado()) {
        // Terceiro caso: aresta sem peso e no com peso
        float peso_no_origem, peso_no_alvo, peso_aresta;
        while (arquivo >> id_no_origem >> peso_no_origem >> id_no_alvo >> peso_no_alvo) {
            peso_aresta = 1;
            grafo->inserirAresta(id_no_origem, id_no_alvo, peso_aresta);
            grafo->getNo(id_no_origem)->setPeso(peso_no_origem);
            grafo->getNo(id_no_alvo)->setPeso(peso_no_alvo);
        }
    } else if (grafo->getNoPonderado() && grafo->getArestaPonderado()) {
        // Quarto caso: aresta com peso e no com peso
        float peso_no_origem, peso_no_alvo, peso_aresta;
        while (arquivo >> id_no_origem >> peso_no_origem >> id_no_alvo >> peso_no_alvo >> peso_aresta) {
            grafo->inserirAresta(id_no_origem, id_no_alvo, peso_aresta);
            grafo->getNo(id_no_origem)->setPeso(peso_no_origem);
            grafo->getNo(id_no_alvo)->setPeso(peso_no_alvo);
        }
    }

    // Inserindo Nos que faltam
    while (grafo->getOrdem() != ordem) {
        grafo->inserirNo(grafo->getOrdem() + 1);
    }

    return grafo;
}
// Fim da funcao para leitura do arquivo de entrada

// Inicio da funcao para leitura do arquivo de entrada do segundo trabalho
Grafo *leituraInstancias(ifstream &arquivo) {
    int direcionado = 0;
    int aresta_ponderado = 1;
    int no_ponderado = 0;

    // Criando objeto grafo
    int ordem = 0;
    Grafo *grafo = new Grafo(ordem, direcionado, aresta_ponderado, no_ponderado);

    // Flag de controle para saber se está lendo os Nos ou as arestas
    bool inserindoNos = true;

    // Percorrendo linha por linha do arquivo de entrada
    for (string line; getline(arquivo, line);) {
        if (line == "" || line == "\r" || line == "\n") {
            inserindoNos = false;
        }

        if (inserindoNos) {
            grafo->inserirNoComGrupo(stoi(line));
        } else {
            int id_no_origem, id_no_alvo;
            float peso_aresta;
            while (arquivo >> id_no_origem >> id_no_alvo >> peso_aresta) {
                grafo->inserirAresta(id_no_origem, id_no_alvo, peso_aresta);
            }
            break;
        }
    }

    cout << "Leitura concluida com sucesso! Nos: " << grafo->getOrdem() << " e Arestas: " << grafo->getNumArestas()
         << endl;
    cout << "Quantidade de Grupos: " << grafo->getQtdGrupos() << endl;

    return grafo;
}
// Fim da funcao para leitura do arquivo de entrada do segundo trabalho

// Inicio funcao salvar string em .dot
void salvarStringDot(string retorno, ofstream &arquivo_saida) {
    arquivo_saida << retorno;
}
// Fim funcao salvar string em .dot

// Inicio funcao opcoes no menu
int menu() {
    int selecao;

    cout << "MENU" << endl;
    cout << "----" << endl;
//    cout << "[1] Fecho Transitivo Direto" << endl;
//    cout << "[2] Fecho Transitivo Indireto" << endl;
//    cout << "[3] Caminho Minimo entre dois vertices - Dijkstra" << endl;
//    cout << "[4] Caminho Minimo entre dois vertices - Floyd" << endl;
//    cout << "[5] Arvore Geradora Minima de Prim" << endl;
//    cout << "[6] Arvore Geradora Minima de Kruskal" << endl;
//    cout << "[7] Arvore do Caminhamento em Profundidade" << endl;
//    cout << "[8] Imprimir Ordenacao Topologica" << endl;
//    cout << "[9] Imprimir Grafo de Entrada" << endl;
    cout << "[1] Algoritmo Guloso AGMG" << endl;
    cout << "[2] Algoritmo Guloso AGMG Randomizado" << endl;
    cout << "[3] Algoritmo Guloso AGMG Randomizado Reativo" << endl;
    cout << "[0] Sair" << endl;

    cin >> selecao;
    return selecao;
}
// Fim funcao opcoes no menu

// Inicio funcao menu salvar arquivo
int menuSalvar() {
    int selecao;

    cout << "Voce deseja salvar o resultado da operacao?" << endl;
    cout << "[0] Nao" << endl;
    cout << "[1] Sim" << endl;

    cin >> selecao;

    return selecao;
}
// Fim funcao menu salvar arquivo

// Inicio funcao selecionar no menu
void selecionar(int selecao, Grafo *grafo, ofstream &arquivo_saida) {
    //String de retorno dos resultados das funcoes, utilizada para exportar o grafo em .dot no arquivo de saida.
    string retorno = "";
    switch (selecao) {
        // Fecho Transitivo Direto
//        case 1: {
//            cout << "Digite o ID do vertice de origem: " << endl;
//            int id;
//            cin >> id;
//            retorno = grafo->fechoTD(id);
//            break;
//        }
//            // Fecho Transitivo Indireto
//        case 2: {
//            cout << "Digite o ID do vertice de origem: " << endl;
//            int id;
//            cin >> id;
//            retorno = grafo->fechoTIRec(id);
//            break;
//        }
//            // Caminho Minimo entre dois vertices - Dijkstra
//        case 3: {
//            cout << "Digite o ID do vertice de origem: " << endl;
//            int id;
//            cin >> id;
//            cout << "Digite o ID do vertice alvo: " << endl;
//            int id_alvo;
//            cin >> id_alvo;
//            retorno = grafo->djikstra(id, id_alvo);
//            break;
//        }
//            // Caminho Minimo entre dois vertices - Floyd
//        case 4: {
//            cout << "Digite o ID do vertice de origem: " << endl;
//            int id;
//            cin >> id;
//            cout << "Digite o ID do vertice alvo: " << endl;
//            int id_alvo;
//            cin >> id_alvo;
//            retorno = grafo->floyd(id, id_alvo);
//            break;
//        }
//            // Arvore Geradora Minima de Prim
//        case 5: {
//            cout << "Digite a ordem do subgrafo de entrada: ";
//            int tamanho;
//            cin >> tamanho;
//            int vertices[tamanho];
//            for (int i = 0; i < tamanho; i++) {
//                cout << "Digite o " << std::to_string(i + 1) << "o vertice: ";
//                cin >> vertices[i];
//                cout << endl;
//            }
//            Grafo *subGrafo = grafo->subgrafo(vertices, tamanho);
//            retorno = subGrafo->agmPrim();
//            break;
//        }
//            // Arvore Geradora Minima de Kruskal
//        case 6: {
//            cout << "Digite a ordem do subgrafo de entrada: ";
//            int tamanho;
//            cin >> tamanho;
//            int vertices[tamanho];
//            for (int i = 0; i < tamanho; i++) {
//                cout << "Digite o " << std::to_string(i + 1) << "o vertice: ";
//                cin >> vertices[i];
//                cout << endl;
//            }
//            Grafo *subGrafo = grafo->subgrafo(vertices, tamanho);
//            retorno = subGrafo->agmKruskal();
//            break;
//        }
//            // Arvore pelo Caminhamento em Profundidade
//        case 7: {
//            int id;
//            cout << "Digite o id de origem: ";
//            cin >> id;
//            retorno = grafo->buscaProf(id);
//            break;
//        }
//            // Imprimir Ordenacao Topologica
//        case 8: {
//            retorno = grafo->ordTopologica();
//            break;
//        }
//            // Imprimir Grafo de Entrada
//        case 9: {
//            retorno = grafo->imprimir();
//            break;
//        }
            // Algoritmo Guloso AGMG
        case 1: {
            retorno = grafo->AGMGPrim();
            break;
        }
            // Algoritmo Guloso AGMG Randomizado
        case 2: {
            int qnt_it;
            cout << "Digite a quantidade de iteracoes: " << endl;
            cin >> qnt_it;
            float alfa;
            cout << "Digite o alfa: " << endl;
            cin >> alfa;
            retorno = grafo->AGMGPrimRandomizado(alfa, qnt_it);
            break;
        }
            // Algoritmo Guloso AGMG Randomizado Reativo
        case 3: {
            retorno = grafo->AGMGRandReativo();
            break;
        }
        case 0: {
            cout << "Encerrando o programa" << endl;
            return;
        }
        default: {
            cout << "Erro: Opcao invalida!" << endl;
        }
    }
    //Chamada da funcao para verificar se o usuario quer salvar o grafo no arquivo de saida
    int selecaoSalvar = menuSalvar();
    if (selecaoSalvar) {
        salvarStringDot(retorno, arquivo_saida);
        cout << retorno << endl;
    } else {
        salvarStringDot("Voce nao salvou a operacao", arquivo_saida);
        cout << "Voce nao salvou a operacao" << endl;
    }
}
// Fim funcao selecionar no menu

// Inicio funcao main menu
int mainMenu(ofstream &arquivo_saida, Grafo *grafo) {
    int selecao = 1;

    while (selecao != 0) {
//        system("@cls||clear");
        selecao = menu();

        if (arquivo_saida.is_open()) {
            selecionar(selecao, grafo, arquivo_saida);
        } else {
            cout << "Erro ao abrir o arquivo de saida!" << endl;
        }

        arquivo_saida << endl;
    }

    return 0;
}
// Fim funcao main menu

// Inicio funcao main
int main(int argc, char const *argv[]) {

    // Verifica se todos parametros necessarios para compilar foram definidos
    if (argc != 3) {
        cout
//                << "Erro: Esperando: ./<program_name> <arquivo_entrada> <arquivo_saida> <direcionado> <aresta_ponderado> <no_ponderado> "
                << "Erro: Esperando: ./<program_name> <arquivo_entrada> <arquivo_saida>"
                << endl;
        return 1;
    }

    string nome_programa(argv[0]);
    string nome_arquivo_entrada(argv[1]);

    string instancia;
    if (nome_arquivo_entrada.find("v") <= nome_arquivo_entrada.size()) {
        string instancia = nome_arquivo_entrada.substr(nome_arquivo_entrada.find("v"));
        cout << "Executando " << nome_programa << " com instancia " << instancia << " ... " << endl;
    }

    // Abrindo arquivos de entrada e saida
    ifstream arquivo_entrada;
    ofstream arquivo_saida;
    arquivo_entrada.open(argv[1], ios::in);
    arquivo_saida.open(argv[2], ios::out | ios::trunc);

    // Criando grafo da leitura do arquivo
    Grafo *grafo;

    // Verificando se conseguiu abrir o arquivo de entrada
    if (arquivo_entrada.is_open()) {
//        grafo = leituraInstancias(arquivo_entrada, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
        grafo = leituraInstancias(arquivo_entrada);
    } else {
        cout << "Nao foi possivel abrir o arquivo de entrada: " << argv[1];
    }

    // Chamando main menu
    mainMenu(arquivo_saida, grafo);

    // Fechando arquivo de entrada
    arquivo_entrada.close();

    // Fechando arquivo de saída
    arquivo_saida.close();

    return 0;
}
// Fim funcao main