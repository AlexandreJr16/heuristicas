#include "astar.h"
#include <chrono>

const int NUM_TESTES = 1000;

int main() {
  srand(time(0));
  string filename = "maps/den000d.map";
  string testCaseFile = "maps/den000d.map.scen";
  double tempoTotal = 0;
  loadMap(filename);

  ifstream file(testCaseFile);
  string token;
  file >> token >> token;

  pair<int, int> start, goal;
  vector<pair<int, int>> caminho;
  int startX, startY, goalX, goalY;
  vector<int> caminhoOtimo;
  double distOtima;

  // Teste Manhattan
  cout << "=== Manhattan ===" << endl;
  long long totalExpansoes = 0;
  tempoTotal=0;
  int totalTestes = 0;
  file.clear();
  file.seekg(0);
  file >> token >> token;
  while (file >> token >> token >> token >> token >> startX >> startY >>
         goalX >> goalY >> distOtima) {
    start = {startY, startX};
    goal = {goalY, goalX};
    estadosExpandidos = 0;
          auto inicioRelogio = chrono::high_resolution_clock::now();

    caminho = aStar(start, goal, heuristicaManhattan);
       auto fimRelogio = chrono::high_resolution_clock::now(); 
      tempoTotal += chrono::duration<double, milli>(fimRelogio-inicioRelogio).count();
    caminhoOtimo.push_back(caminho.size() - 1);
    totalExpansoes += estadosExpandidos;
    if (++totalTestes >= NUM_TESTES)
      break;
  }
  cout << "Média expansões: " << totalExpansoes / totalTestes << endl;
  cout << "Tempo médio por busca: " << tempoTotal / totalTestes << " ms" << endl;

  // Teste Memory-based
  cout << "\n=== Memory-Based ===" << endl;
  vector<int> numPivos = {1, 5, 10, 20, 50, 100, 200};
  for (int n : numPivos) {
    tempoTotal=0;
    computarDistPivos(n);
    totalExpansoes = 0;
    totalTestes = 0;
    file.clear();
    file.seekg(0);
    file >> token >> token;
    while (file >> token >> token >> token >> token >> startX >> startY >>
           goalX >> goalY >> distOtima) {
      
      start = {startY, startX};
      goal = {goalY, goalX};
      estadosExpandidos = 0;
      auto inicioRelogio = chrono::high_resolution_clock::now();
      aStar(start, goal, heuristicaMemoryBased);
      auto fimRelogio = chrono::high_resolution_clock::now(); 
      tempoTotal += chrono::duration<double, milli>(fimRelogio-inicioRelogio).count();
      totalExpansoes += estadosExpandidos;
      if (++totalTestes >= NUM_TESTES)
        break;
    }
    cout << "Pivôs: " << n
         << " | Média expansões: " << totalExpansoes / totalTestes <<" | Tempo médio: "<<tempoTotal/totalTestes<< endl;
  }

  double mediaRatio;
  double ratioSum = 0;
  tempoTotal= 0;
  // Teste Formula-Based
  cout << "\n=== Formula-Based ===" << endl;
  totalExpansoes = 0;
  totalTestes = 0;
  vector<int> caminhoFormula;
  file.clear();
  file.seekg(0);
  file >> token >> token;
  while (file >> token >> token >> token >> token >> startX >> startY >>
         goalX >> goalY >> distOtima) {
    start = {startY, startX};
    goal = {goalY, goalX};
    estadosExpandidos = 0;
          auto inicioRelogio = chrono::high_resolution_clock::now();

    caminho = aStar(start, goal, heuristicaFormula);
       auto fimRelogio = chrono::high_resolution_clock::now(); 
      tempoTotal += chrono::duration<double, milli>(fimRelogio-inicioRelogio).count();
    caminhoFormula.push_back(caminho.size() - 1);
    if (caminhoOtimo[totalTestes] > 0)
      ratioSum += (double)(caminho.size() - 1) / caminhoOtimo[totalTestes];
    totalExpansoes += estadosExpandidos;
    if (++totalTestes >= NUM_TESTES)
      break;
  }
  mediaRatio = double(ratioSum) / double(totalTestes);
  cout << "Média expansões: " << totalExpansoes / totalTestes << endl;
  cout << "Tempo médio por busca: " << tempoTotal / totalTestes << " ms" << endl;
  cout << "Média de ratio:" << mediaRatio << endl;

  // Guardar comprimentos originais do memory-based com 20 pivôs
  int pivosRobustez = 20;
  computarDistPivos(pivosRobustez);
  vector<int> caminhoMemory;
  totalTestes = 0;
  file.clear();
  file.seekg(0);
  file >> token >> token;
  while (file >> token >> token >> token >> token >> startX >> startY >>
         goalX >> goalY >> distOtima) {
    start = {startY, startX};
    goal = {goalY, goalX};
    caminho = aStar(start, goal, heuristicaMemoryBased);
    caminhoMemory.push_back(caminho.size() - 1);
    if (++totalTestes >= NUM_TESTES)
      break;
  }

  // === Teste de Robustez ===
  cout << "\n=== Robustez ===" << endl;
  vector<vector<bool>> gridOriginal = grid;
  vector<int> degradacoes = {100, 500, 1000, 2000, 5000};

  for (int numBloqueios : degradacoes) {
    grid = gridOriginal;
    degradaMapa(numBloqueios);

    double degradManhattan = 0, degradMemory = 0, degradFormula = 0;
    int casosValidos = 0;
    totalTestes = 0;
    file.clear();
    file.seekg(0);
    file >> token >> token;

    while (file >> token >> token >> token >> token >> startX >> startY >>
           goalX >> goalY >> distOtima) {
      start = {startY, startX};
      goal = {goalY, goalX};

      if (totalTestes < NUM_TESTES && verificaPontosConectados(start, goal) 
          && caminhoOtimo[totalTestes] > 0) {

        caminho = aStar(start, goal, heuristicaManhattan);
        int tamManhattan = caminho.size() - 1;

        caminho = aStar(start, goal, heuristicaMemoryBased);
        int tamMemory = caminho.size() - 1;

        caminho = aStar(start, goal, heuristicaFormula);
        int tamFormula = caminho.size() - 1;

        degradManhattan += (double)(tamManhattan - caminhoOtimo[totalTestes]) / caminhoOtimo[totalTestes] * 100;
        degradMemory += (double)(tamMemory - caminhoMemory[totalTestes]) / caminhoMemory[totalTestes] * 100;
        degradFormula += (double)(tamFormula - caminhoFormula[totalTestes]) / caminhoFormula[totalTestes] * 100;
        casosValidos++;
      }

      if (++totalTestes >= NUM_TESTES)
        break;
    }

    if (casosValidos > 0) {
      cout << "Bloqueios: " << numBloqueios
           << " | Casos válidos: " << casosValidos
           << " | Degrad. Manhattan: " << degradManhattan / casosValidos << "%"
           << " | Degrad. Memory(" << pivosRobustez << "): " << degradMemory / casosValidos << "%"
           << " | Degrad. Formula: " << degradFormula / casosValidos << "%" << endl;
    }
  }

  grid = gridOriginal;

  return 0;
}
