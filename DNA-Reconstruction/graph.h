#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

class vertice {
    private:

        void apaga_aresta(vertice *receptor);

    public:

        string fragmento_de_DNA;
        vector<vertice*> vizinhos;
        vertice(string fragmento_a_inserir);
        void elimina_ciclos(vertice* vertice_original, vector<vertice*> vertices_visitados);
        void acha_maior_caminho(vector<vertice*>& caminho, vector<vertice*>& caminho_MAX, vector<vertice*>& visitados);

};

class digrafo {
    public:

        vector<vertice*> vertices_do_grafo;
        digrafo();
        void adiciona_vertice(string fragmento);
        void print_grafo();
        void adiciona_arco(string fragmento_origem, string fragmento_destino);

};


string decodifica_vetor_caminho_max(int k, vector <vertice*> caminho_max);
bool checagem(string fragmento1, string fragmento2, int k);
string *shotgun(int min_fragmento, int max_fragmento, int qtd_fragmento, string DNA);
digrafo produzir_grafo(string nome_do_arquivo);
bool todos_pertencem(vector<vertice*>& vetor1, vector<vertice*>& vetor2);
