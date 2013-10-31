#import "Betweenness.hpp"
#import "FloydWarshall.hpp"
#import <iostream>
#import "Node.hpp"
#import "Edge.hpp"

void Betweenness::run(const Graph& g) {
	// run APSP algorithm
	FloydWarshall fw;
	fw.run(g);

	// now count the paths
	size_t paths = 0;
	for (auto u: g.getNodes()) {
		size_t uPaths = 0; // number of shortest s-t-paths which go trhough u
		for (auto s: g.getNodes()) {
			for (auto t: g.getNodes()) {
				if (t != u && s != u && s != t) {
					std::vector<Path> sps = fw.getPath(s, t);
                    //std::cout << "paths through " << u->getId() << " from " << s->getId() << " -> " << t->getId() << std::endl;
					if (sps.empty()) continue;
					if (sps.front().size() < 3) continue;
					for (auto sp : sps) {
					    assert(!sp.empty());
					    if (std::find(sp.begin(), sp.end(), u) != sp.end()) {
						    uPaths += 1;
						    /*
                            for (auto node : sp) {
                                std::cout << node->getId() << "\t";
                            }
                            std::cout << std::endl;
                            */
					    };
					}
				}
			}
		}
		betweenness[u] = uPaths;
		paths = std::max(paths, uPaths);
	}
	if (paths != 0) {
	    for (auto u : g.getNodes()) {
	        betweenness[u] /= double(paths);
	    }
	}

}


double Betweenness::getValue(NodePtr u) {
	return betweenness[u];
}

