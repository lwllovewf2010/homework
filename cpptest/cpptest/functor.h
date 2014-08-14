

struct add_x {

	add_x(int x) : x(x) {

	}
	int operator()(int y) {

		return x + y;
	}

private:
	int x;
};


/*
int main(int argc, char **argv) {

	add_x add_48( 48 );
	int i = add_48( 192 );

	cout << i << "\n";

	return 0;
}
*/

