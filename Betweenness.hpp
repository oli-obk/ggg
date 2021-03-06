#import "Centrality.hpp"
#import "AllPairShortestPath.hpp"

#import <map>

class Betweenness : public Centrality
{
public:

	void run(const Graph& g) override;

	double getValue(NodePtr u) override;

private:

	std::map<NodePtr, double> betweenness;

};
