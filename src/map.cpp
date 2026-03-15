#include "map.h"

int height, width;
vector<vector<bool>> grid;

// Coordenadas para buscar vizinhos
int px[4] = {0, 0, 1, -1};
int py[4] = {-1, 1, 0, 0};

bool verificaPontosConectados(pair<int, int> start, pair<int,int> goal){
  vector<bool> visitado(width * height, false);
  queue<pair<int, int>> q;

   int actualX, actualY;

  visitado[start.first * width + start.second] = true;
  q.push(start);

  while(!q.empty()){
    pair<int,int> v = q.front();
    q.pop();

    for(int i=0; i<4; i++){
      actualX = v.first + px[i];
      actualY = v.second + py[i];
      int locVizinho = actualX * width + actualY;

      if(actualX >= 0 && actualX < height && actualY >= 0 && actualY < width &&
          grid[actualX][actualY] && !visitado[locVizinho]) {
        visitado[locVizinho] = true;
        q.push({actualX, actualY});
      }
    }
  }


  return visitado[goal.first * width+ goal.second];
}

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
  int cont =0;

  // le o mapa
  for (int i = 0; i < height; i++) {
    getline(file, line);
    for (int j = 0; j < width; j++) {
      grid[i][j] = (line[j] == '.');
      cont++;
    }
  }
  cout << cont << endl;
}

void degradaMapa(int n){
  int i=0;
  while(i<n){
    int x = rand()%height;
    int y = rand()%width;

    if(grid[x][y]){
      grid[x][y] = false;
      i++;
    }
  }
}


