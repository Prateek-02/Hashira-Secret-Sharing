#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <iomanip> 
#include <set>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

using LD = long double;
const LD EPS = 1e-9;

// Decode base-x value 
LD decodeBaseX(string value, int base) {
    LD result = 0;
    for (char c : value) {
        int digit;
        if (isdigit(c)) digit = c - '0';
        else digit = toupper(c) - 'A' + 10;
        result = result * base + digit;
    }
    return result;
}

//  Calculate Determinant
LD determinant(vector<vector<LD>> mat) {
    int n = mat.size();
    if (n == 1) return mat[0][0];
    if (n == 2)
        return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];

    LD det = 0;
    for (int p = 0; p < n; ++p) {
        vector<vector<LD>> submat(n - 1, vector<LD>(n - 1));
        for (int i = 1; i < n; ++i) {
            int colIndex = 0;
            for (int j = 0; j < n; ++j) {
                if (j != p) {
                    submat[i - 1][colIndex++] = mat[i][j];
                }
            }
        }
        det += mat[0][p] * pow(-1, p) * determinant(submat);
    }
    return det;
}

// Solve using Cramer's Rule 
LD solveConstantTerm(const vector<pair<LD, LD>>& points, int k) {
    vector<vector<LD>> A(k, vector<LD>(k));
    vector<LD> B(k);

    for (int i = 0; i < k; ++i) {
        LD x = points[i].first;
        LD y = points[i].second;
        B[i] = y;
        for (int j = 0; j < k; ++j) {
            A[i][j] = pow(x, k - j - 1);
        }
    }

    LD detA = determinant(A);
    if (fabs(detA) < EPS) return -1;

    // Replace last column with B
    vector<vector<LD>> A_const = A;
    for (int i = 0; i < k; ++i)
        A_const[i][k - 1] = B[i];

    LD detConst = determinant(A_const);
    LD constantTerm = detConst / detA;
    return round(constantTerm);
}

// Generate all combinations
void generateCombinations(const vector<pair<LD, LD>>& points, int k, int idx, vector<pair<LD, LD>>& current, map<LD, int>& freqMap) {
    if (current.size() == k) {
        LD secret = solveConstantTerm(current, k);
        if (secret != -1)
            freqMap[secret]++;
        return;
    }

    for (int i = idx; i < points.size(); ++i) {
        current.push_back(points[i]);
        generateCombinations(points, k, i + 1, current, freqMap);
        current.pop_back();
    }
}

//Solve from a file 
LD solveFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening " << filename << endl;
        return -1;
    }

    json j;
    file >> j;

    int n = j["keys"]["n"];
    int k = j["keys"]["k"];

    vector<pair<LD, LD>> points;
    for (auto it = j.begin(); it != j.end(); ++it) {
        if (it.key() == "keys") continue;
        int x = stoi(it.key());
        int base = stoi(it.value()["base"].get<string>());
        string valStr = it.value()["value"];
        LD y = decodeBaseX(valStr, base);
        points.push_back({x, y});
    }

    // Generate all combinations of k out of n and vote on constant term
    map<LD, int> freqMap;
    vector<pair<LD, LD>> current;
    generateCombinations(points, k, 0, current, freqMap);

    // Find the most frequent c
    LD bestC = -1;
    int maxFreq = 0;
    for (auto it = freqMap.begin(); it != freqMap.end(); ++it) {
    if (it->second > maxFreq) {
        maxFreq = it->second;
        bestC = it->first;
    }
}


    return bestC;
}

// ---------- Main ----------
int main() {
    LD secret1 = solveFromFile("testcase1.json");
    LD secret2 = solveFromFile("testcase2.json");

    cout << "Secret from Testcase 1: "<< std::fixed << std::setprecision(0) << secret1 << endl;
    cout << "Secret from Testcase 2: "<< std::fixed << std::setprecision(0)  << secret2 << endl;

    return 0;
}
