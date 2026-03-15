#include "astar.h"
#include <bits/stdc++.h>

using namespace std;

int main() {
  // Tem que colocar mais mapas depois
  string fileName = "maps/den000d.map";
  string scen = "maps/den000d.map.scen";
  loadMap(fileName);

  vector<int> sementes = {42, 137, 271, 314, 512, 777, 1024, 2048, 3141, 9999};
  vector<int> numPivos = {1, 5, 10, 20, 50, 100, 200};
  vector<int> degradacoes = {100, 500, 1000, 2000, 5000};

  // Tabelas
  ofstream csvBase("resultados_base.csv");
  csvBase << "semente,heuristica,n_pivos,preproc_ms,expansoes_medias,tempo_"
             "medio_ms\n";

  ofstream csvRobustez("resultados_robustez.csv");
  csvRobustez << "semente,n_pivos,n_bloqueios,casos_validos,"
              << "degrad_caminho_manhattan_pct,degrad_caminho_memory_pct,"
                 "degrad_caminho_formula_pct,"
              << "degrad_exp_manhattan_pct,degrad_exp_memory_pct,degrad_exp_"
                 "formula_pct\n";
  ofstream csvRatio("resultados_ratio.csv");
  csvRatio << "semente,ratio_medio\n";

  int startX, startY, goalX, goalY;
  vector<pair<int, int>> caminho;
  pair<int, int> start, goal;
  long long totalExpansoes;
  double ratioSum = 0;
  ifstream file(scen);
  double tempoTotal;
  double distOtima;
  int totalTestes;
  string l;

  for (int semente : sementes) {
    srand(semente);

    vector<int> caminhoOtimo;
    vector<int> expansoesManhattanOrig;
    vector<int> caminhoFormula;
    vector<int> expansoesFormulaOrig;
    vector<int> caminhoMemory;
    vector<int> expansoesMemory;
    map<int, vector<int>> caminhoMemoryMap;
    map<int, vector<int>> expansoesMemoryMap;
    map<int, vector<pair<int, int>>> pivosMap;

    caminhoOtimo.clear();
    expansoesManhattanOrig.clear();
    caminhoFormula.clear();
    expansoesFormulaOrig.clear();
    caminhoMemory.clear();
    expansoesMemory.clear();
    caminhoMemoryMap.clear();
    expansoesMemoryMap.clear();

    tempoTotal = 0;
    totalExpansoes = 0;
    totalTestes = 0;

    file.clear();
    file.seekg(0);
    file >> l >> l;

    // Manhattan
    while (file >> l >> l >> l >> l >> startX >> startY >> goalX >> goalY >>
           distOtima) {
      start = {startY, startX};
      goal = {goalY, goalX};
      estadosExpandidos = 0;
      auto inicioRelogio = chrono::high_resolution_clock::now();
      caminho = aStar(start, goal, heuristicaManhattan);
      auto fimRelogio = chrono::high_resolution_clock::now();
      tempoTotal +=
          chrono::duration<double, milli>(fimRelogio - inicioRelogio).count();
      caminhoOtimo.push_back(caminho.size() - 1);
      expansoesManhattanOrig.push_back(estadosExpandidos);
      totalExpansoes += estadosExpandidos;
      ++totalTestes;
    }
    csvBase << semente << ",manhattan,0,0," << totalExpansoes / totalTestes
            << "," << tempoTotal / totalTestes << "\n";



            //Baseado em fórmula
    double ratioSum = 0;
    tempoTotal = 0;
    totalExpansoes = 0;
    totalTestes = 0;

    file.clear();
    file.seekg(0);
    file >> l >> l;
    while (file >> l >> l >> l >> l >> startX >> startY >> goalX >> goalY >>
           distOtima) {
      start = {startY, startX};
      goal = {goalY, goalX};
      estadosExpandidos = 0;
      auto relogioInicio = chrono::high_resolution_clock::now();
      caminho = aStar(start, goal, heuristicaFormula);
      auto relogioFim = chrono::high_resolution_clock::now();
      tempoTotal +=
          chrono::duration<double, milli>(relogioFim - relogioInicio).count();
      caminhoFormula.push_back(caminho.size() - 1);
      expansoesFormulaOrig.push_back(estadosExpandidos);
      if (caminhoOtimo[totalTestes] > 0)
        ratioSum += (double)(caminho.size() - 1) / caminhoOtimo[totalTestes];
      totalExpansoes += estadosExpandidos;
      ++totalTestes;
    }
    csvBase << semente << ",formula,0,0," << totalExpansoes / totalTestes << ","
            << tempoTotal / totalTestes << "\n";
    csvRatio << semente << "," << ratioSum / totalTestes << "\n";
    cout << "Formula: " << totalExpansoes / totalTestes << " exp | "
         << tempoTotal / totalTestes << " ms | ratio " << ratioSum / totalTestes
         << endl;



         //Baseado em memória
    for (int n : numPivos) {
      caminhoMemory.clear();
      expansoesMemory.clear();

      auto relogioInicio = chrono::high_resolution_clock::now();
      computarDistPivos(n);
      pivosMap[n] = pivos;
      auto relogioFim = chrono::high_resolution_clock::now();
      double tempoPreProc =
          chrono::duration<double, milli>(relogioFim - relogioInicio).count();
      tempoTotal = 0;
      totalExpansoes = 0;
      totalTestes = 0;

      file.clear();
      file.seekg(0);
      file >> l >> l;
      while (file >> l >> l >> l >> l >> startX >> startY >> goalX >> goalY >>
             distOtima) {
        start = {startY, startX};
        goal = {goalY, goalX};
        estadosExpandidos = 0;
        auto relogioInicio = chrono::high_resolution_clock::now();
        caminho = aStar(start, goal, heuristicaMemoryBased);
        auto relogioFim = chrono::high_resolution_clock::now();
        tempoTotal +=
            chrono::duration<double, milli>(relogioFim - relogioInicio).count();
        caminhoMemory.push_back(caminho.size() - 1);
        expansoesMemory.push_back(estadosExpandidos);
        totalExpansoes += estadosExpandidos;
        ++totalTestes;
      }
      caminhoMemoryMap[n] = caminhoMemory;
      expansoesMemoryMap[n] = expansoesMemory;

      csvBase << semente << ",memory," << n << "," << tempoPreProc << ","
              << totalExpansoes / totalTestes << "," << tempoTotal / totalTestes
              << "\n";
      cout << "Memory " << n << " pivôs: " << totalExpansoes / totalTestes
           << " exp | " << tempoTotal / totalTestes << " ms" << endl;
    }


//
//
//
//
//
//
//
    //PArte das degradaç~oes
    //
    vector<vector<bool>> gridOriginal = grid;

    for (int numBloqueios : degradacoes) {
      grid = gridOriginal;
      degradaMapa(numBloqueios);

      for (int n : numPivos) {
        computarDistPivosFixos(pivosMap[n]);

        double degrManh = 0, degMem = 0, degForm = 0, noExpManh = 0, noExpMemoria = 0,
               noExpForm = 0;
        int casoVal = 0;
        totalTestes = 0;

        file.clear();
        file.seekg(0);
        file >> l >> l;
        while (file >> l >> l >> l >> l >> startX >> startY >> goalX >> goalY >>
               distOtima) {
          start = {startY, startX};
          goal = {goalY, goalX};

          if (caminhoOtimo[totalTestes] > 0 &&
              verificaPontosConectados(start, goal)) {
            estadosExpandidos = 0;
            caminho = aStar(start, goal, heuristicaManhattan);
            int tamManh = caminho.size() - 1;
            int expManhattan = estadosExpandidos;

            estadosExpandidos = 0;
            caminho = aStar(start, goal, heuristicaMemoryBased);
            int tamMem = caminho.size() - 1;
            int expMem = estadosExpandidos;

            estadosExpandidos = 0;
            caminho = aStar(start, goal, heuristicaFormula);
            int tamForm = caminho.size() - 1;
            int expForm = estadosExpandidos;

            degrManh +=
                (double)(tamManh - caminhoOtimo[totalTestes]) /
                caminhoOtimo[totalTestes] * 100;
            degMem +=
                (double)(tamMem - caminhoMemoryMap[n][totalTestes]) /
                max(1, caminhoMemoryMap[n][totalTestes]) * 100;
            degForm +=
                (double)(tamForm - caminhoFormula[totalTestes]) /
                max(1, caminhoFormula[totalTestes]) * 100;

            if (expansoesManhattanOrig[totalTestes] > 0)
              noExpManh +=
                  (double)(expManhattan - expansoesManhattanOrig[totalTestes]) /
                  expansoesManhattanOrig[totalTestes] * 100;
            if (expansoesMemoryMap[n][totalTestes] > 0)
              noExpMemoria +=
                  (double)(expMem - expansoesMemoryMap[n][totalTestes]) /
                  expansoesMemoryMap[n][totalTestes] * 100;
            if (expansoesFormulaOrig[totalTestes] > 0)
              noExpForm +=
                  (double)(expForm - expansoesFormulaOrig[totalTestes]) /
                  expansoesFormulaOrig[totalTestes] * 100;

            casoVal++;
          }
          ++totalTestes;
        }

        if (casoVal > 0) {
          csvRobustez << semente << "," << n << "," << numBloqueios << ","
                      << casoVal << "," << degrManh / casoVal
                      << "," << degMem / casoVal << ","
                      << degForm / casoVal << ","
                      << noExpManh / casoVal << ","
                      << noExpMemoria / casoVal << ","
                      << noExpForm / casoVal << "\n";
        }
      }
    }


    
    grid = gridOriginal;
    csvBase.flush();
    csvRobustez.flush();
    csvRatio.flush();
  }
  csvBase.close();
  csvRobustez.close();
  csvRatio.close();
  return 0;
}
