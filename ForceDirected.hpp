#import "Layout.hpp"

typedef Position Vector;

class ForceDirected : public Layout {
public:

	ForceDirected(double repConst=0.4, double step=0.05, double scale=1, int iterations=30);

	void run(Graph& g) override;

private:

	double repConst;
	double step;
	double scale;
	int iterations;
};