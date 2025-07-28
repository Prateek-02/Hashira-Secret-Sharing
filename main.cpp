#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include "json.hpp"  // Make sure json.hpp is in same folder

using json = nlohmann::json;
using namespace std;

using LD = long double;
const LD EPS = 1e-9;

// ----------- Decode Base-X to Decimal ------------
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

// ------------ Determinant (Recursive) -------------
LD determinant(vector<vector<LD>> mat) {
    int n = mat.size();
    if (n == 1) return mat[0][0];
    if (n == 2)
        return mat[0][0]*mat[1][1] - mat[0][1]*mat[1][0];

    LD det = 0;
    for (int p = 0; p < n; ++p) {
        vector<vector<LD>> submat(n-1, vector<LD>(n-1));
        for (int i = 1; i < n; ++i)
            for (int j = 0, col = 0; j < n; ++j)
                if (j != p)
                    submat[i-1][col++] = mat[i][j];
        det += mat[0][p] * pow(-1, p) * determinant(submat);
    }
    return det;
}

// ---------- Solve Using Cramer's Rule ----------
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
    if (fabs(detA) < EPS) return -1; // invalid

    // Replace last column with B
    vector<vector<LD>> A_const = A;
    for (int i = 0; i < k; ++i)
        A_const[i][k - 1] = B[i];

    LD detConst = determinant(A_const);
    LD constantTerm = detConst / detA;
    return round(constantTerm);  // final secret
}

// ---------- Parse JSON and Solve -----------
LD solveFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Cannot open " << filename << endl;
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


    // Sort to ensure consistent results
    sort(points.begin(), points.end());

    // Use first k points
    vector<pair<LD, LD>> subset(points.begin(), points.begin() + k);
    return solveConstantTerm(subset, k);
}

// -------------- Main ---------------
int main() {
    string file1 = "testcase1.json";
    string file2 = "testcase2.json";

    LD secret1 = solveFromFile(file1);
    LD secret2 = solveFromFile(file2);

    cout << "Secret from Testcase 1: " << secret1 << endl;
    cout << "Secret from Testcase 2: " << secret2 << endl;

    return 0;
}
