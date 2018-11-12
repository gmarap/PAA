#include <iostream>
#include <list>
#include <algorithm>
#include <bits/stdc++.h>
#include <ctime>
#define INFINITE 999999/0.001

using namespace std;

class Graph {

private:
	int numVertex;
	int **adj;
	bool *visited;
	double best;
	vector<int> bruteForce();
	vector<int> bruteForceRecursive(int atualVertex, double pathValue, vector<int> cities, vector<int> bestPath);
	vector<int> branchAndBound();
	vector<int> branchAndBoundRecursive(int atualVertex, double pathValue, vector<int> cities, vector<int> bestPath);

public:
	time_t timeT;
	int **coor;
	Graph(int numVertex);
	~Graph();
	void display();
	void getBF();
	void getBB();
	void addCoor(int vertex, double x, double y);
	void addEdge(int origin, int destin, int weight);
	int getNumVertex();
	int delEdge(int origin, int destin);
	int getEdge(int origin, int destin);
	bool emptyMat(int v);
	bool isConnected(int origin, int destin);
	bool findValue(int x, vector<int> vector);
};

/**
 * Construtor
 */
Graph::Graph(int numVertex) {

	this->numVertex = numVertex;
	this->best = INFINITE;
  this->timeT = 0;

	visited = new bool [numVertex]; // Criando um array de bool [visitado = true || não visitado = false]

	// Alocando memória para a matriz de adjacência
	adj = new int* [numVertex];

	// Alocando memória para a matriz de coordenadas
	coor = new int* [numVertex];

	for (int i = 0; i < numVertex; i++) {
		adj[i] = new int [numVertex];
		coor[i] = new int [numVertex];
		for(int j = 0; j < numVertex; j++) {
			adj[i][j] = 0; // Inicializando todas as posições com 0
		}
		for(int j = 0; j < 2; j++) {
			coor[i][j] = 0; // Inicializando as duas posições com 0
		}
	}
}

/**
 * Destrutor
 */
Graph::~Graph() {
  for (int i = 0; i < this->numVertex; i++) {
    delete [] this->adj[i];
		delete[] this->coor[i];
  }
  delete [] this->adj;
	delete [] this->coor;
}

/**
 * Retorna o número de vértices do Grafo
 */
int Graph::getNumVertex() {
	return this->numVertex;
}

/**
 * Adiciona uma aresta do vértice "origin" até o "destin", de peso "weight"
 */
void Graph::addEdge(int origin, int destin, int weight) {

	// Verifica se os vértices são válidos
	if( origin > this->numVertex || destin > this->numVertex || origin < 0 || destin < 0) {
		cout << "Invalid edge!\n";
	}
	else {
		adj[origin][destin] = weight; // Adiciona o "destin" como adjacente de "origin"
		adj[destin][origin] = weight; // Adiciona o "origin" como adjacente de "destin"
	}
}

/**
 * Adiciona as coordenadas do vértice dado
 */ 
void Graph::addCoor(int vertex, double x, double y) {
	this->coor[vertex][0] = x;
	this->coor[vertex][1] = y;
}

/**
 * Verifica se dois vértices estão conectados
 */
bool Graph::isConnected(int origin, int destin) {
	return (this->adj[origin][destin] > 0);
}

/**
 * Deleta uma aresta que conecta o vértice "origin" ao "destin" e retorna seu peso ou 0 (caso não exista)
 */
int Graph::delEdge(int origin, int destin) {

	int wei = this->adj[origin][destin];

	// Verifica se a aresta existe
	if(wei > 0) {
		this->adj[origin][destin] = this->adj[destin][origin] = 0; // Seta o valor da aresta como 0 (apaga)
		return wei;
	}
	else {
		return 0;
	}
}

/**
 * Retorna o peso, ou 0 (caso não exista), da aresta que conecta o vértice "origin" ao "destin"
 */
int Graph::getEdge(int origin, int destin) {
	int wei = this->adj[origin][destin];

	// Verifica se a aresta existe
	wei > 0 ? wei : 0;
}

/**
 * Verifica se o vértice é isolado
 */
bool Graph::emptyMat(int v) {
  for (int i = 0; i < this->numVertex; i++) {
    if (adj[v][i] > 0) {
      return false;
    }
  }
  return true;
}

/**
 * Printa o Grafo
 */
void Graph::display() {

	cout << "\n\n            ";
	for(int i = 0; i < this->numVertex; i++) {
		printf("%10d  ", i);
	}

	cout << "\n\n";
	for (int i = 0; i < this->numVertex; i++) {

		printf("%10d  ", i);
		for (int j = 0; j < this->numVertex; j++) {
			printf("%10d  ", this->adj[i][j]);
		}
		cout << endl;
	}
	cout << "\n\n";
}

/**
 * Procura um determinado valor em um vetor e retorna
 * 'true' se encontrar ou 'false', caso contrário.
 */
bool Graph::findValue(int x, vector<int> vec) {
	vector<int>::iterator it;
	it = find(vec.begin(), vec.end(), x);
  return it != vec.end();
}

void Graph::getBF() {

	// Pega o menor caminho do grafo pelo método de força bruta
	vector<int> path = bruteForce();
	double pathTotal = 0;
	string pathStr = "";

	// Soma o valor de cada aresta do melhor caminho
	for(int i = 0; i < path.size(); i++) {
		pathTotal += this->adj[path[i]][path[i+1]];
		pathStr = pathStr + to_string(path[i] + 1) + " ";
	}
	cout << pathTotal << endl << pathStr << endl;
}

/**
 * Método de inicialização do 'bruteForceRecursive'.
 * Retorna o menor caminho do grafo pelo método de força bruta
 * e calcula o tempo de realização do mesmo.
 */
vector<int> Graph::bruteForce() {
	time_t begin, end;
	vector<int> cities, path;
	
	cities.push_back(0);
	this->best = INFINITE;

	begin = clock();
	path = bruteForceRecursive(0, 0, cities, path);
	end = clock();
	this->timeT = end - begin;
	return path;
}

/**
 * Determina o menor caminho Hamiltoniano de um grafo utilizando
 * o paradigma de força bruta.
 * 
 * Método criado por Henrique Schiess Pertussati e 
 * editado por Gabriella Mara Pereira
 * 
 * @param int atualVertex - Vértice que está sendo visitado
 * @param double pathValue - Caminho total até o momento
 * @param vector<int> cities - Vetor de inteiros com o caminho atual percorrido
 * @param vector<int> bestPath - Vetor de inteiro que possui a ordem que os 
 * 	vértices devem ser visitados para obter o menor caminho
 */
vector<int> Graph::bruteForceRecursive(int atualVertex, double pathValue, vector<int> cities, vector<int> bestPath) {
	for(int i = 0; i < this->numVertex; i++) {
		// Se o vértice já não tiver sido visitado
		if(!findValue(i, cities)) {

			// Adiciona a distância do vértice anterior até o atual no total
			pathValue += this->adj[atualVertex][i];

			// Coloca o vértice 'i' no vetor de vértices já visitados
			cities.push_back(i);

			// Atualiza os valores para a pesquisa do menor caminho a partir do novo vértice 
			bestPath = bruteForceRecursive(i, pathValue, cities, bestPath);

			/**
			 * Se todos os vértices foram visitados e se 
			 * [(caminho total até agora) + (aresta do vértice atual até o inicial)]
			 * for menor do que o melhor caminho gerado até agora, o novo melhor 
			 * caminho é esse (cities).
			 */
			double pathNow = pathValue + adj[0][i];
			if(cities.size() == this->numVertex && pathNow < this->best) {
				this->best = pathNow;
				bestPath = cities;
			}
			pathValue -= this->adj[atualVertex][i];
			cities.pop_back();
		}
	}
	return bestPath;
}

void Graph::getBB() {

	// Pega o menor caminho do grafo pelo método de branch and bound
	vector<int> path = branchAndBound();
	double pathTotal = 0;
	string pathStr = "";

	// Soma o valor de cada aresta do melhor caminho
	for(int i = 0; i < path.size(); i++) {
		pathTotal += this->adj[path[i]][path[i+1]];
		pathStr = pathStr + to_string(path[i] + 1) + " ";
	}
	cout << pathTotal << endl << pathStr << endl;
}

/**
 * Método de inicialização do 'branchAndBoundRecursive'.
 * Retorna o menor caminho do grafo pelo método de branch and bound
 * e calcula o tempo de realização do mesmo.
 */
vector<int> Graph::branchAndBound() {
	time_t begin, end;
	vector<int> cities, path;
	
	cities.push_back(0);
	this->best = INFINITE;

	begin = clock();
	path = branchAndBoundRecursive(0, 0, cities, path);
	end = clock();
	this->timeT = end - begin;
	return path;
}

/**
 * Determina o menor caminho Hamiltoniano de um grafo utilizando
 * o paradigma de branch and bound.
 * 
 * Método criado por Henrique Schiess Pertussati e 
 * editado por Gabriella Mara Pereira
 * 
 * @param int atualVertex - Vértice que está sendo visitado
 * @param double pathValue - Caminho total até o momento
 * @param vector<int> cities - Vetor de inteiros com o caminho atual percorrido
 * @param vector<int> bestPath - Vetor de inteiro que possui a ordem que os 
 * 	vértices devem ser visitados para obter o menor caminho
 */
vector<int> Graph::branchAndBoundRecursive(int atualVertex, double pathValue, vector<int> cities, vector<int> bestPath) {
	for(int i = 0; i < this->numVertex; i++) {
		// Se o vértice já não tiver sido visitado
		if(!findValue(i, cities)) {

			// Adiciona a distância do vértice anterior até o atual no total
			pathValue += this->adj[atualVertex][i];

			// Coloca o vértice 'i' no vetor de vértices já visitados
			cities.push_back(i);

			// Só continua se o caminho atual for menor do que o menor encontrado até agora
			if(pathValue < this->best) {
				// Atualiza os valores para a pesquisa do menor caminho a partir do novo vértice 
				bestPath = branchAndBoundRecursive(i, pathValue, cities, bestPath);

				/**
				 * Se todos os vértices foram visitados e se 
				 * [(caminho total até agora) + (aresta do vértice atual até o inicial)]
				 * for menor do que o melhor caminho gerado até agora, o novo melhor 
				 * caminho é esse (cities).
				 */
				double pathNow = pathValue + adj[0][i];
				if(cities.size() == this->numVertex && pathNow < this->best) {
					this->best = pathNow;
					bestPath = cities;
				}
			}

			pathValue -= this->adj[atualVertex][i];
			cities.pop_back();
		}
	}
	return bestPath;
}

/*
int main() {

	Graph g(6);
	g.addEdge(1, 0, 8);
	g.addEdge(2, 1, 7);
	g.addEdge(3, 1, 9);
	g.addEdge(4, 0, 3);
	g.addEdge(5, 4, 2);

	cout << "MATRIZ DE ADJACÊNCIA:\n";
	g.display();

	return 0;
}
*/
