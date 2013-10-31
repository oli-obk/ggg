#import "Betweenness.hpp"


void Betweenness::run(const Graph& g) {
	// run APSP algorithm
	this->apsp->run(g);

	// now count the paths
	size_t paths = 0;
	for (auto u: g.getNodes()) {
		size_t uPaths = 0; // number of shortest s-t-paths which go trhough u
		for (auto s: g.getNodes()) {
			for (auto t: g.getNodes()) {
				if (t != u && t != u) {
					std::vector<NodePtr> sp = this->apsp->getPath(s, t);
					assert (sp.size() > 0);
					paths += 1;
					if (std::find(sp.begin(), sp.end(), u) != sp.end()) {
						uPaths += 1;
					};
				}
			}
		}
		betweenness[u] = uPaths / (double) paths;
	}

}


double Betweenness::getValue(NodePtr u) {
	return betweenness[u];
}

