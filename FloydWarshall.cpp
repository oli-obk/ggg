#import "FloydWarshall.hpp"
#import "Node.hpp"
#import "Edge.hpp"
#import <iostream>

void FloydWarshall::run(const Graph& g) {

		// run Floyd-Warshall all pair shortest path algorithm
	size_t n = g.getNodeCount();

	// initialize all distances to infinity
	for (auto u: g.getNodes()) {
		for (auto v: g.getNodes()) {
			dist[u][v] = inf;
		}
	}

	// do not initialize next

	// nodes have distance 0 to themselves
	for (auto v: g.getNodes()) {
		dist[v][v] = 0.0;
	}


	// neighbors have distance 1
	for (auto uv: g.getUndirectedEdges()) {
		NodePtr u = uv->getSource();
		NodePtr v = uv->getTarget();
		dist[u][v] = 1.0;
		dist[v][u] = 1.0;
	}

	// main part of algorithm
	for (auto k: g.getNodes()) {
		for (auto i: g.getNodes()) {
			for (auto j: g.getNodes()) {
				if (dist[i][k] + dist[k][j] < dist[i][j]) {
					dist[i][j] = dist[i][k] + dist[k][j];
					next[i][j] = k; // needed to reconstruct path
				}
			}
		}
	}

}


std::vector<NodePtr> FloydWarshall::getPath(NodePtr u, NodePtr v) const {
	if (dist.at(u).at(v) == inf) {
		std::vector<NodePtr> empty;
		return empty;
	} 

	try {
		auto intermediate = next.at(u).at(v);
		auto prefix = getPath(u, intermediate);
		prefix.push_back(intermediate);
		auto suffix = getPath(intermediate, v);
		std::copy(std::begin(suffix), std::end(suffix), std::back_inserter(prefix));
		return prefix;
	} catch (const std::out_of_range& ex) {
		// intermediate does not exist
		std::vector<NodePtr> direct = {u, v};
		return direct;
	}
}


void FloydWarshall::printDistanceMatrix() const
{
    for (auto& x : dist) {
        for (auto& y : x.second) {
            if (y.second == inf) {
                std::cout << "inf ";
            } else {
                std::cout << y.second << "\t";
            }
        }
        std::cout << std::endl;
    }
}

