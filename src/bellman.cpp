#include "bellman.h"
#include <vector>
#include <string>
#include <climits>
#include <algorithm>
using namespace std;

struct MyEdge {
    int from;
    int to;
    int cost;
};

int getIndex(char c, char vertices[], int size) {
    for (int i = 0; i < size; ++i) {
        if (vertices[i] == c) return i;
    }
    return -1;
}

void createVertexMapping(int edge[][3], int numberOfEdges, char vertices[], int& numVertices, vector<MyEdge>& edgeList) {
    bool marked[256] = { false };
    numVertices = 0;

    // đầu tiên tìm tất cả các đỉnh duy nhất
    for (int i = 0; i < numberOfEdges; ++i) {
        char u = edge[i][0];
        char v = edge[i][1];

        if (!marked[(int)u]) {
            vertices[numVertices++] = u;
            marked[(int)u] = true;
        }
        if (!marked[(int)v]) {
            vertices[numVertices++] = v;
            marked[(int)v] = true;
        }
    }

    // Tạo danh sách cạnh có chỉ số
    edgeList.clear();
    for (int i = 0; i < numberOfEdges; ++i) {
        int uIdx = getIndex(edge[i][0], vertices, numVertices);
        int vIdx = getIndex(edge[i][1], vertices, numVertices);
        edgeList.push_back({uIdx, vIdx, edge[i][2]});
    }
}

void BF(int edge[][3], int numberOfEdges, char startVertex, int BellmanFordValue[], int BellmanFordPrevious[]) {
    char vertices[256];
    int numVertices = 0;
    vector<MyEdge> edgeList;
    
    createVertexMapping(edge, numberOfEdges, vertices, numVertices, edgeList);

    int startIndex = getIndex(startVertex, vertices, numVertices);
    if (startIndex == -1) return;

    // Khởi tạo với các giá trị hiện tại, không đặt lại nếu đã tồn tại
    bool initialized = false;
    for (int i = 0; i < numVertices; ++i) {
        if (BellmanFordValue[i] == -1) {
            BellmanFordValue[i] = INT_MAX;
            BellmanFordPrevious[i] = -1;
            initialized = true;
        }
    }
    
    // Chỉ đặt đỉnh bắt đầu nếu chúng ta đang khởi tạo mới
    if (initialized) {
        BellmanFordValue[startIndex] = 0;
    }

    // Tạo bản sao tạm thời của giá trị hiện tại (dùng để relax)
    vector<int> tempValues(BellmanFordValue, BellmanFordValue + numVertices);

    // duyệt tất cả các cạnh một lần (sử dụng giá trị từ bản sao tạm)
    for (int j = 0; j < edgeList.size(); ++j) {
        int u = edgeList[j].from;
        int v = edgeList[j].to;
        int w = edgeList[j].cost;

        if (tempValues[u] != INT_MAX && tempValues[u] + w < BellmanFordValue[v]) {
            BellmanFordValue[v] = tempValues[u] + w;
            BellmanFordPrevious[v] = u;
        }
    }
}

string BF_Path(int edge[][3], int numberOfEdges, char startVertex, char goalVertex) {
    char vertices[256];
    int numVertices = 0;
    vector<MyEdge> edgeList;
    int BellmanFordValue[256];
    int BellmanFordPrevious[256];

    createVertexMapping(edge, numberOfEdges, vertices, numVertices, edgeList);

    int startIdx = getIndex(startVertex, vertices, numVertices);
    int goalIdx = getIndex(goalVertex, vertices, numVertices);

    if (startIdx == -1 || goalIdx == -1) return "No path exists";

    // Khởi tạo 
    for (int i = 0; i < numVertices; ++i) {
        BellmanFordValue[i] = INT_MAX;
        BellmanFordPrevious[i] = -1;
    }

    BellmanFordValue[startIdx] = 0;

    // Chạy hàm bellman ford
    for (int i = 0; i < numVertices - 1; ++i) {
        bool changed = false;
        for (int j = 0; j < edgeList.size(); ++j) {
            int u = edgeList[j].from;
            int v = edgeList[j].to;
            int w = edgeList[j].cost;

            if (BellmanFordValue[u] != INT_MAX && BellmanFordValue[u] + w < BellmanFordValue[v]) {
                BellmanFordValue[v] = BellmanFordValue[u] + w;
                BellmanFordPrevious[v] = u;
                changed = true;
            }
        }
        if (!changed) break;
    }

    if (BellmanFordValue[goalIdx] == INT_MAX) return "No path exists";

    // Xây dựng đường dẫn
    vector<char> path;
    int current = goalIdx;
    while (current != -1) {
        path.push_back(vertices[current]);
        current = BellmanFordPrevious[current];
    }

    string result = "";
    for (int i = path.size() - 1; i >= 0; --i) {
        result += path[i];
        if (i > 0) result += " ";
    }
    return result;
}