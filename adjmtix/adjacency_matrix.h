
class adjacencyMatrix {

public:
	adjacencyMatrix( int vertices );
	~adjacencyMatrix();
	void display();

//friend:

//protected:

private:
	int *matrix;
	int vertices;
};

