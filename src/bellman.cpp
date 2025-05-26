#include "bellman.h"

// You can add some helper function here

void BF(int edge[][3], int numberOfEdges, char startVertex, int BellmanFordValue[], int BellmanFordPrevious[])
{
    const int maxV = 26;
    for (int i = 0; i < maxV; i++)
    {
        BellmanFordValue[i] = INT_MAX;
        BellmanFordPrevious[i] = -1;
    }

    int start = startVertex - 'A';
    BellmanFordValue[start] = 0;

    for (int i = 0; i < maxV - 1; i++)
    {
        bool updated = false;
        for (int j = 0; j < numberOfEdges; j++)
        {
            int u = edge[j][0] - 'A';
            int v = edge[j][1] - 'A';
            int w = edge[j][2];

            if (BellmanFordValue[u] != INT_MAX && BellmanFordValue[u] + w < BellmanFordValue[v])
            {
                BellmanFordValue[v] = BellmanFordValue[u] + w;
                BellmanFordPrevious[v] = u;
                updated = true;
            }
        }
        if (!updated) break; // thoát sớm nếu không còn cập nhật
    }
}

string BF_Path(int edge[][3], int numberOfEdges, char startVertex, char goalVertex)
{
    int value[26], prev[26];
    BF(edge, numberOfEdges, startVertex, value, prev);
    int goal = goalVertex - 'A';
    if (value[goal] == INT_MAX)
        return "No path";
    char path[26];
    int pathLength = 0;
    // Lấy đường đi theo prev, lưu vào path (ngược)
    for (int v = goal; v != -1; v = prev[v])
    {
        path[pathLength] = 'A' + v;
        pathLength++;
    }
    // Đảo ngược đường đi (bởi vì hiện tại path lưu ngược từ goal về start)
    string result = "";
    for (int i = pathLength - 1; i >= 0; i--)
    {
        result += path[i];
        if (i != 0)
            result += " ";
    }
    return result;
}
