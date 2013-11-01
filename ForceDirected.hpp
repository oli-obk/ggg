#import "Layout.hpp"

typedef Position Vector;

class ForceDirected : public Layout {
public:

	ForceDirected(double repConst=0.8, double step=5, double scale=100, int iterations=30);

	void run(Graph& g) override;

private:

	double repConst;
	double step;
	double scale;
	int iterations;
};
