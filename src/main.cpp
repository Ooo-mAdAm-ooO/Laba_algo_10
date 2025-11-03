#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
#include <random>
#include <stack>
#include <string>
#include <iomanip>

using namespace std;
using namespace std::chrono;

class Graph
{
public:
    Graph() : vertexCount(0), edgeCount(0) {}

    void genAdjMatrix(int vCount, double probability = 0.4, bool isOriented = false, bool isWeighted = true)
    {
        matrix.assign(vCount, vector<int>(vCount, 0));
        adjList.assign(vCount, {});
        visited.assign(vCount, false);
        edgeCount = 0;
        vertexCount = vCount;

        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dist(0.0, 1.0);
        uniform_int_distribution<int> dist_int(1, 5);

        for (int i = 0; i < vertexCount; i++)
        {
            for (int j = (isOriented ? 0 : i + 1); j < vertexCount; j++)
            {
                if (i == j) continue;
                if (dist(gen) < probability)
                {
                    int w = isWeighted ? dist_int(gen) : 1;

                    matrix[i][j] = w;
                    if (!isOriented)
                        matrix[j][i] = w;

                    ++edgeCount;
                }
            }
        }
        genAdjList();
    }


    void DFS(int start, bool show)
    {
        resetVisited();
        stack<int> st;
        st.push(start);

        vector<int> dist(vertexCount, -1);
        dist[start] = 0;

        while (!st.empty())
        {
            int v = st.top();
            st.pop();

            visited[v] = true;

            for (int i = 0; i < vertexCount; i++)
            {
                if (matrix[v][i] == 1)
                {
                    if (dist[i] == -1 || dist[i] > dist[v] + 1)
                    {
                        dist[i] = dist[v] + 1;
                        st.push(i);
                    }
                }
            }
        }

        cout << "DFS (матрица смежности):\n";
        if (show)
            showDist(dist);
    }

    void BFS(int start, bool show)
    {
        vector<int> dist(vertexCount, -1);
        queue<int> q;
        dist[start] = 0;
        q.push(start);

        while (!q.empty())
        {
            int v = q.front();
            q.pop();

            for (int i = 0; i < vertexCount; i++)
            {
                if (matrix[v][i] == 1 && dist[i] == -1)
                {
                    dist[i] = dist[v] + 1;
                    q.push(i);
                }
            }
        }

        cout << "BFS (матрица смежности):\n";
        if (show)
            showDist(dist);
    }

    void DFS_adj(int start, bool show)
    {
        resetVisited();
        stack<int> st;
        st.push(start);

        vector<int> dist(vertexCount, -1);
        dist[start] = 0;

        while (!st.empty())
        {
            int v = st.top();
            st.pop();

            visited[v] = true;

            for (int nb : adjList[v])
            {
                if (dist[nb] == -1 || dist[nb] > dist[v] + 1)
                {
                    dist[nb] = dist[v] + 1;
                    st.push(nb);
                }
            }
        }

        cout << "DFS (список смежности):\n";
        if (show)
            showDist(dist);
    }

    void BFS_adj(int start, bool show)
    {
        vector<int> dist(vertexCount, -1);
        queue<int> q;
        dist[start] = 0;
        q.push(start);

        while (!q.empty())
        {
            int v = q.front();
            q.pop();

            for (int nb : adjList[v])
            {
                if (dist[nb] == -1)
                {
                    dist[nb] = dist[v] + 1;
                    q.push(nb);
                }
            }
        }

        cout << "BFS (список смежности):\n";
        if (show)
            showDist(dist);
    }

    // заданиe 2

    // Возвращает вектор расстояний от одной вершины до всех остальных
    vector<int> getDistancesFrom(int start)
    {
        vector<int> dist(vertexCount, 9999);
        queue<int> q;
        dist[start] = 0;
        q.push(start);

        while (!q.empty())
        {
            int v = q.front();
            q.pop();

            for (int nb : adjList[v])
            {
                if (dist[nb] == 9999)
                {
                    dist[nb] = dist[v] + 1;
                    q.push(nb);
                }
            }
        }
        return dist;
    }

    // Эксцентриситет вершины
    int getEccentricity(int v)
    {
        vector<int> dist = getDistancesFrom(v);
        int ecc = 0;
        for (int d : dist)
            if (d != 9999 && d > ecc)
                ecc = d;
        return ecc;
    }

    // Радиус, диаметр, центральные и периферийные вершины
    void calcRadiusAndDiameter()
    {
        int radius = 9999;
        int diameter = 0;
        vector<int> ecc(vertexCount);

        for (int v = 0; v < vertexCount; v++)
        {
            ecc[v] = getEccentricity(v);
            if (ecc[v] < radius) radius = ecc[v];
            if (ecc[v] > diameter) diameter = ecc[v];
        }

        cout << "\n=== Радиус и диаметр графа ===\n";
        cout << "Радиус графа: " << radius << "\n";
        cout << "Диаметр графа: " << diameter << "\n";

        cout << "\nЦентральные вершины: ";
        for (int i = 0; i < vertexCount; i++)
            if (ecc[i] == radius)
                cout << i << " ";

        cout << "\nПериферийные вершины: ";
        for (int i = 0; i < vertexCount; i++)
            if (ecc[i] == diameter)
                cout << i << " ";
        cout << "\n";
    }

    void print() const
    {
        cout << "\n=== Матрица смежности ===\n   ";
        for (int j = 0; j < vertexCount; j++)
            cout << setw(2) << j << " ";
        cout << "\n";

        for (int i = 0; i < vertexCount; i++)
        {
            cout << setw(2) << i << " ";
            for (int j = 0; j < vertexCount; j++)
                cout << setw(2) << matrix[i][j] << " ";
            cout << "\n";
        }

        cout << "\n=== Список смежности ===\n";
        for (int i = 0; i < vertexCount; i++)
        {
            cout << setw(2) << i << ": ";
            if (adjList[i].empty())
                cout << "пусто";
            else
                for (int j : adjList[i])
                    cout << j << "(" << matrix[i][j] << ") ";
            cout << "\n";
        }
        cout << "Вершин: " << vertexCount << ", Рёбер: " << edgeCount << "\n\n";
    }

private:
    void resetVisited()
    {
        fill(visited.begin(), visited.end(), false);
    }

    void genAdjList()
    {
        for (int i = 0; i < vertexCount; i++)
        {
            adjList[i].clear();
            for (int j = 0; j < vertexCount; j++)
            {
                if (matrix[i][j])
                    adjList[i].push_back(j);
            }
        }
    }

    void showDist(const vector<int>& dist)
    {
        for (int i = 0; i < vertexCount; i++)
            cout << "DIST[" << i << "] = "
            << (dist[i] == -1 ? "нет связи" : to_string(dist[i])) << "\n";
    }

    vector<vector<int>> matrix;
    vector<vector<int>> adjList;
    vector<bool> visited;
    int vertexCount;
    int edgeCount;
};

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "RUSSIAN");

    bool oriented = false;
    bool weighted = true; // по умолчанию взвешенный

    for (int i = 1; i < argc; i++)
    {
        string arg = argv[i];
        if (arg == "oriented") oriented = true;
        if (arg == "unweighted") weighted = false;
    }
    Graph gp;
    int vertices = 0;
    double probability = 0.0;

    cout << "Введите количество вершин: ";
    cin >> vertices;
    cout << "Введите вероятность связи (0.1 - 0.9): ";
    cin >> probability;

    gp.genAdjMatrix(vertices, probability, oriented, weighted);
    gp.print();

    gp.calcRadiusAndDiameter();

    cout << "\nТип графа: "
        << (oriented ? "ориентированный" : "неориентированный") << ", "
        << (weighted ? "взвешенный" : "невзвешенный") << endl;

    return 0;
}

