#pragma once
#include "../stdafx.h"

// Класс решателя пятнашек.
class Solver15
{
public:
	int base;
	int num;

	Solver15() :pos(base)
	{ }

	// Задаёт начальные положения пятнашек массивом.
	void SetInput(std::vector<char>& v)
	{
		clear();
		base = sqrt(v.size());
		num=base*base;
		beg_pos = v;
		pos = Position(base);
		for (int i = 1; i<num; ++i) {
			pos.correct.push_back( i );
		}
		pos.correct.push_back(0);
	}

	// Считывает пятнашки из стандартного ввода.
	void read()
	{
		clear();
		std::cin >> base;
		num=base*base;
		beg_pos.resize(num);
		for (int i = 0; i<num; i++) {
			std::cin >> beg_pos[i];
		}
		pos = Position(base);
		for (int i = 1; i<num; ++i) {
			pos.correct.push_back( i );
		}
		pos.correct.push_back(0);
	}

	// Решает пятнашки.
	void Process()
	{
		solve();
	}

	// Получить последовательность действий для решения в массиве.
	std::vector<char> GetOutput()
	{
		return way2;
	}

	// Распечатать последовательность действий для решения.
	void Print()
	{
		for (int j = 0; j < way2.size(); j++) {
			std::cout << way2[j] << std::endl;
		}
	}

	// Получить массив с корректной расстановкой пятнашек.
	const std::vector<char> GetCorrectAnswer()
	{
		return pos.correct;
	}

private:
	struct Position
	{
	public:
		int base;
		int num; // base*base
		std::vector<char> massiv;
		int zero_pos;
		Position(int _base) :base(_base), num(base*base), massiv(num) {}

		void SetPos(std::vector<char> m)
		{
			massiv = m;
			for (int i = 0; i<massiv.size(); ++i) {
				if( massiv[i] == 0 ) {
					zero_pos = i;
				}
			}
		}

		std::vector<char> GetPerm(int dx, int dy)
		{
			if (
					(zero_pos % base == 0 && dx == -1 && dy == 0) // left
							|| (zero_pos % base == base-1 && dx == 1 && dy == 0) // right
							|| (zero_pos < base && dx == 0 && dy == -1) // up
							|| (zero_pos >= base*(base-1) && dx == 0 && dy == 1) // down
					) {
				return std::vector<char>();
			} else {
				std::swap(massiv[zero_pos], massiv[zero_pos +dx +base*dy]);
				//print();
				std::vector<char> ret(massiv);
				std::swap(massiv[zero_pos], massiv[zero_pos +dx +base*dy]);
				return ret;
			}
		}

		void print()
		{
			for (int i = 0; i<num; ++i) {
				if (i %base == 0) {
					printf("\n");
				}
				printf("%2d ", massiv[i]);
			}
			printf("\n");
		}

		~Position() {}
		std::vector<char> correct;
	};

	Position pos;
	std::set <std::vector<char>> set;
	std::deque <std::vector<char>> que;
	std::deque <std::vector<char>> ways;
	std::vector <char> way1;
	std::vector <char> way2;
	std::vector<char> beg_pos;

	void solve()
	{
		int ans;

		bool fl = false;
		std::vector<char> m;
		std::vector<char> n; // current arrangement vector

		set.insert(beg_pos);
		que.push_back(beg_pos);
		ways.push_back(way1); // pushing empty way
		while (!fl && (!que.empty())) {
			m = que.front();

			pos.SetPos(m);

			fl = ProcessPerm('U') || ProcessPerm('D') || ProcessPerm('L') || ProcessPerm('R');
			que.pop_front();
			ways.pop_front();
		}
	}

	bool ProcessPerm( char c )
	{
		int dx, dy;
		switch(c) {
			case 'U':
				dx = 0;
				dy = -1;
				break;
			case 'D':
				dx = 0;
				dy = 1;
				break;
			case 'L':
				dx = -1;
				dy = 0;
				break;
			case 'R':
				dx = 1;
				dy = 0;
				break;
			default:
				massert(false, "Unreachable code");
		}
		std::vector<char> n = pos.GetPerm(dx, dy);
		if (n != std::vector<char>()) {
			if (set.find(n) == set.end()) {
				set.insert(n);
				que.push_back(n);
				way1 = ways.front();
				way1.push_back(c);
				ways.push_back(way1);
				way1.clear();
			}
		}
		if (n == pos.correct) {
			way2 = ways.back();
			return true;
		}
		return false;
	}

	void clear()
	{
		way1.clear();
		way2.clear();
		set.clear();
		que.clear();
		ways.clear();
	}
};

class Solver15Tester
{
public:
	void Test(int n)
	{
		Solver15 solver;
		genSequence(n);
		/*massiv = {1, 2, 0, 4, 5, 3, 7, 8, 6};
		zero_pos = 2;
		base = 3;
		num = 9;*/
		solver.SetInput(massiv);
		solver.Process();
		std::vector<char> ans = solver.GetOutput();
		solver.Print();
		std::vector<char> correct = solver.GetCorrectAnswer();
		std::vector<char> answer = checkCorrectness(ans);

		if ( correct == answer ) {
			std::cout << "Correct";
		} else {
			std::cout << "Incorrect";
		}
	}

private:
	std::vector<char> massiv;
	int zero_pos;
	int base;
	int num;

	void genSequence(int _base)
	{
		base = _base;
		num = base*base;
		zero_pos= num-1;
		for (int i = 1; i<num; ++i) {
			massiv.push_back( i );
		}
		massiv.push_back(0);
		for (int i = 0; i<100; ++i) {
			int dx, dy;
			if ( rand()%2 ) {
				dx = 0;
				dy = rand()%2==0?1:-1;
			} else {
				dx = rand()%2==0?1:-1;
				dy = 0;
			}
			if (zero_pos +dx +base*dy >= num || zero_pos +dx +base*dy<0) {
				continue;
			}
			std::swap(massiv[zero_pos], massiv[zero_pos +dx +base*dy]);
			zero_pos += dx +base*dy;
		}
	}

	std::vector<char>& checkCorrectness(std::vector<char>& ans)
	{
		for (int i = 0; i<ans.size(); ++i) {
			int dx, dy;
			switch( ans[i] ) {
				case 'U':
					dx = 0;
					dy = -1;
					break;
				case 'D':
					dx = 0;
					dy = 1;
					break;
				case 'L':
					dx = -1;
					dy = 0;
					break;
				case 'R':
					dx = 1;
					dy = 0;
					break;
				default:
					massert( false, "Unreachable code" );
			}
			std::swap(massiv[zero_pos], massiv[zero_pos +dx +base*dy]);
			zero_pos += dx +base*dy;
		}
		for (int i = 0; i<num; ++i) {
			if (i %base == 0) {
				printf("\n");
			}
			printf("%2d ", massiv[i]);
		}
		printf("\n");
		return massiv;
	}
};

void TestSolver15()
{
	Solver15Tester tester;
	tester.Test(4);
}