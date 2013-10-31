#import "ForwardDeclarations.hpp"

#import <limits>
#import <algorithm>

constexpr double inf = std::numeric_limits<double>::max();

/**
* Abstract base class for graph algorithms.
*/
class Algorithm {
public:
	virtual void run(const Graph& g) = 0;

};
