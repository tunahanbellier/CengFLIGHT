# Flight Finder Application ✈️

This is a self-project that implements a functional airline flight finder system, similar to services like Skyscanner or Flightsfinder.com. The application uses C++ and relies on key data structures and algorithms to efficiently manage and process flight data.

## Features

* **Multi-Graph Structure**: Represents airports as vertices and flights as edges. This allows for a flexible and robust way to model the interconnected network of airline routes.
* **Dijkstra's Algorithm**: Utilized for finding the shortest path between airports based on a heuristic weight (e.g., cost vs. price).
* **Breadth-First Search (BFS)** and **Depth-First Search (DFS)**: These algorithms are employed for various graph traversal tasks, such as finding the maximum depth of transfers or discovering related "sister" airlines.
* **Hash Table**: An in-memory cache implemented with a hash table structure is used to store previously calculated flight paths. This significantly improves performance for repeated queries by avoiding redundant computations.
* **Flight Management**: The system can handle operations like temporarily halting and resuming specific flight routes. Halting a flight removes the corresponding edge from the graph and stores it in a separate list. Resuming a flight re-adds the edge to the graph.

## Data Structures

### `MultiGraph`
The core of the application's data management is the **`MultiGraph`** class. It models the flight network where:
* **Airports** are represented as **vertices** (`GraphVertex`).
* **Flights** are represented as **edges** (`GraphEdge`), allowing for multiple flights (edges) between the same two airports.

The `MultiGraph` class provides methods for:
* Inserting and removing vertices (airports).
* Adding and removing edges (flights), which includes handling exceptions for duplicate or non-existent vertices/edges.
* Finding the shortest path using a heuristic approach.
* Finding the shortest path while filtering out specific airlines.
* Calculating the number of bidirectional flights.
* Finding the maximum transfer depth from an airport using a specific airline.

### `HashTable`
The **`HashTable`** is used to implement a Least Recently Used (LRU) cache. This is crucial for optimizing performance. When a user searches for a flight, the system first checks the cache. If the path is found, it's retrieved instantly. If not, the path is calculated using Dijkstra's algorithm and then stored in the cache for future use.

## Algorithms

### Shortest Path (`HeuristicShortestPath`)
This function calculates the shortest path between a starting and ending airport. It uses a **modified Dijkstra's algorithm** that takes a `heuristicWeight` parameter. This parameter, `alpha`, allows the user to find a path that is weighted between two different criteria (e.g., cost and price). The algorithm uses a min-heap to efficiently explore the graph and find the optimal path.

### Filtered Path (`FilteredShortestPath`)
This function also finds the shortest path but with an added constraint: it avoids flights from a list of unwanted airlines. This is implemented using a variation of Dijkstra's algorithm that skips edges belonging to the specified airlines.

### `MaxDepthViaEdgeName`
This function performs a **BFS traversal** on the graph to find the maximum number of consecutive transfers from a given airport using a specific airline. This is useful for understanding the reach of an airline's network from a particular hub.
