#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <random>
#include <filesystem>
#include <fstream>
using namespace std;

namespace fs = std::filesystem;
using namespace std;


struct Item {
    int value;
    int weight;
};

int main() {
    std::string line;
    std::string path = "data";
    for (const auto & entry : fs::directory_iterator(path)){
        ifstream in(entry.path());
        int ans;
        int N;
        in >> N;

        int W;
        in >> W;

        vector<Item> items(N);


        for (int i = 0; i < N; i++) {
            in >> items[i].value >> items[i].weight;
        }

        vector<vector<bool>> population(4, vector<bool>(N, false));
        for (int i = 0; i < 4 && i < N; i++) {
            population[i][i] = true;
        }

        for (int h = 0; h < 200; h++) {
            vector<vector<bool>> new_population(10, vector<bool>(N, false));
            int iter = 0;


            for (int i = 0; i < 4; i++) {
                for (int j = i + 1; j < 4; j++) {
                    if (iter < 10) {
                        for (int k = 0; k < N; k++) {
                            new_population[iter][k] = population[i][k] ^ population[j][k];
                        }
                        iter++;
                    }
                }
            }

            for (int i = 0; i < 4 && iter < 10; i++) {
                new_population[iter++] = population[i];
            }

            vector<Item> cost(iter);
            vector<pair<int, pair<int, int>>> valid_items;


            for (int i = 0; i < iter; i++) {
                cost[i].value = 0;
                cost[i].weight = 0;
                for (int j = 0; j < N; j++) {
                    cost[i].value += new_population[i][j] * items[j].value;
                    cost[i].weight += new_population[i][j] * items[j].weight;
                }

                if (cost[i].weight <= W) {
                    valid_items.emplace_back(cost[i].value, make_pair(cost[i].weight, i));
                }
            }


            sort(valid_items.rbegin(), valid_items.rend());


            int limit = min(4, (int)valid_items.size());
            if (valid_items[0].first > ans){
                ans= valid_items[0].first;
            }



            for (int i = 0; i < limit; i++) {
                population[i] = new_population[valid_items[i].second.second];
            }
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, N - 1);

            int num1 = dis(gen);
            int num2 = dis(gen);
            population[num1 % 4][num1] = population[num1 % 4][num1] ^ 1;
            population[num2 % 4][num2] = population[num2 % 4][num2] ^ 1;

        }
        cout << ans<< endl;
    }
    return 0;
}
