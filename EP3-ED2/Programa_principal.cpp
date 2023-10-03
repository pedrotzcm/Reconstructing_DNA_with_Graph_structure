#include "graph.h"
using namespace std;


int main(){
    
    ifstream file("arquivo_da_string.txt");
    
    string sequencia;

    int k = 4;
    int qtd_de_fragmentos = 100;
    while(getline(file, sequencia)){
    }
    file.close();
    
    
    
    
    string *lista_de_frags = shotgun(4, 6, qtd_de_fragmentos, sequencia);
    vector<string> vec(lista_de_frags, lista_de_frags + qtd_de_fragmentos);
    sort(vec.begin(), vec.end());
    vec.erase(unique(vec.begin(), vec.end()), vec.end());
    
    ofstream arquivo_escrita("arquivo_do_grafo.txt");
    string linha;
    linha = to_string(qtd_de_fragmentos);
    arquivo_escrita << linha << "\n";
    

    string frag1, frag2;
    int contador_de_linhas = 0;
    for(int i = 0; i < vec.size(); i++){
        for(int j = 0; j < vec.size(); j++){
            if (i == j){
                continue;
            }

            if (checagem(vec[i], vec[j], k)){
                arquivo_escrita << vec[i] <<" ";
                arquivo_escrita << vec[j] << "\n";
                contador_de_linhas++;
            }
        }
    }
    arquivo_escrita.close();

    ifstream arquivo_escrita_aux("arquivo_do_grafo.txt");
    vector<string> linhas_do_arquivo;
    string linha_aux;
    while(getline(arquivo_escrita_aux, linha_aux)){
        linhas_do_arquivo.push_back(linha_aux);
    }

    arquivo_escrita_aux.close();

    linhas_do_arquivo[0] = to_string(qtd_de_fragmentos) + " " + to_string(contador_de_linhas);

    ofstream arquivo_escrita_aux2("arquivo_do_grafo.txt");

    for(auto& linha_aux: linhas_do_arquivo){
        arquivo_escrita_aux2 << linha_aux << endl;
    }
    

    arquivo_escrita_aux2.close();

    digrafo digrafo_1;
    digrafo_1 = produzir_grafo("arquivo_do_grafo.txt");
    //digrafo_1.print_grafo();       print do grafo com ciclos
    //cout << endl << endl << endl;

    vector<vertice*> vertices_visitados;
    for( vertice* v : digrafo_1.vertices_do_grafo){ //eliminio os ciclos do grafo nesse for
        v->elimina_ciclos(v, vertices_visitados);
        vertices_visitados.clear();
    }

    //digrafo_1.print_grafo();   print do grafo sem ciclos
    
    cout << endl;
    vector<vertice*> caminho_max; //declaração de vetores para utiliza-los na função de achar o maior caminho
    vector<vertice*> visitados;
    vector<vertice*> caminho;
    for(vertice* v : digrafo_1.vertices_do_grafo){
        v->acha_maior_caminho(caminho, caminho_max, visitados);
        caminho.clear();
        visitados.clear();
    }

    cout << decodifica_vetor_caminho_max( k,caminho_max) << endl;

    return 0;
}