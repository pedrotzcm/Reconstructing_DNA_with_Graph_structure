#include "graph.h"
using namespace std;





void vertice::apaga_aresta(vertice *receptor){
    for(int i = 0; i < vizinhos.size(); i++){
        if(vizinhos[i]->fragmento_de_DNA == receptor->fragmento_de_DNA){
            vizinhos.erase(vizinhos.begin() + i);
            break;
        }
    }
}

    


vertice::vertice(string fragmento_a_inserir){
    fragmento_de_DNA = fragmento_a_inserir;            
}

        

void vertice::elimina_ciclos(vertice* vertice_original, vector<vertice*> vertices_visitados) { //elimiina ciclos que contem o respectivo vértice com a função apaga aresta

    if(find(vertices_visitados.begin(), vertices_visitados.end(), this) != vertices_visitados.end()){
                return;
    }

    vertices_visitados.push_back(this); //uso de um vetor que armazena os vértices que ja visitamos

    if (vizinhos.empty()) {
        return;
    }

    for (vertice* viz : vizinhos) {
        if (viz == nullptr) {
            continue;
        }

        for (vertice* viz_viz : viz->vizinhos) {
            if (viz_viz != nullptr && viz_viz->fragmento_de_DNA == vertice_original->fragmento_de_DNA) { //o objetivo é apenas apagar uma única aresta, quebrando o ciclo e minimizando a perda de um
                                                                                                                //possivel comprimento máximo para a posterior remontagem do dna  
                viz->apaga_aresta(vertice_original); 
                return;
            }
        }

        viz->elimina_ciclos(vertice_original, vertices_visitados); //chamada recursiva da função
    }

    vertices_visitados.pop_back();
}

void vertice::acha_maior_caminho( vector <vertice*>& caminho, vector <vertice*>& caminho_MAX, vector <vertice*>& visitados){ 
    //método aplicado a um vértice para achar qual o maior caminho possivel tendo tal vértice como origem
    //é uma função void, porem retorna via ponteiros um vetor de vertices* que representa esse maior caminho
    //e tambem utiliza um vetor de vertices visitados
    caminho.push_back(this);
    visitados.push_back(this);

    if(vizinhos.size() == 0){
        if(caminho_MAX.size() == 0){ //aqui é so um facilitador para a primeira iteração, quando o caminho_max ainda e nulo
            caminho_MAX = caminho;
            caminho.pop_back();
            return;
        }

        else{
            if(caminho.size() > caminho_MAX.size()){ //substitui o caminho_MAX pelo respectivo caminho, que é maior
                caminho_MAX = caminho;
            }

            caminho.pop_back(); //"retrocede" um vértice
            return;
        }
    }

    else{
        while(todos_pertencem(vizinhos, visitados) == false){ //queremos que visitem todos os vértices vizinhos
            for(vertice* viz : vizinhos){
                viz->acha_maior_caminho(caminho, caminho_MAX, visitados);
            }
            caminho.pop_back();
            return;
        }

    }
}

    






digrafo::digrafo(){

}   

void digrafo::adiciona_vertice(string fragmento){ //adiciona um vértice na estrutura do grafo
    for(vertice* vertice : vertices_do_grafo){
        if(vertice->fragmento_de_DNA == fragmento){
            return;
        }
    }
    vertice* vertice_novo = new vertice(fragmento);
    vertices_do_grafo.push_back(vertice_novo);
}

void digrafo::print_grafo(){ //imprime o grafo (lista de adjacências)
    for (vertice* vertice_da_vez: vertices_do_grafo){
        cout << vertice_da_vez->fragmento_de_DNA << " -> ";
        for(vertice* vizinho : vertice_da_vez->vizinhos){
            cout << vizinho->fragmento_de_DNA << " ";
        }
        cout << endl;
    }
}

void digrafo::adiciona_arco(string fragmento_origem, string fragmento_destino){
    //adiciona uma aresta direcionada entre dois vertices que carregam as strings passadas como parametro
    vertice* vertice_origem = nullptr;
    vertice* vertice_destino = nullptr;

    for(vertice* vertice: vertices_do_grafo){
        if(vertice->fragmento_de_DNA == fragmento_origem){
            vertice_origem = vertice;
        }
        if(vertice->fragmento_de_DNA == fragmento_destino){
            vertice_destino = vertice;
        }
        if(vertice_destino && vertice_origem){
            break;
        }
    }

    if(vertice_origem && vertice_destino){
        vertice_origem->vizinhos.push_back(vertice_destino);
    }
}






string decodifica_vetor_caminho_max(int k, vector <vertice*> caminho_max){ 
    //função que apenas pega o vetor de caminho_max e escreve a fita "advinhada"
        string string_return = "";
        string parte_string;
        string_return = string_return + caminho_max[0]->fragmento_de_DNA;
        for(vertice* v: caminho_max){
            if (v->fragmento_de_DNA == caminho_max[0]->fragmento_de_DNA){
                continue;
            }
            else{
                string_return = string_return + v->fragmento_de_DNA.substr(k); 
                
            }
        }

        return string_return;
        
    }

bool checagem(string fragmento1, string fragmento2, int k){
    //função auxiliar utilizada para identificar quando colocamos um arco entre dois vertices ou não
    string substring1, substring2;
    substring1 = fragmento1.substr(fragmento1.size() - k, k);
    substring2 = fragmento2.substr(0, k);

    if (substring1 == substring2){
        return true;
    }
    return false;
}
string *shotgun(int min_fragmento, int max_fragmento, int qtd_fragmento, string DNA){
    //função utilizada para produzir os fragmentos da fita de DNA de forma randomica, com tamanho minimo
    //sendo igual ao int min_fragmento e o tamanho máximo sendo max_fragmento)
    int tamanho_do_frag;
    int posicao_inicial;
    string fragmento = "";
    random_device rd;
    mt19937 generator(rd());
    auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    generator.seed(seed);
    uniform_int_distribution<int> distribution_tamanho(min_fragmento, max_fragmento);

    string *lista_de_fragmentos = new string[qtd_fragmento];
    for (int i = 0; i < qtd_fragmento; i++){
        tamanho_do_frag = distribution_tamanho(generator);
        uniform_int_distribution<int> distribution_posicao(0, DNA.size() - tamanho_do_frag);
        posicao_inicial = distribution_posicao(generator);
        for(int j = posicao_inicial; j < posicao_inicial + tamanho_do_frag; j++){
            fragmento = fragmento + DNA[j];
        }
        lista_de_fragmentos[i] = fragmento;
        fragmento = "";

    }

    return lista_de_fragmentos;

}


digrafo produzir_grafo(string nome_do_arquivo){ //lê um arquivo e monta um digrafo como pedido no enunciado
    digrafo digrafo_da_vez;
    ifstream file(nome_do_arquivo);

    string linha;
    getline(file, linha);
    string primeira, segunda;
    istringstream iss(linha);
    iss >> primeira >> segunda;
    int num_vertices, num_arcos;
    num_vertices = stoi(primeira);
    num_arcos = stoi(segunda);
    for (int i = 0; i < num_arcos ; i++){
        getline(file, linha);
        istringstream iss2(linha);
        iss2 >> primeira >> segunda;
        digrafo_da_vez.adiciona_vertice(primeira);
        digrafo_da_vez.adiciona_vertice(segunda);
        digrafo_da_vez.adiciona_arco(primeira, segunda);

    }

    return digrafo_da_vez;
}



bool todos_pertencem(vector<vertice*>& vetor1,  vector<vertice*>& vetor2) {
    //checa se todos os elementos de um vetor1 estão em um vetor2
    for ( auto& vertex : vetor1) {
        if (find(vetor2.begin(), vetor2.end(), vertex) == vetor2.end()) {
            return false;
        }
    }
    return true;
}

