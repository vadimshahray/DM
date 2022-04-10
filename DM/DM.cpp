﻿#include <fstream>  // объект для потока ввода
#include <iostream> // поток ввода/вывода
#include <vector>
#include <stack>

using namespace std;

const string IN_PATH  = "in.txt",
             OUT_PATH = "out.txt",
             INF_PATH = "info.txt";

const char REL  = 1,  // REL (relation) - связь
           CREL = 2;  // CREL (checked relation) - проверенная связь


int main()
{
   int N, V; // N - кол-во ребер, V - кол-во вершин
   vector<vector<char>> graph;

   ifstream stream(INF_PATH);

   if (!stream.is_open()) // Проверка открытия файла
   {
      cerr << "Не удалось открыть файл " << INF_PATH;
      return -1;
   }
   stream >> N >> V;
   stream.close();

   stream = ifstream(IN_PATH);
   if (!stream.is_open())
   {
      cerr << "Не удалось открыть файл " << IN_PATH;
      return -1;
   }

   graph.resize(V);
   for (int i = 0; i < V; i++)
      graph[i].resize(V);

   for (int r, c, i = 0; i < N; i++)
   {
       stream >> r >> c;

       graph[r][c] = REL;
       graph[c][r] = REL;
   }

   // В этом стеке будем хранить номера строк матрицы графа [Номер строки также обозначает номер вершины графа, таким образом мы сохраняем номер строки в стеке до тех пор, пока не пройдем по всем ее поддеревьям]. Когда мы спускаемся, вершина добавляется в стек, когда поднимаемся - вершина удаляется из стека, потому что больше мы к ней не собираемся возвращаться.
   stack<int> stack;
   
   /*
   *
   *
      # ИДЕЯ! Есть ситуация, когда K первых вершин (0 до К) не используются. По алгоритму, они будут проверятся, хотя логично, раз в этих строках нет '1', значит и проверять их смысла не имеет. 
      Как это решить: можно в цикле, в котором происходит считывание вершин из файла, запоминать самую минимальную (а то и просто вести список используемых вершин). ВРОДЕ КАК ЭТО СОКРАТИТ КОЛ-ВО НЕНУЖНЫХ ИТЕРАЦИЙ.
   *
   *
   */

   // Добавляем первую строку в стек сразу, поскольку в непустом файле мы всегда начниаем с 0-ой строки (даже если у нас не будет задана вершина '0', мы не получим ошибку, потому что в конце концев перейдем к нужной строке).
   stack.push(0);

   // Проход будет идти, пока стек не станет пуст (это будет означать, что мы перебрали все строки матрицы, а значит и вершины).
   // TODO проверить, верно ли все работает с графом, у которого нет допустим 0, 1 вершины, то есть некоторые первые строки матрицы не будут использоваться.
   while (!stack.empty())
   {
      // c (column) - номер столбца (тоже самое, что и вершина), у которого есть связь с r, при этом по вычисленному c мы еще не спускались;
      // r (row) - номер строки, у которой мы ищем не пройденные ранее вершины (c).
      int c = 0, r = stack.top();
      // Погружаемся ли дальше в глубину, то есть спускаемся ли по поддереву вниз.
      bool dive = false;

      // <FIND_SUBTREE> Ищем есть ли на текущей (r) строке связь с какой-нибудь вершиной (c), которую мы еще не проверили.
      while (c < V && !dive)
         dive = graph[r][c++] == REL;
      c--; // TODO ПОПЫТАТЬСЯ УЛУЧШИТЬ ЗАПИСЬ: после отработки цикла 'c' лишний раз увеличивается из-за 'c++', потому его нужно уменьшить.

      // Смотрим, можно ли нырнуть глубже от текущей вершины.
      if (dive)
      {
         // Если можно, то помечаем текущую вершину как "уже пройденную" в матрице, задав отношение и в строку и в столбец в виде '2'.
         graph[r][c] = graph[c][r] = CREL;
         //cout << r << " " << c << endl;
         // Добавляем данную вершину в стек, чтобы когда, мы поднимались вверх, смогли проверить остальные поддеревья на этой строке (<FIND_SUBTREE>). Также помечаем [r = c], что теперь мы будем просматривать (<FIND_SUBTREE>) ту строку, для которой только что нашли отношение ('c' по сути является и номером строки, если транспонировать матрицу графа).
         stack.push(r = c);
      }
      // Иначе, получается, что спускаться от данной вершины больше некуда. Остается только возвращаться наверх, чтобы обработать остальные узлы.
      else stack.pop();
   }

   bool one_component = true;
   for (int r = 0; r < graph.size(); r++)
   {
      for (int c = 0; c < graph[r].size(); c++)
         cout << (int)graph[r][c] << "  ";
      cout << endl;
   }

   for (int r = 0; r < graph.size() && one_component; r++)
   {
      for (int c = 0; c < graph[r].size() && one_component; c++)
         one_component = graph[r][c] != 1;
   }

   if (one_component)
   {
      if (N == V - 1)
         cout << "Vse ZBS" << endl;
      else cout << "Have loop" << endl;
   }
   else cout << "Greater than 1 relation component" << endl;

   return 0;
}