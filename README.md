# Cricket Scorecard Management System

A console-based cricket scorecard and live match tracking application built entirely in C++. This project demonstrates fundamental algorithmic design by implementing standard data structures from scratch without relying on the Standard Template Library (STL).

## 🚀 Core Architectural Features

- **Custom Linked List:** Implements a dynamic singly linked list (`Player* head`) within the `CricketTeam` class to handle dynamic squad generation, sequential lookup, and stat allocation.
- **Custom Queue Architecture:** Features a custom structural Queue engine (`BattingQueue`) managing a First-In, First-Out (FIFO) lineup sequence to dynamically allocate striking and non-striking batsmen.
- **Dynamic Structural Pointer Management:** Utilizes low-level pointer arithmetic (`striker`, `nonStriker`) to track player identity shifts, dismissals, and physical side changes over standard overs cleanly.
- **Precision Data Outputs:** Leverages standard `<iomanip>` streams to output live, tabular metrics on individual strike performance, extra runs calculations, and aggregate team metrics.

## 🛠️ Tech Stack

- **Language:** C++ (C++11 or higher recommended)
- **Paradigm:** Object-Oriented Programming (OOP)
- **Libraries:** Standard IO streams (`<iostream>`, `<iomanip>`, `<string>`)

## 🎮 How to Run

1. **Compile the source file:**
   ```bash
   g++ main.cpp -o cricket_scorecard
