
class adjacencyMatrix {

public:
	adjacencyMatrix( int vertices );
	~adjacencyMatrix();

	bool insertVertex( int v );
	bool deleteVertex( int v );
	bool hasVertex( int v );

	bool insertEdge( int v, int u );
	bool deleteEdge( int v, int u );
	bool hasEdge( int v, int u );

	void display();

private:
	int *vertex;
	int *matrix;
	int vertices;
};

