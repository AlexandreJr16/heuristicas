#include "map.h"

int height, width;
vector<vector<bool>> grid;

// Coordenadas para buscar vizinhos
int px[4] = {0, 0, 1, -1};
int py[4] = {-1, 1, 0, 0};

// Carrega o mapa
void loadMap(string &filename) {
  ifstream file(filename);
  string token;

  // Faz a leituras das informaç~ões do mapa
  file >> token >> token;
  file >> token >> height;
  file >> token >> width;
  file >> token;

  grid.resize(height, vector<bool>(width));
  string line;
  getline(file, line);

  // le o mapa
  for (int i = 0; i < height; i++) {
    getline(file, line);
    for (int j = 0; j < width; j++) {
      grid[i][j] = (line[j] == '.');
    }
  }
}
