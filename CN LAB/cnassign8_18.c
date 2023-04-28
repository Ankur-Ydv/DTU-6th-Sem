#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NODES 100
#define INFINITY 99999

typedef struct
{
    int cost;
    int next_hop;
} RoutingEntry;

typedef struct
{
    int seq_num;
    int neighbors[MAX_NODES];
    int costs[MAX_NODES];
} LSA;

RoutingEntry table[MAX_NODES][MAX_NODES];
LSA lsas[MAX_NODES];
int n;

void initialize_table()
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            table[i][j].cost = (i == j) ? 0 : INFINITY;
            table[i][j].next_hop = -1;
        }
    }
}

void dijkstra(int source)
{
    bool visited[n];
    memset(visited, false, sizeof(visited));
    table[source][source].cost = 0;
    table[source][source].next_hop = source;
    for (int i = 0; i < n; i++)
    {
        int min_cost = INFINITY;
        int min_node = -1;
        for (int j = 0; j < n; j++)
        {
            if (!visited[j] && table[source][j].cost < min_cost)
            {
                min_cost = table[source][j].cost;
                min_node = j;
            }
        }
        if (min_node == -1)
        {
            break;
        }
        visited[min_node] = true;
        for (int j = 0; j < n; j++)
        {
            if (lsas[min_node].neighbors[j] && table[source][min_node].cost + lsas[min_node].costs[j] < table[source][j].cost)
            {
                table[source][j].cost = table[source][min_node].cost + lsas[min_node].costs[j];
                table[source][j].next_hop = min_node;
            }
        }
    }
}

void update_routing_table(int source)
{
    for (int i = 0; i < n; i++)
    {
        if (i == source)
        {
            continue;
        }
        table[i][source].cost = lsas[i].costs[source];
        table[i][source].next_hop = source;
        for (int j = 0; j < n; j++)
        {
            if (j == source || j == i)
            {
                continue;
            }
            if (lsas[i].neighbors[j] && table[source][j].cost + lsas[i].costs[j] < table[i][j].cost)
            {
                table[i][j].cost = table[source][j].cost + lsas[i].costs[j];
                table[i][j].next_hop = source;
            }
        }
    }
}

void send_lsas()
{
    for (int i = 0; i < n; i++)
    {
        lsas[i].seq_num++;
        for (int j = 0; j < n; j++)
        {
            lsas[i].costs[j] = table[i][j].cost;
        }
    }
}

void print_tables()
{
    printf("Routing tables:\n");
    for (int i = 0; i < n; i++)
    {
        printf("Node %d:\n", i);
        printf("Destination\tCost\tNext Hop\n");
        for (int j = 0; j < n; j++)
        {
            printf("%d\t\t%d\t%d\n", j, table[i][j].cost, table[i][j].next_hop);
        }
    }
}

int main()
{
    FILE *fp;
    fp = fopen("file.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file.\n");
        exit(1);
    }
    fscanf(fp, "%d", &n);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int cost;
            fscanf(fp, "%d", &cost);
            if (cost > 0)
            {
                lsas[i].neighbors[j] = 1;
                lsas[i].costs[j] = cost;
            }
        }
    }
    initialize_table();
    for (int i = 0; i < n; i++)
    {
        dijkstra(i);
        update_routing_table(i);
    }
    print_tables();
    fclose(fp);
    return 0;
}
