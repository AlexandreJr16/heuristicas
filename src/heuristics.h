#pragma once
#include "map.h"

typedef int (*Heuristica)(pair<int, int>, pair<int, int>);

// Utilizo isso para memory-based
vector<pair<int, int>> pivos;
vector<vector<vector<int>>> distPivos;

// Gerar números aleatórios
int randomNumbers(int num) { return rand() % num; }

// Gera pivôs aleatórios
vector<pair<int, int>> generatePivots(int n) {

  vector<pair<int, int>> pivotList;
  pair<int, int> pivot;
  int cont = 0;

  while (cont < n) {

    // Talvez isso estea invertido, tenho que verificar certinho
    pivot.first = randomNumbers(height);
    pivot.second = randomNumbers(width);
    if (grid[pivot.first][pivot.second]) {
      pivotList.push_back(pivot);
      cont++;
    }
  }

  return pivotList;
}

// BFS dos pivos até todos os pontos
vector<vector<int>> bfsPivo(pair<int, int> pivo) {
  vector<vector<int>> dists(height, vector<int>(width, -1));
  vector<bool> visitado(width * height, false);
  queue<pair<int, int>> q;
  int actualX, actualY;

  visitado[pivo.first * width + pivo.second] = true;
  q.push(pivo);
  dists[pivo.first][pivo.second] = 0;

  while (!q.empty()) {
    pair<int, int> v = q.front();
    // isso soa como k-pop
    q.pop();

    for (int i = 0; i < 4; i++) {
      actualX = v.first + px[i];
      actualY = v.second + py[i];
      int locVizinho = actualX * width + actualY;
      if (actualX >= 0 && actualX < height && actualY >= 0 && actualY < width &&
          grid[actualX][actualY] && !visitado[locVizinho]) {
        visitado[locVizinho] = true;
        dists[actualX][actualY] = dists[v.first][v.second] + 1;
        q.push({actualX, actualY});
      }
    }
  }
  return dists;
}

// Pre-computa a distancia de todos os pontos a todos os pivôs
void computarDistPivos(int n) {
  pivos = generatePivots(n);
  distPivos.clear();
  for (auto &pivo : pivos) {
    distPivos.push_back(bfsPivo(pivo));
  }
}

// Heuristica baseada em memória
int heuristicaMemoryBased(pair<int, int> s, pair<int, int> goal) {
  int dist = 0;
  for (unsigned i = 0; i < pivos.size(); i++) {
    dist = max(abs(distPivos[i][s.first][s.second] -
                   distPivos[i][goal.first][goal.second]),
               dist);
  }
  return dist;
}

// Heurística que será aplicada
int heuristicaManhattan(pair<int, int> start, pair<int, int> goal) {

  // Manhattan
  return abs(start.first - goal.first) + abs(start.second - goal.second);
}

// heuristica de memória para o den000d
int heuristicaFormula(pair<int, int> s, pair<int, int> goal) {
  int dx = abs(s.first - goal.first);
  int dy = abs(s.second - goal.second);
  return 75 * max(dx, dy);
}
