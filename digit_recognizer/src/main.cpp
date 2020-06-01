#include <DataLoader.h>
#include <Engine.h>
#include <layer/Convolution.h>

int main()
{
	s_cout << "Loading data\n";

	DataLoader dataLoader;

	if(!dataLoader.loadAll()) {
		s_cerr << "Error: Data loading failed\n";
		return EXIT_FAILURE;
	}

	s_cout << "Loaded data\n";

	Engine engine;

	Convolution firstConvolutionPass{
		.filters = 32,
		.kernelSize = {5, 5},
		.activation = Activation::Relu,
	};
	engine.add(firstConvolutionPass);
	engine.add(firstConvolutionPass);

	Convolution secondConvolutionPass{
			.filters = 64,
			.kernelSize = {3, 3},
			.activation = Activation::Relu,
	};
	engine.add(secondConvolutionPass);
	engine.add(secondConvolutionPass);

	return EXIT_SUCCESS;
}
