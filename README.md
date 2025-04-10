# Random Walk Simulation in C

Simulates 1D, 2D, and 3D random walks using Monte Carlo methods, with optional self-avoiding behavior in higher dimensions. Designed to analyze expected displacement and variance across a large number of trials, inspired by physical diffusion models — with relevance to stochastic modeling in quantitative finance.

---

## Features

- **Configurable Walks**: Supports 1D, 2D, and 3D random walks via user input.
- **Self-Avoiding Walks**: Optional for 2D and 3D modes — avoids revisiting the same coordinates.
- **Statistical Outputs**: Calculates average displacement and mean squared displacement over 100,000+ simulations.
- **File Output**: Saves simulation results to `.txt` files for further analysis or plotting.

---

## Core Concepts

- **Stochastic Processes**: Models based on symmetric discrete-time random walks.
- **Diffusion Constant**: Adjustable via user input, with probability `p = D * Δt / Δx²`.
- **Monte Carlo Simulation**: Repeats the walk process for statistical averaging.

These concepts are widely applicable in **financial modeling**, including:
- Asset price evolution (e.g., Brownian motion)
- Risk assessment
- Options pricing foundations

---

## Getting Started

### Requirements
- C compiler (e.g., `gcc`)
- Standard C libraries (`stdio.h`, `stdlib.h`, `math.h`, etc.)

### Compile
```bash
gcc -o random_walk main.c -lm
```

### Run
```bash
./random_walk
```

The program will prompt for:
- Diffusion constant (0 < D ≤ 0.5)
- Number of dimensions (1, 2, or 3)
- Number of steps (max 1000)
- Whether the walk should be self-avoiding (for 2D/3D)

---

## Output

- `positions.txt`: Trajectory of the **first** simulation (for plotting).
- `final_positions.txt`: Final coordinates of all simulations (used for statistical aggregation).
- Console output includes average absolute displacement and squared displacement.

---

## Why This Matters

Understanding the statistical behavior of random walks is essential in fields like:
- Quantitative finance (e.g., asset diffusion, volatility modeling)
- Computational physics
- Algorithmic trading strategies

This project demonstrates both the **mathematical foundations** and **efficient C implementation** of such models.

---

## Author

**Guy Mangan**  
BSc Physics, University of Exeter 
