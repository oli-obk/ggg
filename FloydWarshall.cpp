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
	    //std::cout << "k = " << k->getId() << std::endl;
		for (auto i: g.getNodes()) {
		    //std::cout << "\ti = " << i->getId() << std::endl;
			for (auto j: g.getNodes()) {
    		    //std::cout << "\t\tj = " << j->getId() << std::endl;
			    auto d = dist[i][k] + dist[k][j];
    		    //std::cout << "\t\t\t" << d << " = " << dist[i][k] << " + " << dist[k][j] << std::endl;
				if (d < dist[i][j]) {
        		    //std::cout << "\t\t\t" << d << " < " << dist[i][j] << std::endl;
					dist[i][j] = d;
				}
			}
		}
	}
	
	
	for (auto from: g.getNodes()) {
		for (auto to: g.getNodes()) {
		    if (from == to) continue;
		    if (dist[from][to] == inf) continue;
		    if (dist[from][to] == 1) continue;
		    double mind = inf;
			for (auto over: g.getNodes()) {
			    if (from == over) continue;
			    if (dist[from][over] != 1) continue;
			    auto d = dist[over][to];
			    if (d < mind) {
			        mind = d;
			        next[from][to] = {over};
			    } else if (d == mind) {
			        next[from][to].insert(over);
			    }
			}
		}
	}
	/*
	std::cout << std::endl;
	for (auto u_ : next) {
    	std::cout << u_.first->getId() << std::endl;
	    for (auto v_ : u_.second) {
	        std::cout << "\t" << v_.first->getId() << "\t";
	        for (auto node : v_.second) {
	            std::cout << "\t" << node->getId();
	        }
	        std::cout << std::endl;
	    }
	}
	assert(false);
	*/
}

std::vector<Path> FloydWarshall::getPath(NodePtr u, NodePtr v) const {
	if (dist.at(u).at(v) == inf) {
		return std::vector<Path>();
	}
	if (u == v) {
	    throw std::runtime_error("knoten haben keine pfade zu sich selbst");
	}
	
	/*
	std::cout << std::endl;
	for (auto u_ : next) {
    	std::cout << u_.first->getId() << std::endl;
	    for (auto v_ : u_.second) {
	        std::cout << "\t" << v_.first->getId() << "\t";
	        for (auto node : v_.second) {
	            std::cout << "\t" << node->getId();
	        }
	        std::cout << std::endl;
	    }
	}
	*/

	try {
		std::vector<Path> prefixes;
		for (auto intermediate : next.at(u).at(v)) {
		    //std::cout << u.get() << " -> " << intermediate.get() << std::endl;
        	for (auto suffix : getPath(intermediate, v)) {
    		    Path prefix({u});
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
    std::cout << "\t";
    for (auto& x : dist) {
        std::cout << x.first->getId() << "\t";
    }
    std::cout << std::endl;
    for (auto& x : dist) {
        std::cout << x.first->getId() << "\t";
        for (auto& y : x.second) {
            if (y.second == inf) {
                std::cout << "inf\t";
            } else {
                std::cout << y.second << "\t";
            }
        }
        std::cout << std::endl;
    }
}

