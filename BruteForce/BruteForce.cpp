#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <iomanip>
#include <ctime>
#include <random>
#include "../Graph.cpp"

using namespace std;

/**
 * Calcula a distância euclidiana entra duas coordenadas. 
 */
double euclideanDistance(int x1, int y1, int x2, int y2) {
  return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

Graph makeGraph() {
  int nCities;  // Quantidade de cidades
  double x, y;  // Coordenadas

  // Lê a quantidade de cidades
  cin >> nCities;

  // Cria o grafo
  Graph graph(nCities);

  // Lê as coordenadas de cada cidade
  for(int i = 0; i < nCities; i++) {
    cin >> x;
    cin >> y;

    // Adiciona as coordenadas (x,y) do vértice
    graph.addCoor(i, x, y);
  }

  /**
   * Percorre as 'nCities' cidades, adicionando uma aresta em todos 
   * os pares com o valor da distância euclidiana entre as duas 
   * cidades em questão.
   */ 
  for(int i = 0; i < nCities; i++) {
    for(int j = 0; j < nCities; j++) {
      if(i != j) {
        int x1 = graph.coor[i][0];
        int y1 = graph.coor[i][1];
        int x2 = graph.coor[j][0];
        int y2 = graph.coor[j][1];
        graph.addEdge(i, j, euclideanDistance(x1, y1, x2, y2));
      }
    }
  }
  return graph;
}

int main() {
  double path;
  Graph graph = makeGraph();
  graph.getBF();
}