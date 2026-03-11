#pragma once
#include <bits/stdc++.h>

using namespace std;

extern int height, width;
extern vector<vector<bool>> grid;

// Coordenadas para buscar vizinhos
extern int px[4];
extern int py[4];

// Carrega o mapa
void loadMap(string &filename);
