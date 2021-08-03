#include "Grafo.h"
#include "No.h"
#include "Aresta.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <queue>
#include <list>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <float.h>
#include <iomanip>
# include <limits>


using namespace std;

Grafo::Grafo(int ordem, bool direcionado, bool aresta_ponderado, bool no_ponderado) {
    this->ordem = ordem;
    this->direcionado = direcionado;
    this->aresta_ponderado = aresta_ponderado;
    this->no_ponderado = no_ponderado;
    this->primeiro_no = this->ultimo_no = nullptr;
    this->num_arestas = 0;

    for(int i = 0; i < ordem; i++) {
        this->inserirNo(i);
    }
}

Grafo::~Grafo() {
    No* prox_no = this->primeiro_no;

    while (prox_no != nullptr) {
        prox_no->removerTodasArestas();
        No* aux_no = prox_no->getProxNo();
        delete prox_no;
        prox_no = aux_no;
    }
}

int Grafo::getOrdem() {
    return this->ordem;
}

int Grafo::getNumArestas() {
    return this->num_arestas;
}

bool Grafo::getDirecionado() {
    return this->direcionado;
}

bool Grafo::getArestaPonderado() {
    return this->aresta_ponderado;
}

bool Grafo::getNoPonderado() {
    return this->no_ponderado;
}

No* Grafo::getPrimeiroNo() {
    return this->primeiro_no;
}

No* Grafo::getUltimoNo() {
    return this->ultimo_no;
}

No* Grafo::getNo(int id) {
    if(this->primeiro_no != nullptr) {
        for (No* no = this->primeiro_no; no != nullptr; no = no->getProxNo()) {
            if(no->getId() == id) {
                return no;
            }
        }
    }
    return NULL;
}

bool Grafo::existeNo(int id) {
    if(this->primeiro_no != nullptr) {
        for (No* no = this->primeiro_no; no != nullptr ; no = no->getProxNo()) {
            if(no->getId() == id) {
                return true;
            }
        }
    }
    return false;
}

void Grafo::inserirNo(int id) {
    No* no = new No(id);

    if(this->primeiro_no == nullptr){
        this->primeiro_no = no;
        this->ultimo_no = no;
    }else{
        this->ultimo_no->setProxNo(no);
        this->ultimo_no = no;
    }
    ordem ++;
}

void Grafo::removerNo(int id) {
    if(existeNo(id)) {
        No* node = this->getPrimeiroNo();
        No* temp = nullptr;

        while(node->getId() != id) {
            temp = node;
            node = node->getProxNo();
        }

        if(temp != nullptr)
            temp->setProxNo(node->getProxNo());
        else
            this->primeiro_no = node->getProxNo();

        if(node == this->ultimo_no)
            this->ultimo_no = temp;

        if(node->getProxNo() == this->ultimo_no)
            this->ultimo_no = node->getProxNo();

        No* aux = this->getPrimeiroNo();

        while(aux != nullptr) {
            aux->removerAresta(node->getId(), this->direcionado, node);
            aux = aux->getProxNo();
        }

        delete node;
        this->ordem--;
    }
}

void Grafo::inserirAresta(int id, int id_alvo, float peso) {
    if(existeNo(id) && existeNo(id_alvo)) {
        No* no_origem = this->getNo(id);
        No* no_alvo = this->getNo(id_alvo);
        if(this->direcionado) {
            no_origem->inserirAresta(id_alvo, peso);
            no_origem->aumentarGrauSaida();
            no_alvo->aumentarGrauEntrada();
        }else {
            no_origem->inserirAresta(id_alvo, peso);
            no_alvo->inserirAresta(id, peso);
            no_origem->aumentarGrauSaida();
            no_origem->aumentarGrauEntrada();
            no_alvo->aumentarGrauSaida();
            no_alvo->aumentarGrauEntrada();
        }
    }else{
        cout << "Erro: Algum dos Nos da aresta nao existe!" << endl;
    }
}

void Grafo::imprimir() {
    cout << "Funcao imprimir Grafo: " << endl;
    if(this->primeiro_no != nullptr) {
        for(No* no = this->primeiro_no; no != nullptr; no = no->getProxNo()) {
            cout << "No: "  << no->getId() << endl;
            for(Aresta* aresta = no->getPrimeiraAresta(); aresta != nullptr; aresta = aresta->getProxAresta()){
                if(!aresta->getDirecionado() || this->direcionado){
                    cout << no->getId()  << " " << aresta->getIdAlvo() << endl;
                }
            }
        }
    }else {
        cout << "Grafo vazio" << endl;
    }
}

void Grafo::salvarDot() {
    std::ofstream saida("saida.dot");

    if(!saida.is_open()){
        std::cerr << "Erro na abertura do arquivo" << std::endl;
    }

    if(this->primeiro_no != nullptr) {
        if(this->direcionado) {
            saida << "digraph {" << std::endl;
        }else {
            saida << "strict graph {" << std::endl;
        }
        for(No* no = this->primeiro_no; no != nullptr; no = no->getProxNo()) {
            for(Aresta* aresta = no->getPrimeiraAresta(); aresta != nullptr; aresta = aresta->getProxAresta()){
                if(this->direcionado) {
                    saida << "\t" << no->getId()  << " -> " << aresta->getIdAlvo() << ";" << std::endl;
                }else {
                    saida << "\t" << no->getId()  << " -- " << aresta->getIdAlvo() << ";" << std::endl;
                }
            }
        }
        saida << "}" << std::endl;
    }else {
        saida << "Erro: Grafo vazio" << std::endl;
    }
}

// Funcionalidades do trabalho

// Inicio Fecho Transitivo Direto
string Grafo::fechoTD(int id) {
    if(!this->direcionado) {
        cout << "Erro: O grafo precisa ser direcionado!" << endl;
        return "";
    }

    // Criando string de retorno
    string retorno = "Fecho transitivo direto \n";
    retorno += "digraph { \n";

    // Declarando vetor bool de nos visitados
    bool visitados[ordem];

    // Pre-definindo que nenhum no do vertice foi visitado ainda
    for(int i = 0; i < ordem; i++)
        visitados[i] = false;

    // Definindo que o no inicial foi visitado
    visitados[id] = true;

    // Criando uma pilha auxiliar e colocando o no inicial no topo dela
    stack<int> pilhaAux;
    pilhaAux.push(id);

    // Enquanto a pilha nao for vazia fica rodando
    while(!pilhaAux.empty()) {

        // Recuperando o no do primeiro id da pilha
        No* no = this->getNo(pilhaAux.top());

        // Removendo o topo da pilha
        pilhaAux.pop();

        // Percorrendo as arestas do no que estava no topo da pilha e acabou de ser removido
        for(Aresta* aresta = no->getPrimeiraAresta(); aresta != nullptr; aresta = aresta->getProxAresta()) {
            // Verifica se o no alvo de cada aresta ainda nao foi visitado
            if(!visitados[aresta->getIdAlvo()]) {
                // Se ele ainda nao foi visitado, coloca ele como visitado
                visitados[aresta->getIdAlvo()] = true;
                // Exibindo para o usuario em execucao
//                cout << "\t" << no->getId() << " -> " << aresta->getIdAlvo() << endl;
                // Preenchendo string de retorno para salvar no arquivo .dot depois
                retorno += "\t" + std::to_string(no->getId()) + " -> " + std::to_string(aresta->getIdAlvo()) + "\n";
                /* Adiciona o id do alvo da aresta atual que esta no loop sendo o proximo
                no a ser verificado, alimentando o while principal da funcao */
                pilhaAux.push(aresta->getIdAlvo());
            }
        }
    }
    return retorno + "} \n" + "---------------------";
}
// Fim Fecho Transitivo Direto

// Inicio Fecho Transitivo Indireto
string Grafo::fechoTI(int id) {
    if(!this->direcionado) {
        cout << "Erro: O grafo precisa ser direcionado!" << endl;
        return "";
    }

    // Criando string de retorno
    string retorno = "Fecho transitivo indireto \n";
    retorno += "digraph { \n";

    // Criando uma pilha auxiliar e colocando o no inicial no topo dela
    stack<int> pilhaAux;
    pilhaAux.push(id);

    // Enquanto a pilha nao for vazia fica rodando
    while(!pilhaAux.empty()) {

        // Recuperando o no do primeiro id da pilha
        int topo = pilhaAux.top();

        // Removendo o topo da pilha
        pilhaAux.pop();

        // Percorrendo Nos do grafo a partir do No que estava no topo da pilha e acabou de ser removido
        for(No* no = this->getPrimeiroNo(); no != nullptr; no = no->getProxNo()) {
            // Verifica se o No alvo possui aresta com o No atual do for
            if(no->existeArestaEntre(topo)) {
                // Exibindo para o usuario em execucao
//                cout << "\t" << no->getId()  << " -> " << topo << endl;
                // Preenchendo string de retorno para salvar no arquivo .dot depois
                retorno += "\t" + std::to_string(no->getId())  + " -> " + std::to_string(topo) + "\n";
                // No a ser verificado, alimentando o while principal da funcao
                pilhaAux.push(no->getId());
            }
        }
    }
    return retorno + "} \n" + "---------------------";
}
// Fim Fecho Transitivo Indireto

// Inicio Caminho Minimo por Djikstra

string Grafo::djikstra(int id, int id_alvo){
    //Verificando se o grafo possui peso nas arestas, e retornando caso não seja.
    if(!aresta_ponderado){
        cout << "Grafo Invalido! Arestas nao ponderadas! \n";
        return "";
    }
    //Variavel infinito utilizada para numerar as distâncias entre vértices que não possuem caminho
    int infinito = std::numeric_limits<int>::max();
    //Vetor booleano para verificar quais vertices foram visitados
    bool visitados[this->ordem];
    //vetor para armazenar a distância entre os vértices
    int dist[this->ordem];
    //Vetor para auxiliar a "gravar" o caminho entre os vértices
    int auxCaminho[this->ordem];
    //Inicializando as distâncias entre os vértices como infinito, e marcando os vértices como não visitados
    for(int i=0; i<this->ordem; i++){
        dist[i] = infinito;
        visitados[i] = false;
        auxCaminho[i] = id;
    }
    //Inicializando a distância do vértice de origem para ele mesmo como 0, e marcando-o como visitado
    dist[id] = 0;
    visitados[id] = true;
    No* noAux = this->getNo(id);
    /* Percorrendo as arestas do vértice de origem, calculando as distâncias entre ele e seus adjacentes,
    e gravando-as no vetor dist[] */
    for(Aresta* arestaAux = noAux->getPrimeiraAresta(); arestaAux != nullptr; arestaAux = arestaAux->getProxAresta()){
        dist[arestaAux->getIdAlvo()] = arestaAux->getPeso();
    }
    //Laço que percorre todos os vértices do grafo
    for(int i = 0; i < this->ordem-1; i++){
        //Chamada da função que calcula o id do vértice de menor distância (entre os vértices não visitados) aos já visitados
        int j = distMinima(visitados, dist);
        if(j == id_alvo)
            continue;
        //Marca o vértice de menor distância como visitado
        visitados[j] = true;
        //Laço que percorre os vértices do grafo
        for(int k = 0; k < this->ordem; k++){
            No* atual = getNo(j);
            /* condição que atualiza o valor de dist caso o vértice sendo verificado (k) não tenha sido visitado,
            a distância do nó atual (j) até o nó de origem não seja infinito, e caso exista aresta entre os nós k e j */
            if(!visitados[k] && dist[j] != infinito && atual->existeArestaEntre(k)){
                Aresta* arestaEntre = atual->getArestaEntre(k);
                //Atualiza o valor da distância caso o novo cálculo seja menor do que o anterior.
                if(dist[j] + arestaEntre->getPeso() < dist[k]){
                    dist[k] = dist[j] + arestaEntre->getPeso();
                    /*"Grava" o caminho no vetor auxCaminho, para ser realizada a impressão no arquivo de saida no final
                    A ideia é gravar de onde o vértice k veio, armazenando na posiçao k o id de j */
                    auxCaminho[k] = j;
                }
            }
        }
    }
    // Criando string de retorno
    string retorno = "Fecho transitivo indireto \n";
    //Colocando o cabeçalho do grafo corretamente, verificando se é grafo ou digrafo.
    if(direcionado)
        retorno += "digraph { \n";
    else
        retorno += "strict graph { \n";
    /* Caso a distância do vértice alvo ao vértice de origem seja infinito, não existe caminho entre eles.
    Portanto, é impresso na tela uma mensagem de erro e encerrada a função */
    if(dist[id_alvo] == infinito){
        cout << "Nao existe caminho entre os vertices." << endl;
        return "Djikstra: Vertices de entrada invalidos! \n";
    }
    //variável apra auxiliar a na criação do texto em .dot
    int k = id_alvo;
    //Criação do texto em .dot, que é colocado na string de retorno
    do{
        int auxK;
        auxK = k;
        //Aqui, utilizamos o vetor auxiliar para ir "retornando" no caminho que fazemos do vértice origem até o alvo
        k = auxCaminho[k];
        retorno += "\t" + std::to_string(k)  + " -- " + std::to_string(auxK) + "\n";
    }while(k != id);
    return retorno + "} \n" + "---------------------";
}
    //Fim Caminho Minimo por Djikstra

    //Início função auxiliar para calcular o vértice de menor distância dentre os vértices não visitados
int Grafo::distMinima(bool visitados[], int dist[]){
    //Variável de valor mínimo, inicializada como + infinito
    int min = std::numeric_limits<int>::max();
    //Variável para gravar o id do menor vértice
    int idMenor;
    //Laço que percorre o grafo
    for(int i=0; i<ordem; i++){
        //Caso o vértice não tenha sido visitado e tenha distância menor que a variável 'min', o mesmo passa a ser o menor
        if(visitados[i]==false && dist[i]<=min){
            min = dist[i];
            idMenor = i;
        }
    }
    //Retorna o vértice de menor valor de distância
    return idMenor;
}
    //Fim função auxiliar

void Grafo::buscaProf(int id) {
    list<int> listaVisitados;
    listaVisitados.push_back(id);

    auxBuscaProf(&listaVisitados, id);

    Grafo* grafo = new Grafo(listaVisitados.size(), 0, 0, 0);

    // imprimindo lista
    list<int>::iterator it;
    cout << "imprimindo lista" << endl;
    while(!listaVisitados.empty()) {
        grafo->inserirNo(listaVisitados.back());
        cout << listaVisitados.back() << endl;
        listaVisitados.pop_back();
    }
    grafo->salvarDot();
    cout << "fim imprimindo lista" << endl;
}

void Grafo::auxBuscaProf(list<int>* listaVisitados, int id) {
    No* no = this->getNo(id);

    for(Aresta* aresta = no->getPrimeiraAresta(); aresta != nullptr; aresta = aresta->getProxAresta()) {
        if(!auxBuscaLista(listaVisitados, aresta->getIdAlvo())){
            listaVisitados->push_back(aresta->getIdAlvo());
            auxBuscaProf(listaVisitados, aresta->getIdAlvo());
        }
    }
}

bool Grafo::auxBuscaLista(list<int>* lista, int id) {
    list<int>::iterator it;
    for(it = lista->begin(); it!=lista->end();it++){
        if(*it == id) {
            return true;
        }
    }
    return false;
}