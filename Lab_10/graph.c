#include <stdio.h>
#include <limits.h>
#include<omp.h>
#define V 10

int minKey(int dist[], bool Vist[])
{
    int min = INT_MAX, min_index;
    int num_threads;
    #pragma omp parallel num_threads(V)
    {
        num_threads = omp_get_num_threads();
    #pragma omp for
        for (int v = 0; v < V; v++)
            if (Vist[v] == false && dist[v] < min)
    #pragma omp critical
                min = dist[v], min_index = v;
    }
    return min_index;
}

void printMST(int start[], int n, int graph[V][V])
{
    printf("Edge   Weight\n");
    for (int i = 1; i < V; i++)
        printf("%d - %d    %d \n", start[i], i, graph[i][start[i]]);
}

void primMST(int graph[V][V])
{
    int start[V];
    int dist[V], num_threads;
    bool Vist[V];
    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX, Vist[i] = false;

    dist[0] = 0;
    start[0] = -1;

    for (int count = 0; count < V - 1; count++)
    {
        int u = minKey(dist, Vist);
        Vist[u] = true;

        int v;

    #pragma omp parallel num_threads(V)
        {
            num_threads = omp_get_num_threads();
    #pragma omp for
            for (v = 0; v < V; v++)
                if (graph[u][v] && Vist[v] == false && graph[u][v] < dist[v])
            #pragma omp critical
                    start[v] = u, dist[v] = graph[u][v];
        }
    }
    printMST(start, V, graph);
}
int main()
{
    double start = omp_get_wtime();
    int graph[V][V] = { {0, 0, 0, 3, 0, 0, 0, 7, 0, 9}, 
                        {0, 0, 2, 3, 4, 5, 1, 1, 8, 9}, 
                        {0, 2, 0, 3, 4, 2, 6, 7, 8, 2}, 
                        {3, 3, 3, 0, 3, 3, 6, 3, 8, 9}, 
                        {0, 4, 4, 3, 0, 4, 4, 7, 4, 9},
                        {0, 5, 2, 3, 4, 0, 5, 5, 8, 9},
                        {0, 1, 6, 6, 4, 5, 0, 6, 6, 6},
                        {7, 1, 7, 3, 7, 5, 6, 0, 7, 7},
                        {0, 8, 8, 8, 4, 8, 6, 7, 0, 9},
                        {9, 9, 2, 9, 9, 9, 6, 7, 9, 0}
    };
    primMST(graph);
    double end = omp_get_wtime();
    printf("Time = %.16g\n", end - start);

    return 0;
}