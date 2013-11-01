#import "ForceDirected.hpp"
#import <map>

ForceDirected::ForceDirected(double repConst, double step, double scale, int iterations) : 
	repConst(repConst), step(step), scale(scale), iterations(iterations) {

}


void ForceDirected::run(Graph& g) {

	double springLength = pow(scale, 2) / g.getNodeCount();

	auto distance = [&](NodePtr u, NodePtr v) {
		return sqrt(pow(u->x - v->x, 2) + pow(u->y - v->y, 2));
	};

	auto attractiveForce = [&](NodePtr u, NodePtr v) {
		Node& _u = *u;
		Node& _v = *v;
		double d = distance(u, v);
		if (d < 0.000001) {
			d = 0.000001;
		}
		double value = d / springLength;
		Vector direction = _v - _u;
		Vector force = (direction / d) * value;
		return force;
	};


	auto repellingForce = [&](NodePtr u, NodePtr v) {
		Node& _u = *u;
		Node& _v = *v;
		double d = distance(u, v);
		if (d < 0.000001) {
			d = 0.000001;
		}
		double value = repConst / d;
		Vector direction = _u - _v;
		Vector force = (direction / d) * value;
		return force;
	};

	auto move = [&](NodePtr u, Vector force) {
		Node& _u = *u;
		_u = _u + force * step;
	};


	std::map<NodePtr, Vector> forces; // combined forces acting on a node
	for (NodePtr u : g.getNodes()) {
		forces[u] = Vector(0.0, 0.0);
	}

	for (NodePtr u : g.getNodes()) {
		for (NodePtr v: g.getNodes()) {
			if (u != v) {
				forces[u] += repellingForce(u, v);
				forces[v] += repellingForce(v, u);
			}
		}
	}

	for (EdgePtr uv : g.getUndirectedEdges()) {
		NodePtr u = uv->getSource();
		NodePtr v = uv->getTarget();
		forces[u] += attractiveForce(u, v);
		forces[v] += attractiveForce(v, u);
	}

	for (NodePtr u : g.getNodes()) {
		move(u, forces.at(u));
	}

}
