
#include <iostream>
#include <thread>
#include <atomic>
#include <array>
#include <vector>
#include <queue>
#include <limits.h>
#include <stdio.h>
#include <algorithm>
class Network {


	static const int MYNULL = -1;
	struct MyEdge {
		short int from;
		short int to;
		short int capacity = 0;
		short int flow = 0;
		short int previous_edge = MYNULL;
		short int next_edge = MYNULL;

		MyEdge(int f, int t, int c) {
			from = f;
			to = t;
			capacity = c;
		}
	};

	struct MyVertex {
		int first_edge = MYNULL;
	};

	std::vector <MyEdge> allEdges;
	std::vector <MyVertex> allVertexes;
	std::vector <short int> iteratorBegins;
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
		NetworkEdgesIterator& operator ++ () {
			if (position != MYNULL)
				position = myNetwork->allEdges[position].next_edge;
			return *this;
		}
		NetworkEdgesIterator& operator ++ (int) {
			if (position != MYNULL)
				position = myNetwork->allEdges[position].next_edge;
			return *this;
		}

		bool operator == (const NetworkEdgesIterator &other) {
			return (position == other.position);
		}
		bool operator != (const NetworkEdgesIterator &other) {
			return (position != other.position);
		}

		bool isBackEdge() {
			return position % 2;
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

	void NetworkResize(int network_size) {
		nw_size = network_size;
		allVertexes.resize(nw_size);
		edges_count = 0;
		iteratorBegins.resize(nw_size, -1);
	}
	void clearIterator() {
		for (int i = 0; i < nw_size; i++) {
			iteratorBegins[i] = allVertexes[i].first_edge;
		}
	}
	~Network() {}

private:

	void subInsertEdge(size_t new_from, size_t new_to, int new_capacity) {

		MyEdge NewEdge(new_from, new_to, new_capacity);
		int tmp = allVertexes[new_from].first_edge;
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
		}
		else {
			return number - 1;
		}
	}

};
class OptimalFind {
	std::vector <int> distance;
	size_t vertex_count;
	int N;
	int M;
	int source = 0;
	int target = 1;
	int e_count = 0;
	int max_flow = 0;
	Network myNetwork;
	std::vector< short int> myMap;
	std::vector <short int> numb;
public:
	OptimalFind() {}
	~OptimalFind() {}

	void findAnswer() {
		readNetwork();
		int a = findMaxFlow();
		if (source == target) {
			printf("%i", -1);
		}
		else {
			if (a >= 3000) {
				printf("%i", -1);
			}
			else {
				if (a > 0) {
					printAnswer();
				}
				else {
					printf("%i", 0);
				}

			}
		}
	}
private:
	int getC(int a, int b) {
		if ((a == 2) || (b == 2)) {
			return 1;
		}
		else {
			return 3000;
		}
	}
	int readCoordinate() {
		int a, b;
		scanf("%i %i", &a, &b);
		//std::cin >> a >> b;
		a--;
		b--;
		return a*M + b;

	}
	bool bfs() {
		std::queue <int> myQueue;
		myNetwork.clearIterator();
		distance.assign(vertex_count, INT_MAX);
		distance[source] = 0;
		myQueue.push(source);
		while (!myQueue.empty()) {

			int u = myQueue.front();
			myQueue.pop();
			Network::NetworkEdgesIterator myIt = myNetwork.rbegin(u);
			while (!myIt.isNull()) {
				if ((myIt.getResidualCapacity() > 0) && (distance[myIt.getFinish()] == INT_MAX)) {
					distance[myIt.getFinish()] = distance[u] + 1;
					myQueue.push(myIt.getFinish());
				}
				myIt++;
			}
		}
		return distance[target] != INT_MAX;
	}


	void dop_bfs(int s) {
		std::queue <int> myQueue;
		myNetwork.clearIterator();
		distance.assign(vertex_count, INT_MAX);
		distance[s] = 0;
		numb[s] = s;
		myQueue.push(s);
		while (!myQueue.empty()) {
			int p = myQueue.front();
			myQueue.pop();
			if ((p - M >= 0) && (myMap[p - M] == 1)) {
				if (distance[p - M] == INT_MAX) {
					distance[p - M] = 1;
					if (myMap[p - M] == 1) {
						myQueue.push(p - M);
						numb[p - M] = s;
					}
				}
			}
			if ((p + M< vertex_count) && (myMap[p + M] == 1)) {
				if (distance[p + M] == INT_MAX) {
					distance[p + M] = 1;
					if (myMap[p + M] == 1) {
						myQueue.push(p + M);
						numb[p + M] = s;
					}
				}
			}
			if ((p + 1 <vertex_count) && (myMap[p + 1] == 1)) {
				if (distance[p + 1] == INT_MAX) {
					distance[p + 1] = 1;
					if (myMap[p + 1] == 1) {
						myQueue.push(p + 1);
						numb[p + 1] = s;
					}
				}
			}
			if ((p - 1 >= 0) && (myMap[p - 1] == 1)) {
				if (distance[p - 1] == INT_MAX) {
					distance[p - 1] = 1;
					if (myMap[p - 1] == 1) {
						myQueue.push(p - 1);
						numb[p - 1] = s;
					}
				}
			}


		}
	}




	int dfs(int u, int minC) {

		if ((u == target) || (minC == 0)) {
			return minC;
		}
		int delta;
		Network::NetworkEdgesIterator myIt = myNetwork.rbegin(u);
		while (!myIt.isNull()) {

			if (distance[myIt.getFinish()] == distance[u] + 1) {
				delta = dfs(myIt.getFinish(), std::min(minC, myIt.getResidualCapacity()));
				if (delta != 0) {
					myIt.pushFlow(delta); // íàñûùàåì ðåáðà ïî ïóòè dfs
					return delta;
				}
			}
			myNetwork.throwFirstEdge(myIt.getStart());
			myIt++;
		}
		return 0;
	}
	void readNetwork() {
		int k, l, a, b;
		scanf("%i %i", &N, &M);
		//std::cin >> N >> M;
		vertex_count = N * M;
		myNetwork.NetworkResize(N * M);
		myMap.resize(vertex_count, 1);
		numb.resize(vertex_count, -1);
		scanf("%i %i", &k, &l);
		//std::cin >> k >> l;
		for (int i = 0; i < k; i++) {
			myMap[readCoordinate()] = 0;
		}
		for (int i = 0; i < l; i++) {
			myMap[readCoordinate()] = 2;
		}
		source = readCoordinate();
		target = readCoordinate();
		for (int i = 0; i < vertex_count; i++) {
			if (myMap[i] == 2) {
				numb[i] = i;
			}
			else {
				if ((myMap[i] == 1) && (numb[i] == -1))
					dop_bfs(i);
			}

		}
		source = numb[source];
		target = numb[target];
		int p = -1;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				p++;
				if ((myMap[p] != 0)) {
					if ((i != 0) && (myMap[p - M] != 0) && (numb[p] != numb[p - M])) {
						myNetwork.insertEdge(numb[p], numb[p - M], 1);
					}
					if ((i != N - 1) && (myMap[p + M] != 0) && (numb[p] != numb[p + M])) {
						myNetwork.insertEdge(numb[p], numb[p + M], 1);
					}
					if ((j != M - 1) && (myMap[p + 1] != 0) && (numb[p] != numb[p + 1])) {
						myNetwork.insertEdge(numb[p], numb[p + 1], 1);
					}
					if ((j != 0) && (myMap[p - 1] != 0) && (numb[p] != numb[p - 1])) {
						myNetwork.insertEdge(numb[p], numb[p - 1], 1);
					}
				}

			}
		}
		distance.resize(vertex_count, INT_MAX);
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


	void printAnswer() {
		std::queue <int> myQueue;
		std::vector <bool> used(vertex_count, 0);
		std::vector <bool> ans(vertex_count, 0);
		myQueue.push(source);
		while (!myQueue.empty()) {
			int u = myQueue.front();
			myQueue.pop();
			Network::NetworkEdgesIterator myIt = myNetwork.begin(u);
			used[u] = 1;
			while (!myIt.isNull()) {
				if ((distance[myIt.getFinish()] == INT_MAX)) {
					if (myMap[u] != 1) {
						ans[u] = 1;
					}
					else {
						ans[myIt.getFinish()] = 1;
					}
				}
				else {
					if ((distance[myIt.getFinish()] != INT_MAX) && (!used[myIt.getFinish()]))
						myQueue.push(myIt.getFinish());
				}
				myIt++;
			}
		}

		int res = 0;
		for (int i = 0; i < vertex_count; i++) {
			res += ans[i];
		}
		printf("%i\n", res);
		for (int i = 0; i < vertex_count; i++) {
			if (ans[i]) {
				int a = i / M + 1;
				int b = i % M + 1;
				printf("%i %i\n", a, b);
			}
		}
	}
};

int main() {
	OptimalFind myAl;
	myAl.findAnswer();

	return 0;
}
