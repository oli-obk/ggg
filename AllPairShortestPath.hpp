#import "Algorithm.hpp"
#import <vector>

typedef std::vector<NodePtr> Path;

class AllPairShortestPath : public Algorithm {
public:

	/**
	* Get the shortest paths between nodes u and v.
	*/
	virtual std::vector<Path> getPath(NodePtr u, NodePtr v) const = 0;

};
