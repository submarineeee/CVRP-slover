# CVRP Solver — Operations Research Project

**Course:** Operations Research and Logistics · TU Dresden (2026)  
**Language:** C++ · **Solver:** SCIP Optimization Suite  
**Status:** Completed · All instances solved to optimality (0.00% gap)

---

## Problem Overview

This project implements an exact solver for the **Capacitated Vehicle Routing 
Problem (CVRP)** — a classic NP-hard combinatorial optimization problem in 
logistics and transportation.

Given:
- A set of **customer nodes** with known demands
- A **depot node** as the start/end point for all vehicles
- A fixed **vehicle capacity** (CAP = 15)

**Goal:** Find a set of minimum-cost routes such that every customer is visited 
exactly once and no vehicle exceeds its capacity.

---

## Methodology

### Step 1 — Graph Construction
A connected graph of 27 nodes was constructed based on a student-ID-derived 
node selection scheme, yielding 57 arcs.

### Step 2 — Floyd-Warshall Preprocessing
All-pairs shortest paths were computed using the **Floyd-Warshall algorithm**:
```
d[i][j] = min(d[i][j], d[i][k] + d[k][j])  for all k
```

This produces the distance matrix `d_ij` used as the cost parameter in the ILP.

### Step 3 — ILP Formulation

The CVRP is modeled as a **Mixed Integer Linear Program**:

**Decision Variables:**
- `x_ij ∈ {0,1}` — 1 if arc (i→j) is used, 0 otherwise
- `u_i ≥ 0` — accumulated vehicle load upon arriving at node i

**Objective:** Minimize total travel distance

**Key Constraints:**
- Each customer is visited exactly once (in-flow = out-flow = 1)
- Number of vehicles leaving depot ≤ K_max (= 5)
- **MTZ sub-tour elimination:** `u_i - u_j + M·x_ij ≤ M - q_j`
- Capacity bounds: `q_i ≤ u_i ≤ CAP`

### Step 4 — Solving with SCIP
The ILP model is solved using the **[SCIP Optimization Suite](https://www.scipopt.org/)**, 
one of the fastest non-commercial solvers for mixed-integer programs.

---

## Instances & Results

| Instance | Nodes | Vehicles Used | Total Distance | Optimality Gap |
|----------|-------|---------------|----------------|----------------|
| CVRP 1   | 6     | 1             | 16.000         | 0.00%          |
| CVRP 2   | 10    | 1             | 55.269         | 0.00%          |
| CVRP 3   | 16    | 1             | 86.654         | 0.00%          |
| CVRP 4   | 21    | 1             | 94.654         | 0.00%          |
| CVRP 5   | 27    | 1             | 114.228        | 0.00%          |

All five instances were solved to **proven global optimality**.

---

## Route Visualization (CVRP 5 — largest instance, 27 nodes)

<img width="1270" height="814" alt="image" src="https://github.com/user-attachments/assets/f892fc32-063a-4b8a-a579-243b868f11fb" />

Optimal route:  
`6 → 7 → 8 → 9 → 16 → 17 → 19 → 20 → 18 → 21 → 27 → 28 → 29 → 32 → 30 → 31 → 22 → 23 → 24 → 25 → 26 → 49 → 50 → 51 → 52 → 53 → 54 → 6`

---

## Project Structure
```

├── src/
│   ├── main.cpp        # Entry point: SCIP model setup, ILP solving pipeline
│   ├── graph.cpp       # Graph construction, Floyd-Warshall execution, XML I/O
│   ├── arc.cpp         # Arc data structure and arc-related operations
│   ├── node.cpp        # Node data structure and node-related operations
│   ├── vehicle.cpp     # Vehicle routing logic and capacity management
│   ├── arc.h           # Arc class declaration and type definitions
│   ├── node.h          # Node class declaration and type definitions
│   ├── vehicle.h       # Vehicle class declaration and type definitions
│   └── graph.h         # Graph class declaration and interface definitions
├── data/
│   └── graph.xml       # Input graph: node coordinates and arc definitions
├── results/
│   └── CVRP5.xml       # Optimal route output for the largest instance (27 nodes)
└── README.md
```

---

## Key Concepts Demonstrated

- **Combinatorial Optimization** — ILP modeling of NP-hard routing problems
- **Graph Algorithms** — Floyd-Warshall all-pairs shortest paths  
- **Mathematical Programming** — MTZ formulation for sub-tour elimination
- **Solver Integration** — SCIP C++ API for branch-and-bound solving
- **Data Engineering** — XML-based graph data management

---

