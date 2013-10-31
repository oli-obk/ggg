#import "FloydWarshall.hpp"
#import "Node.hpp"
#import "Edge.hpp"
#import "Graph.hpp"
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
		    if (i == k) continue;
			for (auto j: g.getNodes()) {
			    if (i == j) continue;
			    if (j == k) continue;
			    auto d = dist[i][k] + dist[k][j];
				if (d < dist[i][j]) {
					dist[i][j] = d;
					next[i][j].clear();
					next[i][j].insert(k); // needed to reconstruct path
				} else if (d == dist[i][j]) {
				    if (!next[i][j].empty()) {
				        next[i][j].insert(k);
				    }
				}
			}
		}
	}

}

std::vector<Path> FloydWarshall::getPath(NodePtr u, NodePtr v) const {
	if (dist.at(u).at(v) == inf) {
		return std::vector<Path>();
	}
	
	std::cout << std::endl;
	for (auto u_ : next) {
    	std::cout << u_.first.get() << std::endl;
	    for (auto v_ : u_.second) {
	        std::cout << "\t" << v_.first.get() << "\t";
	        for (auto node : v_.second) {
	            std::cout << "\t" << node.get();
	        }
	        std::cout << std::endl;
	    }
	}

	try {
		std::vector<Path> prefixes;
		for (auto intermediate : next.at(u).at(v)) {
		    std::cout << u.get() << " -> " << intermediate.get() << std::endl;
        	for (auto suffix : getPath(intermediate, v)) {
    		    Path prefix({u, intermediate});
        		std::copy(std::begin(suffix), std::end(suffix), std::back_inserter(prefix));
        		prefixes.push_back(prefix);
        	}
		}
		return prefixes;
	} catch (const std::out_of_range& ex) {
		// intermediate does not exist
		std::vector<Path> direct = {{u, v}};
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

