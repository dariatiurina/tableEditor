/**
 * @file graph.h
 * @author GeeksforGeeks
 * @brief Declaration of Graph
 * @version 1.0
 * @date 2023-05-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <list>
#include <stack>

/**
 * @brief ADT Graph
 *
 */
class Graph
{
public:
    /**
     * @brief Construct a new Graph object
     *
     * @param sizeS number of elements in Graph
     */
    Graph(int sizeS);

    /**
     * @brief Destroy the Graph object
     *
     */
    ~Graph();

    /**
     * @brief Add new edge to a graphs
     *
     * @param ind from what point
     * @param anotherInd to what point
     */
    void addEdge(int ind, int anotherInd);

    /**
     * @brief Topological sort
     *
     * @return std::vector<int> indexes of elements after topological sort
     */
    std::vector<int> topologicalSort();

    /**
     * @brief Detects if graph has cycle
     *
     * @return true graph has cycle
     * @return false graph doesn't have cycle
     */
    bool isCyclic();

private:
    //!> number of points in graph
    int size;

    //!> connections between points
    std::list<int> *adj;

    //!> helping recursion function for Topological sort
    void topologicalSortUtil(int v, bool visited[], std::stack<int> &Stack);

    //!> helping recursion function for cycle detection
    bool isCyclicUtil(int v, bool visited[], bool *rs);
};

#endif // GRAPH_H