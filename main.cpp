#include "stdafx.h"
#define MY_CIN_REDIR


#include <iostream>
#include <thread>
#include <atomic>
#include <array>
#include <vector>
#include <queue>
#include <limits.h>

class Network {


	static const int MYNULL = -1;

	struct MyEdge {
		int from;
		int to;
		int capacity;
		int flow = 0;
		int previous_edge = MYNULL;
		int next_edge = MYNULL;
		int marked = 0;

		MyEdge(int f, int t, int c) {
			from = f;
			to = t;
			capacity = c;
		}
	};

	struct MyVertex {
		int first_edge = MYNULL;
		int last_edge = MYNULL;
	};

	std::vector<MyEdge> allEdges;
	std::vector<MyVertex> allVertexes;
	std::vector<int> iteratorBegins;
	size_t nw_size = 0;
	size_t edges_count = 0;
	size_t source;
	size_t target;


public:

	class NetworkEdgesIterator {
		size_t EdgeNum;
		Network* myNetwork;

		int vertex;
	public:
		int position;
	public:
		//friend class Network;
		NetworkEdgesIterator() {}

		NetworkEdgesIterator(Network* newNetwork) {
			myNetwork = newNetwork;
		}
		NetworkEdgesIterator(Network* newNetwork, int new_vertex, int new_position) {
			myNetwork = newNetwork;
			position = new_position;
			vertex = new_vertex;
		}

		void pushFlow(int flow_inc) {
			myNetwork->allEdges[position].flow += flow_inc;
			myNetwork->allEdges[myNetwork->backEdge(position)].flow -= flow_inc;
		}

		MyEdge getEdge() const {
			return myNetwork->allEdges[position];
		}

		int getStart() const {
			return myNetwork->allEdges[position].from;
		}
		int getFinish() const {
			return myNetwork->allEdges[position].to;
		}
		int getCapacity() const {
			return myNetwork->allEdges[position].capacity;
		}
		int getFlow() const {
			return myNetwork->allEdges[position].flow;
		}
		int getResidualCapacity() const {
			return myNetwork->allEdges[position].capacity - myNetwork->allEdges[position].flow;
		}
		bool isNull() {
			return (position == MYNULL);
		}
		NetworkEdgesIterator& operator++() {
			if (position != MYNULL)
				position = myNetwork->allEdges[position].next_edge;
			return *this;
		}
		NetworkEdgesIterator& operator++(int) {
			if (position != MYNULL)
				position = myNetwork->allEdges[position].next_edge;
			return *this;
		}
		NetworkEdgesIterator& operator--() {
			if (position != MYNULL)
				position = myNetwork->allEdges[position].previous_edge;
			return *this;
		}

		NetworkEdgesIterator& operator--(int) {
			if (position != MYNULL)
				position = myNetwork->allEdges[position].previous_edge;
			return *this;
		}
		bool operator==(const NetworkEdgesIterator& other) {
			return (position == other.position);
		}
		bool operator!=(const NetworkEdgesIterator& other) {
			return (position != other.position);
		}

		bool isBackEdge() {
			return position % 2;
		}

		void getMark() {
			myNetwork->allEdges[position].marked = 1;
		}
		void getBackMark() {
			myNetwork->allEdges[myNetwork->backEdge(position)].marked = 1;
		}
		bool isMarked() {
			return myNetwork->allEdges[position].marked;
		}
	};

	Network() {}

	Network(size_t network_size) {
		nw_size = network_size;
		allVertexes.resize(nw_size);
		iteratorBegins.resize(nw_size, MYNULL);
	}

	Network(size_t network_size, size_t s, size_t t) {
		nw_size = network_size;
		source = s;
		target = t;
		allVertexes.resize(nw_size);
		iteratorBegins.resize(nw_size, MYNULL);
	}

	void insertEdge(size_t new_from, size_t new_to, int new_capacity) {
		subInsertEdge(new_from, new_to, new_capacity);
		subInsertEdge(new_to, new_from, 0);
		//subInsertEdge(new_to, new_from, -new_capacity);
	}


	void throwFirstEdge(int vertex) {
		if (vertex != MYNULL) {
			if (iteratorBegins[vertex] != MYNULL) {
				iteratorBegins[vertex] = allEdges[iteratorBegins[vertex]].next_edge;
			}
		}
	}

	NetworkEdgesIterator rbegin(size_t vertex_number) {
		return NetworkEdgesIterator(this, vertex_number, iteratorBegins[vertex_number]);
	}
	NetworkEdgesIterator begin(size_t vertex_number) {
		return NetworkEdgesIterator(this, vertex_number, allVertexes[vertex_number].first_edge);
	}
	NetworkEdgesIterator end(size_t vertex_number) {
		return NetworkEdgesIterator(this, vertex_number, allVertexes[vertex_number].last_edge);
	}

	void NetworkResize(int network_size) {
		nw_size = network_size;
		allEdges.clear();
		allVertexes.resize(nw_size);
		edges_count = 0;
		iteratorBegins.clear();
		iteratorBegins.resize(nw_size, -1);
	}
	void clearIterator() {
		iteratorBegins.clear();
		iteratorBegins.resize(nw_size, -1);
		for (int i = 0; i < nw_size; i++) {
			iteratorBegins[i] = allVertexes[i].first_edge;
		}
	}
	~Network() {}

private:

	void subInsertEdge(size_t new_from, size_t new_to, int new_capacity) {

		MyEdge NewEdge(new_from, new_to, new_capacity);
		int tmp = allVertexes[new_from].first_edge;

		if (allVertexes[new_from].first_edge == MYNULL) {
			allVertexes[new_from].last_edge = edges_count;
		}

		allVertexes[new_from].first_edge = edges_count;
		NewEdge.next_edge = tmp;

		if (tmp != MYNULL) {
			allEdges[tmp].previous_edge = edges_count;
		}

		allEdges.push_back(NewEdge);
		edges_count++;
	}

	size_t backEdge(size_t number) {
		if (number % 2 == 0) {
			return number + 1;
		} else {
			return number - 1;
		}
	}

};

class BipartiteSubGraphFind {
	std::vector<int> distance;
	std::vector<bool> visited;
	std::vector<int> marks;
	size_t vertex_count;
	size_t bCount;
	size_t cGirls;
	size_t source;
	size_t target;
	size_t max_flow;
	Network net;

public:
	BipartiteSubGraphFind() {}
	~BipartiteSubGraphFind() {}

	void findBipartiteComponent() {
		readNetwork();
		if ((bCount != 0) && (cGirls != 0)) {
			max_flow = findMaxFlow();
			findWays();
			printResult();
		} else {
			if (bCount != 0) {
				std::cout << bCount << std::endl;
				std::cout << bCount << " " << 0 << std::endl;
				for (int i = 0; i < bCount; i++) {
					std::cout << i + 1 << " ";
				}
				std::cout << std::endl;
			} else {
				if (cGirls != 0) {
					std::cout << cGirls << std::endl;
					std::cout << 0 << " " << cGirls << std::endl;
					for (int i = 0; i < cGirls; i++) {
						std::cout << i + 1 << " ";
					}
					std::cout << std::endl;
				} else {
					std::cout << 0 << std::endl;
					std::cout << "0 0" << std::endl;
					std::cout << std::endl;
				}

			}

		}
	}
private:
	bool bfs() {
		std::queue<int> myQueue;
		distance.clear();
		net.clearIterator();
		distance.resize(vertex_count, INT_MAX);
		distance[source] = 0;
		myQueue.push(source);
		while (!myQueue.empty()) {

			int u = myQueue.front();
			myQueue.pop();
			Network::NetworkEdgesIterator myIt = net.rbegin(u);
			while (!myIt.isNull()) {
				if ((myIt.getResidualCapacity() > 0) && (distance[myIt.getFinish()] == INT_MAX)) {
					distance[myIt.getFinish()] = distance[u] + 1;
					myQueue.push(myIt.getFinish());
				}
				if (myIt.getResidualCapacity() == 0) {
				}
				myIt++;
			}
		}
		return distance[target] != INT_MAX;
	}

	int dfs(int u, int minC) {

		if ((u == target) || (minC == 0)) {
			return minC;
		}
		int delta;
		Network::NetworkEdgesIterator myIt = net.rbegin(u);
		while (!myIt.isNull()) {
			if (distance[myIt.getFinish()] == distance[u] + 1) {
				delta = dfs(myIt.getFinish(), std::min(minC, myIt.getResidualCapacity()));
				if (delta != 0) {
					myIt.pushFlow(delta); // íàñûùàåì ðåáðà ïî ïóòè dfs
					return delta;
				}
			}
			net.throwFirstEdge(myIt.getStart());
			myIt++;
		}
		return 0;
	}
	void readNetwork() {
		int a;
		std::cin >> bCount >> cGirls;
		vertex_count = bCount + cGirls + 2;
		net.NetworkResize(vertex_count);
		source = 0;
		target = 1;

		for (int i = 0; i < bCount; i++) {
			std::vector<bool> friends(cGirls, 1);
			std::cin >> a;
			while (a != 0) {
				friends[a - 1] = 0;
				std::cin >> a;
			}
			for (int j = 0; j < cGirls; j++) {
				if (friends[j]) {
					net.insertEdge(i + 2, bCount + 2 + j, 1);
				}
			}
		}
		for (int i = 0; i < bCount; i++) {
			net.insertEdge(0, i + 2, 1);
		}
		for (int i = 0; i < cGirls; i++) {
			net.insertEdge(bCount + 2 + i, 1, 1);
		}

	}
	int findMaxFlow() {
		int maxFlow = 0;
		int flow;
		while (bfs()) {
			flow = dfs(source, INT_MAX);
			while (flow != 0) {
				maxFlow += flow;
				flow = dfs(source, INT_MAX);
			}
		}

		return maxFlow;
	}

	void dfs(int u) {
		if (visited[u] == 0) {
			visited[u] = 1;
			Network::NetworkEdgesIterator myIt = net.begin(u);
			while (!myIt.isNull()) {
				if (!myIt.isMarked()) {
					dfs(myIt.getFinish());
				}
				myIt++;
			}
		}
	}

	void findWays() {
		visited.resize(vertex_count, 0);
		marks.resize(vertex_count, 0);
		visited[0] = 1;
		visited[1] = 1;

		for (int i = 2; i < bCount + 2; i++) {
			auto myIt = net.begin(i);
			while (myIt != net.end()) {
				if (myIt.getFlow() == 1) {
					marks[myIt.getStart()] = 1;
					myIt.getMark();
					//myIt.getBackMark();
				} else {
					myIt.getBackMark();
				}
				myIt++;
			}
		}
		for (int i = 2; i < bCount + 2; i++) {
			if ((marks[i] == 0) && (visited[i] == 0)) {
				dfs(i);
			}
		}
	}

	void printResult() {
		std::vector<int> boys;
		std::vector<int> girls;
		for (int i = 2; i < bCount + 2; i++) {
			if (visited[i]) {
				boys.push_back(i - 1);
			}
		}
		for (int i = bCount + 2; i < vertex_count; i++) {
			if (!visited[i]) {
				girls.push_back(i - bCount - 1);
			}
		}
		if (std::max(bCount, cGirls) <= boys.size() + girls.size()) {
			std::cout << boys.size() + girls.size() << std::endl;
			std::cout << boys.size() << " " << girls.size() << std::endl;
			for (int i = 0; i < boys.size(); i++) {
				std::cout << boys[i] << " ";
			}
			if (boys.size() != 0)std::cout << std::endl;
			for (int i = 0; i < girls.size(); i++) {
				std::cout << girls[i] << " ";
			}
			if (girls.size() != 0) std::cout << std::endl;
			std::cout << std::endl;
		} else {
			if (bCount > cGirls) {
				std::cout << bCount << std::endl;
				std::cout << bCount << " " << 0 << std::endl;
				for (int i = 0; i < bCount; i++) {
					std::cout << i + 1 << " ";
				}
				std::cout << std::endl;
			} else {

				std::cout << cGirls << std::endl;
				std::cout << 0 << " " << cGirls << std::endl;
				for (int i = 0; i < cGirls; i++) {
					std::cout << i + 1 << " ";
				}
				std::cout << std::endl;
			}
		}
	}
};

int main() {

	int n;
	std::cin >> n;

	for (int i = 0; i < n; i++) {
		BipartiteSubGraphFind myAl;
		myAl.findBipartiteComponent();
	}

	return 0;
}


/*int main()
{
	srand( static_cast<unsigned int>(time( 0 )));
#ifdef MY_CIN_REDIR
	std::ifstream ifs( "in.txt", std::ios_base::in );
	std::streambuf* cinbuf = std::cin.rdbuf();
	std::cin.rdbuf( ifs.rdbuf() );
#endif

	MutexTreeTester tester;
	for(int i = 0; i<4; ++i)
		tester.test(36, 10000);

#ifdef MY_CIN_REDIR
	std::cin.rdbuf( cinbuf );
#endif
	std::cin.get();
	return 0;
}
*/