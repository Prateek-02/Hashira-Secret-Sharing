# 📦 Catalog Placements Assignment – Shamir Secret Sharing Solver

This project implements a simplified version of **Shamir's Secret Sharing** using matrix-based polynomial reconstruction (Cramer's Rule) to recover the secret constant term `c` from given encoded points.

---

## 🧩 Problem Statement

You are given:
- A JSON file containing `n` points
- Each point has:
  - An `x` value (the key)
  - A `y` value encoded in a specific number `base`

You are also given:
- `k`: Minimum number of points required to reconstruct the polynomial (i.e., `degree = k - 1`)

Your task:
- Decode all points
- Select the first `k` points
- Reconstruct the degree `k-1` polynomial
- Output the constant term `c` — the **secret**

---

## 📂 Folder Structure

catalog-placement-assignment/
├── main.cpp           # 🔧 Main C++ implementation
├── json.hpp           # 📦 nlohmann/json single-header parser
├── testcase1.json     # 🧪 Sample test case 1 (provided)
├── testcase2.json     # 🧪 Sample test case 2 (provided)
├── README.md          # 📄 Project overview and instructions


## 📘 Explanation
The code uses:

1.Base decoding logic to convert values like "111" in base 2 to decimal
2.Matrix system (A · X = B) construction using the x-values
3.Cramer's Rule to solve the linear system for coefficients
4.The final coefficient c (the constant term) is extracted and printed

## 🛠️ Technologies Used
Language: C++ (C++17)
JSON Parsing: nlohmann/json (header-only)
Math: Determinants and basic linear algebra (no external libraries)