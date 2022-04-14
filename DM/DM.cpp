#include <iostream>
#include <fstream>
#include <vector>
#include <stack>

using namespace std;

int DFS(vector<vector<char>> &graph);
void generate_graph(int v_count);


int main()
{
   generate_graph(10000);

   int V, E;

   ifstream in("info.txt");
   if (!in.is_open())
   {
      cerr << "Не удалось открыть файл info";
      return -1;
   }
   in >> V >> E;


   in = ifstream("in.txt");
   if (!in.is_open())
   {
      cerr << "Не удалось открыть файл in";
      return -1;
   }


   vector<vector<char>> graph(V);
   for (int i = 0; i < V; i++)
      graph[i].resize(V);

   for (int i = 0; i < E; i++)
   {
      int r, c;
      in >> r >> c;

      graph[r][c] = 1;
      graph[c][r] = 1;
   }


   ofstream out = ofstream("out.txt");

   if (E - DFS(graph) == 0)
   {
      if (E == V - 1)
         out << "Дерево." << endl;
      else out << "Не дерево: есть цикл." << endl;
   }
   else out << "Не дерево: компонент связности больше 1." << endl;

   return 0;
}


int DFS(vector<vector<char>> &graph)
{
   int checked_edges = 0;

   stack<int> stack;
   stack.push(0);

   while (!stack.empty())
   {
      bool down = false;
      int r = stack.top();

      for (int c = 0; c < graph.size() && !down; c++)
      {
         if (down = graph[r][c] == 1)
         {
            checked_edges++;

            graph[r][c] = 2;
            graph[c][r] = 2;

            stack.push(c);
         }
      }

      if (!down) stack.pop();
   }

   return checked_edges;
}

void generate_graph(int v_count)
{
   ofstream out("info.txt");
   out << v_count + 1 << ' ' << v_count << endl;


   out = ofstream("in.txt");

   int v_num = 0, dif = 1;
   for (int i = 0; i < v_count; i += 2)
   {
      out << v_num << ' ' << dif++ << endl;
      out << v_num << ' ' << dif++ << endl;

      v_num++;
   }
}