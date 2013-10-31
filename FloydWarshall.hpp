#import "AllPairShortestPath.hpp"

class FloydWarshall : public AllPairShortestPath {

public:
	
	void run(const Graph& g) override;

	/**
	* Get the shortest path between nodes u and v.
	*/
	std::vector<NodePtr> getPath(NodePtr u, NodePtr v) const override;
	
	void printDistanceMatrix() const;

private:
	std::map<NodePtr, std::map<NodePtr, double> > dist;
	std::map<NodePtr, std::map<NodePtr, NodePtr> > next;

};
