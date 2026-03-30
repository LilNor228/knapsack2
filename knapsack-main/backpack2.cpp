#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <fstream>

#include <filesystem>
namespace fs = std::filesystem;
using namespace std;

struct Node {
    int level;
    double profit;
    double bound;
    int weight;

    Node(int lvl, double prof, int wgt) : level(lvl), profit(prof), weight(wgt), bound(0) {}
};

double calculateBound(Node &node, int N, int W, vector<pair<int, int>> &items) {
    if (node.weight >= W) return 0;

    double profitBound = node.profit;
    int j = node.level + 1;
    int totalWeight = node.weight;


    while (j < N && totalWeight + items[j].second <= W) {
        totalWeight += items[j].second;
        profitBound += items[j].first;
        j++;
    }


    if (j < N) {
        profitBound += (W - totalWeight) * (static_cast<double>(items[j].first) / items[j].second);
    }

    return profitBound;
}

int main() {
    std::string line;
    std::string path = "data";
    for (const auto & entry : fs::directory_iterator(path)){
        ifstream in(entry.path());
        //cout << entry.path();


        int N;
        in >> N;

        int W;
        in >> W;

        vector<pair<int, int>> items(N);

        for (int i = 0; i < N; i++) {
            in >> items[i].first >> items[i].second;
        }
        in.close();


        sort(items.begin(), items.end(), [](pair<int, int> &a, pair<int, int> &b) {
            return (static_cast<double>(a.first) / a.second) > (static_cast<double>(b.first) / b.second);
        });

        double maxProfit = 0;


        Node root(-1, 0, 0);
        root.bound = calculateBound(root, N, W, items);


        vector<Node> stack;
        stack.push_back(root);

        while (!stack.empty()) {
            Node node = stack.back();
            stack.pop_back();


            if (node.level == N - 1) continue;


            int nextLevel = node.level + 1;


            Node include(nextLevel, node.profit + items[nextLevel].first, node.weight + items[nextLevel].second);
            if (include.weight <= W) {
                if (include.profit > maxProfit) {
                    maxProfit = include.profit;
                }
                include.bound = calculateBound(include, N, W, items);
                if (include.bound > maxProfit) {
                    stack.push_back(include);
                }
            }


            Node exclude(nextLevel, node.profit, node.weight);
            exclude.bound = calculateBound(exclude, N, W, items);
            if (exclude.bound > maxProfit) {
                stack.push_back(exclude);
            }
        }
        cout << "\n " << entry.path() << "  " << maxProfit;
        //cout << maxProfit << endl;
    }
    return 0;
}
