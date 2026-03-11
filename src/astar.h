#pragma once
#include "heuristics.h"

typedef struct tipoNo {
  pair<int, int> coord;
  pair<int, int> pai;
  int g;
} tipoNo;

struct comparadorTipoNo {
  bool operator()(const pair<int, tipoNo> &a, const pair<int, tipoNo> b) {
    // O desempate do Saunders pelo g
    if (a.first == b.first)
      return a.second.g < b.second.g;
    return a.first > b.first;
  }
};

extern int estadosExpandidos;

// Algoritmo A*
vector<pair<int, int>> aStar(pair<int, int> start, pair<int, int> goal,
                             Heuristica h_func);
