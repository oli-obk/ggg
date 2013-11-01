#import "Graph.hpp"

class Layout {

public:
	/**
	* Layout the graph by writing positions into the nodes.
	**/
	virtual void run(Graph& g) = 0;

};