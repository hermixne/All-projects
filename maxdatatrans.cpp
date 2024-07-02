#include <vector>
#include <algorithm>
#include <iostream>
#include <limits>

using namespace std;

class MaxHeap {
private:
    vector<pair<int, pair<int, int>>> listed;
    int length;

    void heapUp(int idx) {
        while (idx > 0) {
            int parent = (idx - 1) / 2;
            if (listed[idx].first > listed[parent].first) {
                swap(listed[idx], listed[parent]);
                idx = parent;
            } else {
                return;
            }
        }
    }

    void heapDown(int idx) {
        while (2 * idx + 2 < length) {
            int leftChild = 2 * idx + 1;
            int rightChild = 2 * idx + 2;
            int maxChild = (listed[leftChild].first > listed[rightChild].first) ? leftChild : rightChild;

            if (listed[idx].first < listed[maxChild].first) {
                swap(listed[idx], listed[maxChild]);
                idx = maxChild;
            } else {
                return;
            }
        }

        if (2 * idx + 1 < length && listed[idx].first < listed[2 * idx + 1].first) {
            swap(listed[idx], listed[2 * idx + 1]);
        }
    }

public:
    MaxHeap() : length(0) {}

    void enqueue(const pair<int, pair<int, int>>& x) {
        listed.push_back(x);
        length++;
        heapUp(length - 1);
    }

    pair<int, pair<int, int>> extractMax() {
        if (length == 0) {
            throw runtime_error("Heap is empty");
        }
        auto max = listed[0];
        listed[0] = listed.back();
        listed.pop_back();
        length--;
        if (length > 0) {
            heapDown(0);
        }
        return max;
    }

    bool isEmpty() const {
        return length == 0;
    }
};

pair<int, vector<int>> findMaxCapacity(int n, const vector<pair<int, pair<int, int>>>& links, int s, int t) {
    vector<vector<pair<int, pair<int, int>>>> adjList(n);
    vector<bool> visited(n, false);
    MaxHeap edgeList;
    vector<int> prevTrack(n, -1);

    for (const auto& link : links) {
        int u = link.second.first;
        int v = link.second.second;
        int c = link.first;
        adjList[u].push_back({c, {v, u}});
        adjList[v].push_back({c, {u, v}});
    }

    for (const auto& edge : adjList[s]) {
        edgeList.enqueue(edge);
    }

    visited[s] = true;
    int capacity = numeric_limits<int>::max();

    while (!edgeList.isEmpty()) {
        auto a = edgeList.extractMax();
        int currentCapacity = a.first;
        int currentNode = a.second.first;
        int prevNode = a.second.second;

        if (!visited[currentNode]) {
            visited[currentNode] = true;
            prevTrack[currentNode] = prevNode;
            capacity = min(capacity, currentCapacity);

            if (currentNode == t) {
                break;
            }

            for (const auto& edge : adjList[currentNode]) {
                if (!visited[edge.second.first]) {
                    edgeList.enqueue(edge);
                }
            }
        }
    }

    vector<int> tracker;
    int current = t;
    while (current != -1) {
        tracker.push_back(current);
        current = prevTrack[current];
    }
    reverse(tracker.begin(), tracker.end());

    return {capacity, tracker};
}

int main() {
    auto result1 = findMaxCapacity(7, {{2,{0,1}},{5,{0,2}},{4,{1,3}},{4,{2,3}},{6,{3,4}},{4,{3,5}},{1,{2,6}},{2,{6,5}}}, 0, 5);
    cout << "Capacity: " << result1.first << ", Path: ";
    for (int node : result1.second) {
        cout << node << " ";
    }
    cout << endl;

    auto result2 = findMaxCapacity(5, {{3,{0,1}},{5,{1,2}},{2,{2,3}},{3,{3,4}},{8,{4,0}},{7,{0,3}},{4,{1,3}}}, 0, 2);
    cout << "Capacity: " << result2.first << ", Path: ";
    for (int node : result2.second) {
        cout << node << " ";
    }
    cout << endl;

    return 0;
}