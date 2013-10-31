#import <iostream>

#import "Graph.hpp"
#import "Node.hpp"
#import "Edge.hpp"

void testGraph() {
	Graph G;
	auto u = G.createNode(Position(0, 0));
	auto v = G.createNode(Position(1, 1));
	u->connect(v);
	std::cout << "number of nodes: " << G.getNodeCount() << std::endl;
}

int main() {
	std::cout << "Test" << std::endl;
	testGraph();
}
