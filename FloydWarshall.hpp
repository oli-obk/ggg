#import "AllPairShortestPath.hpp"
#import <map>
#import <set>

class FloydWarshall : public AllPairShortestPath {

public:
	
	void run(const Graph& g) override;

	/**
	* Get the shortest paths between nodes u and v.
	*/
	std::vector<Path> getPath(NodePtr u, NodePtr v) const override;
	
	void printDistanceMatrix() const;

private:
	std::map<NodePtr, std::map<NodePtr, double> > dist;
	std::map<NodePtr, std::map<NodePtr, std::set<NodePtr> > > next;

};
