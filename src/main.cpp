#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <stack>
#include <string>
#include <iomanip>

using namespace std;

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
                if (matrix[v][i] >= 1)
                {
                    if (dist[i] == -1 || dist[i] > dist[v] + matrix[v][i])
                    {
                        dist[i] = dist[v] + matrix[v][i];
                        st.push(i);
                    }
                }
            }
        }

        cout << "DFS (матрица смежности):\n";
        if (show)
            showDist(dist);
    }
    void DFS_true(int start, bool show)
    {
        const int INF = 1e9;

        std::vector<int> dist(vertexCount, INF);
        dist[start] = 0;

        std::stack<int> st;
        st.push(start);

        while (!st.empty())
        {
            int v = st.top();
            st.pop();

            for (int i = 0; i < vertexCount; i++)
            {
                if (matrix[v][i] >= 1)
                {
                    // можно 
                    if (dist[v] + matrix[v][i] < dist[i])
                    {
                        dist[i] = dist[v] + matrix[v][i];
                        st.push(i);
                    }
                }
            }
        }
        std::cout << "DFS (поиск кратчайших расстояний):\n";
        if (show)
            showDist(dist);
    }
    void BFS(int start, bool show)
    {
        const int INF = 1e9;
        vector<int> dist(vertexCount, INF);
        queue<int> q;

        dist[start] = 0;
        q.push(start);

        while (!q.empty())
        {
            int v = q.front();
            q.pop();

            for (int i = 0; i < vertexCount; i++)
            {
                if (matrix[v][i] <= 0) continue;

                int w = matrix[v][i];

                // разрешаю улучшение
                if (dist[v] + w < dist[i])
                {
                    dist[i] = dist[v] + w;
                    q.push(i);
                }
            }
        }

        cout << "BFS (матрица смежности):\n";
        if (show) showDist(dist);
    }
    void DFS_adj(int start, bool show)
    {
        const int INF = 1e9;
        resetVisited();

        vector<int> dist(vertexCount, INF);
        dist[start] = 0;

        stack<int> st;
        st.push(start);

        while (!st.empty())
        {
            int v = st.top();
            st.pop();

            visited[v] = true;

            for (int to : adjList[v])
            {
                int w = matrix[v][to]; // ВЕС берем из матрицы!

                if (dist[v] + w < dist[to])
                {
                    dist[to] = dist[v] + w;
                    st.push(to);
                }
            }
        }

        cout << "DFS (список смежности):\n";
        if (show)
            showDist(dist);
    }
    void BFS_adj(int start, bool show)
    {
        const int INF = 1e9;
        vector<int> dist(vertexCount, INF);

        queue<int> q;
        dist[start] = 0;
        q.push(start);

        while (!q.empty())
        {
            int v = q.front();
            q.pop();

            for (int to : adjList[v])
            {
                int w = matrix[v][to]; // ВЕС из матрицы

                if (dist[v] + w < dist[to])
                {
                    dist[to] = dist[v] + w;
                    q.push(to);
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
    void showDist(const vector<int>& dist) const
    {
        for (int i = 0; i < vertexCount; i++)
            cout << "DIST[" << i << "] = "
            << (dist[i] == -1 || dist[i] > 1000 ? "нет связи" : to_string(dist[i])) << "\n";
        cout << "\n";
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

    Graph gp;

    int vertices = 0, start = 0;
    double probability = 0.4;
    bool oriented = false;
    bool weighted = true; // по умолчанию взвешенный

    for (int i = 1; i < argc; i++)
    {
        string arg = argv[i];
        if (arg == "oriented") oriented = true;
        if (arg == "unweighted") weighted = false;
    }

    cout << "Введите количество вершин: ";
    cin >> vertices;
    cout << "Вероятность связи: 0.4 \n\n";

        gp.genAdjMatrix(vertices, probability, oriented, weighted);
        gp.print();

    goto_start:
    cout << "Введите вершину для старта:";
    cin >> start;
    if (start > vertices || start < 0) goto goto_start;

        gp.DFS(start, true);
        gp.DFS_true(start, true);
        gp.BFS(start, true);
        gp.DFS_adj(start, true);
        gp.BFS_adj(start, true);
        gp.calcRadiusAndDiameter();

    cout << "\nТип графа: "
        << (oriented ? "ориентированный" : "неориентированный") << ", "
        << (weighted ? "взвешенный" : "невзвешенный") << endl;

    system("pause");
    return 0;
}