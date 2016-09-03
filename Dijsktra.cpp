#include "stdafx.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <map>
#include <vector>
#include <queue>
#include <forward_list>
#include <climits>
using namespace std;
namespace myGraph {
	//Indexed min priority queue, generally I will have this in different file but hacker rank needs the code 
	//in one single file
	template<typename KeyType, typename compare = less<KeyType>, class Hash = hash<KeyType>>
	class minPriorityQ {
	public:
		typedef unsigned int _uInt;
		minPriorityQ() {
			_heap.push_back(KeyType());//insert at 0th position so that actual elements start at 1
		}
		minPriorityQ(const vector<KeyType>& dataArray) {
			_heap.push_back(KeyType());//insert at 0th position so that actual elements start at 1
			for (KeyType Key : dataArray) {
				push(Key);
			}
		}
		/*
		Inserts the element at the end and floats it up
		*/
		void push(KeyType key) {
			_heap.push_back(key);
			_keyIndex[key] = _heap.size() - 1;//-1 as we are doing post insertion
			swim_up(_heap.size() - 1);
		}
		KeyType min() {
			if (_heap.size() < 2) {//One is the element at 0th position which should not be considered
				throw("Underflow error");
			}
			return _heap.at(1);
		}
		/*
		Removes the minimum element
		Don't return the element to provide strong guarntee
		If the function returns the template type and the user defined type
		raises exception while coping(during the return statement) the heap
		will be left in bad state (element removed but not returned)
		*/
		void extractMin() {
			if (_heap.size() < 2) {//One is the element at 0th position which should not be considered
				throw("Underflow error");
			}
			//First swap the first and last element
			KeyType minK = min();
			swap_elements(1, _heap.size() - 1);
			_keyIndex.erase(minK);

			_heap.pop_back();
			//restore the min heap variant
			sink_down(1);
		}
		/*
		changes the key if it is lesser and restores the min priority queue invariant
		*/
		bool decreaseKey(KeyType oldKey, KeyType newKey) {
			if (_keyIndex.find(oldKey) == _keyIndex.end())
				return false;

			_uInt elementIndex = _keyIndex[oldKey];
			if (_heap.at(elementIndex) < newKey)
				return false;

			//delete the earlier index and insert the new key
			_keyIndex.erase(_heap.at(elementIndex));
			_keyIndex[newKey] = elementIndex;

			//Insert and Restore the priority queue in-variant
			_heap.at(elementIndex) = newKey;
			swim_up(elementIndex);
			return true;
		}
		/*
		changes the key if it is lesser and restores the min priority queue invariant
		*/
		bool decreaseKey(KeyType Key) {
			if (_keyIndex.find(Key) == _keyIndex.end())
				return false;

			_uInt elementIndex = _keyIndex[Key];
			if (_heap.at(elementIndex) < Key)
				return false;

			//delete the earlier index and insert the new key
			_keyIndex.erase(_heap.at(elementIndex));
			_keyIndex[Key] = elementIndex;

			//Insert and Restore the priority queue in-variant
			_heap.at(elementIndex) = Key;
			swim_up(elementIndex);
			return true;
		}
		size_t size() {
			return _heap.size();
		}
		bool empty() {
			return !(_heap.size() > 1);
		}

	private:
		//Data strucutre which holds the keys
		vector<KeyType> _heap;
		//Data strucutre to hold the index of the keys in the heap
		//To help speedup the decrease/increase key operations in priority queue
		unordered_map<KeyType, size_t, Hash> _keyIndex;

		unsigned int parent(_uInt elementIndex) {
			if (_heap.size() > 2)
				return elementIndex / 2;
			else
				return 1;
		}
		unsigned int left(_uInt elementIndex) {
			return 2 * elementIndex;
		}
		unsigned int right(_uInt elementIndex) {
			return 2 * elementIndex + 1;
		}
		void swap_elements(_uInt index1, _uInt index2) {
			_keyIndex[_heap.at(index1)] = index2;
			_keyIndex[_heap.at(index2)] = index1;
			std::swap(_heap.at(index1), _heap.at(index2));
		}
		/*
		Takes the element's index as input which needs to be put in right place
		Keeps moving the element down until the childern's are bigger
		*/
		void sink_down(_uInt elementIndex) {
			_uInt leftIndex = left(elementIndex);
			_uInt rightIndex = right(elementIndex);
			_uInt smallestIndex = leftIndex;
			while (leftIndex <= _heap.size() - 1 && _heap.at(leftIndex) < _heap.at(elementIndex) ||
				rightIndex <= _heap.size() - 1 && _heap.at(rightIndex) < _heap.at(elementIndex)) {

				//First check if left side is smallest
				if (leftIndex <= _heap.size() - 1 && _heap.at(leftIndex) < _heap.at(elementIndex))
					smallestIndex = leftIndex;
				else
					smallestIndex = elementIndex;

				//check the smallest against right element
				if (rightIndex <= _heap.size() - 1 && _heap.at(rightIndex) < _heap.at(smallestIndex))
					smallestIndex = rightIndex;

				swap_elements(elementIndex, smallestIndex);
				leftIndex = left(smallestIndex);
				rightIndex = right(smallestIndex);
				elementIndex = smallestIndex;
			}
		}
		/*
		Takes the element's index as input which needs to be put in right place
		Keeps moving the element up until the parent is lesser
		*/
		void swim_up(_uInt elementIndex) {
			_uInt parentIndex = parent(elementIndex);
			while (_heap[elementIndex] < _heap[parentIndex]) {
				swap_elements(elementIndex, parentIndex);
				elementIndex = parentIndex;
				parentIndex = parent(elementIndex);
			}
		}
	};

	//Graph related stuff
	enum class vertexStates { UNDISCOVERED, DISCOVERED, PROCESSED };
	struct edge {
		int toNode_;
		int weight_;
		edge(int t, int w) :toNode_{ t }, weight_{ w } {}
	};
	class Graph {
	public:
		unordered_map<int, int> vertices_;
		unordered_map<int, std::vector<edge>> adjList_;
		Graph(bool isDir = false) :isDirected{ isDir } {}
		void insert_vertex(int x) {
			if (vertices_.find(x) == vertices_.end()) {
				vertices_[x] = x;
			}
		}
		void insert_edge(int from, int to, int weight = 1) {
			//map takes care of creating new list and adding if not already present
			adjList_[from].emplace_back(to, weight);
			if (!isDirected)
				adjList_[to].emplace_back(from, weight);
		}

	private:
		bool isDirected;
	};

	//This class's object will be elements of the pq
	//we need vertex name and current dist from source as elements
	//the vertex number will be used for hash generation and distance for comparision
	class PQElement {
	public:
		int vNum_;
		int dist_;
		PQElement(int v = INT_MIN, int d = INT_MIN) :vNum_{ v }, dist_{ d } {}
		bool operator < (const PQElement& rhs) {
			return dist_ < rhs.dist_ ? true : false;
		}
		bool operator == (const PQElement& rhs) const {
			return vNum_ == rhs.vNum_ ? true : false;
		}
	};
	struct hash_PQElement {
		//This is used by the hash map of the priority queue
		//And we want to hash it with the vertex number so that
		//while updating the keys in the PQ it can find the element quickly by given vertex
		size_t operator()(const PQElement& d) const {
			return std::hash<int>()(d.vNum_);
		}
	};
	//returns distance to all the nodes(ordered by node number)
	//for an unreachable node distance is -1
	map<int, int> DijkstraShortestPath(Graph& g, int source) {
		map<int, int> distance;
		//queue to process the nearest first
		myGraph::minPriorityQ<PQElement, less<PQElement>, hash_PQElement> pq;
		std::for_each(g.vertices_.begin(), g.vertices_.end(),
			[&pq, &distance](auto& v) //Lambda function
		{
			pq.push(PQElement(v.first, INT_MAX));
			distance[v.first] = INT_MAX;//all are unreachable at the start
		});

		//setup the source
		pq.decreaseKey(PQElement(source, 0));
		distance[source] = 0;

		while (!pq.empty()) {
			PQElement currentNode = pq.min();
			pq.extractMin();
			if (currentNode == 88)
				int j = 0;

			auto adjListIter = g.adjList_.find(currentNode.vNum_);
			if (adjListIter != g.adjList_.end()) {
				//To completely explore a vertex v, we must evaluate each edge leaving currVertex
				for (auto& e : adjListIter->second) {
					//distance from source till now and new edge weight if it lesser update the queue
					if (distance[currentNode.vNum_] != INT_MAX && pq.decreaseKey(PQElement(e.toNode_, distance[currentNode.vNum_] + e.weight_))) {
						distance[e.toNode_] = distance[currentNode.vNum_] + e.weight_;
					}
				}
			}
		}

		//remove source
		distance.erase(source);
		return distance;
	}
}
/*
1 --number of test cases
4 4 --Nodes Edges
1 2 24 --Edge from,to,weight
1 4 20
3 1 3
4 3 12
1 --Source Node
*/
int main()
{
	std::ios::sync_with_stdio(false);
	int T = 0; //number of test cases
	cin >> T;
	vector<map<int, int>> op;
	while (T-- > 0)
	{
		myGraph::Graph g(false);
		int N = 0; int M = 0;
		cin >> N >> M;

		while (M-- > 0) {
			int f = 0;
			int t = 0;
			int w = 0;
			cin >> f >> t >> w;
			g.insert_vertex(f);
			g.insert_vertex(t);
			g.insert_edge(f, t, w);
		}
		int S = 0;
		cin >> S;
		map<int, int> distance = myGraph::DijkstraShortestPath(g, S);
		op.push_back(distance);
	}
	for (auto& singleV : op) {
		std::for_each(singleV.begin(), singleV.end(),
			[](auto a)
		{
			if (a.second == INT_MAX)
				cout << -1 << " ";
			else
				cout << a.second << " ";
		});
		cout << endl;
	}

	return 0;
}