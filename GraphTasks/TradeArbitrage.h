#pragma once
#include <cfloat>
#include "../stdafx.h"

// Class that can perform trade arbitrage.
class TradeArbitrage {
public:

	// Function that performs trade arbitrage.
	int PerformArbitrage()
	{
		int ret;
		read();
		doFloydWarshall();
		ret = getAnswer();
		matrix.clear();
		return ret;
	}

private:
	std::vector<std::vector<double> > matrix;
	unsigned long n;

	void read()
	{
		std::cin >> n;
		matrix.resize( n );
		for( int i = 0; i < n; i++ ) {
			matrix[i].resize( n, 0 );
			for( int j = 0; j < n; j++ ) {
				if( j != i ) {
					std::cin >> matrix[i][j];
					if( matrix[i][j] == -1 ) {
						matrix[i][j] = 0;
					}
				} else {
					matrix[i][j] = 1.0;
				}
				//printf("%lf ", matrix[i][j]);
			}
			//printf("\n");
		}

	}

	void doFloydWarshall()
	{
		for( int k = 0; k < n; k++ ) {
			for( int i = 0; i < n; i++ ) {
				for( int j = 0; j < n; j++ ) {
					if( matrix[i][j] < matrix[i][k] * matrix[k][j] && (matrix[i][j]!=0)) {
						//printf("%lf: %lf %lf\n", matrix[i][j], matrix[i][k], matrix[k][j]);
						matrix[i][j] = matrix[i][k] * matrix[k][j];
					}
				}
			}
		}
	}

	int getAnswer()
	{
		int ans = 0;
		for( int i = 0; i < n; i++ ) {
			//std::cout << matrix[i][i] << ' ';
			if( matrix[i][i] > 1 ) {
				ans = 1;
			}
		}
		return ans;
	}
};

int TestArbitrage()
{
	TradeArbitrage ta;
	if( ta.PerformArbitrage() == 0 ) {
		std::cout << "No";
	} else {
		std::cout << "Yes";
	}
	return 0;
}
