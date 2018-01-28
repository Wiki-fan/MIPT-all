#pragma once
#include "../stdafx.h"

/*6

2 3
2 3
2 3
2 3
2 3
6 1
6 3
 */

// Class that can find path in labyrinth.
class LabyrinthSolver {
public:

	// Perform reading and solving.
	int Process()
	{
		read();
		return find();
	}

private:
	/* Coordinates structure. */
	struct Coord {
		Coord() { }
		Coord( int _x, int _y ) : x( _x ), y( _y ) { }
		int x;
		int y;
	};

	std::vector<std::vector<int> > matrix;
	std::deque<Coord> deq;
	unsigned long int n;
	Coord source, target;

	// Read labyrinth from stdin.
	void read()
	{
		int x1, x2;
		// INT_MAX - free space, INT_MIN - wall.
		std::string str;

		// Trick: walls around labyrinth to not handle "out-of-array" errors.
		std::getline( std::cin, str );
		std::istringstream ist( str );
		ist >> n;
		matrix.resize( n + 2 );
		for( int i = 0; i < n + 2; i++ ) {
			matrix[i].resize( n + 2, INT_MAX );
		}
		// Reading matrix.
		for( int i = 1; i <= n; ++i ) {
			std::getline( std::cin, str );
			ist.str( str );
			ist.clear();
			while( ist >> x1 >> x2) {
				for( int j = x1; j < x2; ++j ) {
					matrix[i][j] = INT_MIN;
				}
			}
		}
		for(int i=0; i< n+2; i++){
			matrix[0][i] = INT_MIN;
			matrix[i][0] = INT_MIN;
			matrix[n+1][i] = INT_MIN;
			matrix[i][n+1] = INT_MIN;
		}


		std::cin >> source.x >> source.y;
		std::cin >> target.x >> target.y;
		/*target.x += 1;
		target.y += 1;
		source.x += 1;
		source.y += 1;*/
		print();
		matrix[source.x][source.y] = 0;
	}

	void print()
	{
		for( int i = 1; i <= n; ++i ) {
			for( int j = 1; j <= n; ++j ) {
				printf( "%11d ", matrix[i][j] );
			}
			printf( "\n" );
		}
		printf( "\n" );
	}
	// Check if [p.x+dx][p.y+dy] already visited, is target tile or needs to be pushed to queue.
	int check( Coord& p, int dx, int dy )
	{
		printf("%d %d\n", p.x + dx,p.y + dy);
		if( matrix[p.x + dx][p.y + dy] == INT_MAX ) {
			matrix[p.x + dx][p.y + dy] = matrix[p.x][p.y] + 1;
			print();
			if( p.x + dx == target.x && p.y + dy == target.y ) {
				return 0;
			}
			deq.push_back( Coord( p.x + dx, p.y + dy ));
		}
		return 1;
	}

	// Find answer.
	int find()
	{
		Coord p;
		deq.push_back( source );
		while( !deq.empty()) {
			p = deq.front();
			deq.pop_front();
			if( !(check( p, 1, 0 ) &&
					check( p, -1, 0 ) &&
					check( p, 0, 1 ) &&
					check( p, 0, -1 )
			)) {
				break;
			}
		}
		int ret;
		if( matrix[target.x][target.y] != INT_MAX && matrix[target.x][target.y] != INT_MIN) {
			ret = matrix[target.x][target.y];
		} else {
			ret = -1;
		}
		matrix.clear();
		deq.clear();
		return ret;
	}
};

void TestLabyrinth()
{
	LabyrinthSolver ls;
	int ans = ls.Process();
	if( ans != -1 ) {
		std::cout << ans;
	} else {
		std::cout << "No way";
	}
}
