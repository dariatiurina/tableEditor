/**
 * @file graph.cpp
 * @author GeeksforGeeks
 * @brief Implementation of Graph
 * @version 1.0
 * @date 2023-05-31
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "graph.h"

Graph::Graph(int sizeS)
{
    this->size = sizeS;
    adj = new std::list<int>[size];
}

Graph::~Graph(){
    delete[] adj;
}

void Graph::addEdge(int ind, int anotherInd)
{
    adj[ind].push_back(anotherInd);
}

void Graph::topologicalSortUtil(int v, bool visited[], std::stack<int> &Stack)
{
    visited[v] = true;

    std::list<int>::iterator i;
    for (i = adj[v].begin(); i != adj[v].end(); ++i)
        if (!visited[*i])
            topologicalSortUtil(*i, visited, Stack);

    Stack.push(v);
}

std::vector<int> Graph::topologicalSort()
{
    std::stack<int> Stack;

    bool *visited = new bool[size];
    for (int i = 0; i < size; i++)
        visited[i] = false;

    for (int i = 0; i < size; i++)
        if (visited[i] == false)
            topologicalSortUtil(i, visited, Stack);

    std::vector<int> ret;
    while (Stack.empty() == false)
    {
        ret.push_back(Stack.top());
        Stack.pop();
    }

    delete[] visited;
    return ret;
}

bool Graph::isCyclicUtil(int v, bool visited[], bool* recStack)
{
    if (visited[v] == false) {
        visited[v] = true;
        recStack[v] = true;
 
        std::list<int>::iterator i;
        for (i = adj[v].begin(); i != adj[v].end(); ++i) {
            if (!visited[*i]
                && isCyclicUtil(*i, visited, recStack))
                return true;
            else if (recStack[*i])
                return true;
        }
    }

    recStack[v] = false;
    return false;
}

bool Graph::isCyclic()
{
    bool* visited = new bool[size];
    bool* recStack = new bool[size];
    for (int i = 0; i < size; i++) {
        visited[i] = false;
        recStack[i] = false;
    }

    for (int i = 0; i < size; i++)
        if (!visited[i] && isCyclicUtil(i, visited, recStack)){
            delete[] visited;
            delete[] recStack;
            return true;
        }
 
    delete[] visited;
    delete[] recStack;
    return false;
}