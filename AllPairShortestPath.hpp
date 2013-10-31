#import "Algorithm.hpp"

class AllPairShortestPath : public Algorithm {
public:

	/**
	* Get the shortest path between nodes u and v.
	*/
	virtual std::vector<NodePtr> getPath(NodePtr u, NodePtr v) const = 0;

};
