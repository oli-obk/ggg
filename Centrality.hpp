#import "Graph.hpp"
#import "Algorithm.hpp"

class Centrality : public Algorithm {
public:

	/**
	* Get the centrality value for a node. Run method must have been called before.
	*/
	virtual double getValue(NodePtr u) = 0;
};
