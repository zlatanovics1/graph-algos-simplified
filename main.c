#include <stdio.h>
#include <stdlib.h>

#define SIZE 40
#define MAX 1000
#define INF 9999



const char* gradovi[] = {"Beograd","Vranje","Nis","Kragujevac","Novi Pazar"};
struct node
    {
        int vertex;
        int weight;
        struct node* next;
    };
struct Graph
    {
        int numVertices;
        int* visited;
        struct node** adjLists; // we need int** to store a two dimensional array. Similary, we need struct node** to store an array of Linked lists
    };
struct queue {
        int items[SIZE];
        int front;
        int rear;
    };

    struct node* createNode(int v, int weight);
    struct Graph* createGraph(int);
    void addEdge(struct Graph*, int src, int dest, int weight);
    void printGraph(struct Graph*);



    // dfs bfs
    struct queue* createQueue();
    void enqueue(struct queue* q, int);
    int dequeue(struct queue* q);
    void display(struct queue* q);
    int isEmpty(struct queue* q);
    void printQueue(struct queue* q);
    void DFS(struct Graph*, int);
    void bfs(struct Graph* graph, int startVertex);



    // dijkstra
    void pisiPut(int t[], int src, int dest);
    void ispihSvih(int src, int t[], int d[]);
    void dijkstra(struct Graph* graph, int src);

    // toplosko
    void topologicalSort(struct node** adjLists, int V);

    void floydWarshall(struct Graph* graph, int N) ;


    int main()
    {
        struct Graph* graph = createGraph(6);
        addEdge(graph, 3, 1, 10);
        addEdge(graph, 3, 2, 5);
        addEdge(graph, 4, 2, 3);
        addEdge(graph, 4, 5, 3);
        addEdge(graph, 1, 0, 3);
        addEdge(graph, 0, 5, 3);


        /*addEdge(graph, 1, 3, 1);
        addEdge(graph, 2, 1, 2);
        addEdge(graph, 2, 3, 9);
        addEdge(graph, 2, 4, 2);
        addEdge(graph, 3, 4, 4);
        addEdge(graph, 4, 0, 7);
        addEdge(graph, 4, 3, 6);*/
        printGraph(graph);
       // dijkstra(graph,0);
        //floydWarshall(graph,5);
        // DFS(graph, 3);
        //bfs(graph,2);
        topologicalSort(graph->adjLists,graph->numVertices);

        return 0;
    }

void floydWarshall(struct Graph* graph,int N) {
    int d[N][N], t[N][N], i, j, k;

    // Inicijalizacija matrice rastojanja
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            d[i][j] = (i == j) ? 0 : INF;

    // Inicijalizacija matrice prethodnika
    for(i=0; i<N; i++)
        for(j=0; j<N; j++)
            t[i][j] = i;

    // Popunjavanje matrice rastojanja na osnovu listi susjedstva
    for (i = 0; i < graph->numVertices; i++) {
        struct node* current = graph->adjLists[i];
        while (current != NULL) {
            d[i][current->vertex] = current->weight;
            current = current->next;
        }
    }

    // Floyd-Warshall algoritam
    for (k = 0; k < N; k++)
        for (i = 0; i < N; i++)
            for (j = 0; j < N; j++)
                if (d[i][k] + d[k][j] < d[i][j]) {
                    t[i][j] = t[k][j];
                    d[i][j] = d[i][k] + d[k][j];
                }

    // Ispis rezultata
    printf ("The following matrix shows the shortest distances"
            " between every pair of vertices \n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (d[i][j] == INF)
                printf("%7s", "INF");
            else
                printf ("%7d", d[i][j]);
        }
        printf("\n");
    }

    printf("Paths:\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            //printf("Path from %s to %s: ", gradovi[i], gradovi[j]);
            printf("Path from %d to %d: ", i,j);

            int temp = j;
            //printf("%s    ", gradovi[temp]);
            printf("%d ", temp);
            while (temp != i) {
                // printf("%s ", gradovi[t[i][temp]]);
                printf("%d ", t[i][temp]);
                temp = t[i][temp];
            }
            printf("\n");
        }
        printf("\n\n");
    }
}

    // Dijkstra algoritam za listu susedstva
void dijkstra(struct Graph* graph, int src) {
    int V = graph->numVertices;
    int dist[V];
    int prev[V];

    // Initialization of dist[], prev[] and visited[]
    for (int i = 0; i < V; ++i) {
        dist[i] = INF;
        prev[i] = -1;
    }
    dist[src] = 0;

    // Marking visited nodes
    int visited[V];
    for (int i = 0; i < V; ++i)
        visited[i] = 0;

    // Main loop of the algorithm
    for (int count = 0; count < V - 1; ++count) {
        // Finding the node with the smallest distance
        int u = -1;
        for (int v = 0; v < V; ++v)
            if (!visited[v] && (u == -1 || dist[v] < dist[u]))
                u = v;

        // Marking the node as visited
        visited[u] = 1;

        // Updating distances of adjacent nodes of the selected node
        struct node* current = graph->adjLists[u];
        while (current != NULL) {
            int v = current->vertex;
            int weight = current->weight;
            if (!visited[v] && dist[u] != INF && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
            }
            current = current->next;
        }
    }

    // Printing the result
    printf("\nDo:\tTezina\tPutanja\n");
    for (int i = 0; i < V; ++i) {
        //printf("%s\t", gradovi[i]);
        printf("%d\t", i);
        if (dist[i] == INF)
            printf("INF\t");
        else
            //printf("%dkm\t", dist[i]);
            printf("%d\t", dist[i]);
        // Printing the path
        int temp = i;
        printf("|");
        while (temp != -1) {
            //printf("%s ", gradovi[temp]);
            printf("%d ", temp);
            temp = prev[temp];
        }
        printf("\n\n");
    }
}

// Pomoćna funkcija za ispis puta
void pisiPut(int prev[], int src, int dest) {
    if (src != dest)
        pisiPut(prev, src, prev[dest]);
    printf("%d ", dest);
}

void bfs(struct Graph* graph, int startVertex) {
        struct queue* q = createQueue();

        graph->visited[startVertex] = 1;
        enqueue(q, startVertex);

        while(!isEmpty(q)){
            printQueue(q);
            int currentVertex = dequeue(q);
            printf("\nVisited %d\n", currentVertex);

           struct node* temp = graph->adjLists[currentVertex];

           while(temp) {
                int adjVertex = temp->vertex;
                if(graph->visited[adjVertex] == 0){
                    graph->visited[adjVertex] = 1;
                    enqueue(q, adjVertex);
                }
                temp = temp->next;
           }
        }
    }

void DFS(struct Graph* graph, int vertex) {
            struct node* adjList = graph->adjLists[vertex];
            struct node* temp = adjList;

            graph->visited[vertex] = 1;
            printf("Visited %d \n", vertex);

            while(temp!=NULL) {
                int connectedVertex = temp->vertex;

                if(graph->visited[connectedVertex] == 0) {
                    DFS(graph, connectedVertex);
                }
                temp = temp->next;
            }
    }

// Funkcija za topološko sortiranje grafa

void topologicalSort(struct node** adjLists, int V) {
    int indegree[MAX] = {0};

    // Izračunavanje ulaznih stupnjeva za svaki čvor
    for (int v = 0; v < V; v++) {
        struct node* temp = adjLists[v];
        while (temp != NULL) {
            indegree[temp->vertex]++;
            temp = temp->next;
        }
    }

    // Kreiranje reda za čvorove s ulaznim stupnjem 0
    int queue[MAX];
    int front = 0, rear = -1;

    for (int i = 0; i < V; i++) {
        if (indegree[i] == 0)
            queue[++rear] = i;
    }

    // Brojač posjećenih čvorova
    int visited = 0;
    printf("Topolosko sortiranje grafa: \n");

    while (visited < V) {
        // Ako nema čvorova s ulaznim stupnjem 0, graf ima ciklus
        if (front > rear) {
            printf("\nGraf sadrzi ciklus!\n");
            return;
        }

        int currentVertex = queue[front++];
        printf("%d ", currentVertex);
        visited++;

        // Smanjenje ulaznih stupnjeva susjednih čvorova
        struct node* temp = adjLists[currentVertex];
        while (temp != NULL) {
            indegree[temp->vertex]--;
            if (indegree[temp->vertex] == 0)
                queue[++rear] = temp->vertex;
            temp = temp->next;
        }
    }
}

struct node* createNode(int v, int weight)
    {
        struct node* newNode = (struct node*)malloc(sizeof(struct node));
        newNode->vertex = v;
        newNode->weight = weight;
        newNode->next = NULL;
        return newNode;
    }
struct Graph* createGraph(int vertices)
    {
        struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
        graph->numVertices = vertices;

        graph->adjLists =(struct node**)malloc(vertices * sizeof(struct node*));

        graph->visited = malloc(vertices * sizeof(int));

        int i;
        for (i = 0; i < vertices; i++) {
            graph->adjLists[i] = NULL;
            graph->visited[i] = 0;
        }
        return graph;
    }

void addEdge(struct Graph* graph, int src, int dest,int weight)
    {
        // Add edge from src to dest
        struct node* newNode = createNode(dest,weight);
        newNode->next = graph->adjLists[src];
        graph->adjLists[src] = newNode;

        // Add edge from dest to src
        // ako neces neusmeren
        /*newNode = createNode(src,weight);
        newNode->next = graph->adjLists[dest];
        graph->adjLists[dest] = newNode;*/
    }

void printGraph(struct Graph* graph)
    {
        int v;
        for (v = 0; v < graph->numVertices; v++)
        {
            struct node* temp = graph->adjLists[v];
            printf("\n Adjacency list of vertex %d\n ", v);
            while (temp)
            {
                printf("%d -> ", temp->vertex);
                temp = temp->next;
            }
            printf("\n");
        }
    }

struct queue* createQueue(){
        struct queue* q = malloc(sizeof(struct queue));
        q->front = -1;
        q->rear = -1;
        return q;
    }

int isEmpty(struct queue* q) {
        if(q->rear == -1)
            return 1;
        else
            return 0;
    }
void enqueue(struct queue* q, int value){
        if(q->rear == SIZE-1)
            printf("\nQueue is Full!!");
        else {
            if(q->front == -1)
                q->front = 0;
            q->rear++;
            q->items[q->rear] = value;
        }
    }
int dequeue(struct queue* q){
        int item;
        if(isEmpty(q)){
            printf("\nQueue is empty\n");
            item = -1;
        }
        else{
            item = q->items[q->front];
            q->front++;
            if(q->front > q->rear){
                printf("\nResetting queue\n");
                q->front = q->rear = -1;
            }
        }
        return item;
    }
void printQueue(struct queue *q) {
        int i = q->front;
        if(isEmpty(q)) {
            printf("\nQueue is empty\n");
        } else {
            printf("\nQueue contains: \n");
            for(i = q->front; i < q->rear + 1; i++) {
                    printf("\t%d ", q->items[i]);
            }
        }
    }
