//
// Created by xgallom on 5/31/20.
//

#include <DataLoader.h>
#include <Delegate.h>
#include <FStream.h>
#include <Utility.h>
#include <execution>

static inline double normalize(uint8_t value)
{
	return double(value) / std::numeric_limits<uint8_t>::max();
}

template<typename F>
static inline void loadLine(IFStream &file, Image::Data &pixelsBuffer, F beforePixels)
{
	uint32_t value;
	char comma;

	beforePixels(file);

	auto pixelData = makeUnique<Image::PixelData>();
	auto walk = pixelData->begin();

	for(const auto last = pixelData->end() - 1; walk < last; ++walk) {
		file >> value >> comma;
		*walk = normalize(value);
	}

	file >> value;
	*walk = normalize(value);

	pixelsBuffer.push_back(move(pixelData));
}

template<typename F>
static inline void loadLines(IFStream &file, Image::Data &pixelsBuffer, F beforePixels)
{
	do
		loadLine(file, pixelsBuffer, beforePixels);
	while(!file.eof());
}

static bool load(Image::Data &pixels, Image::Values &values, const char *fileName, size_t expectedSize)
{
	Image::Data pixelsBuffer;
	Image::Values valuesBuffer;
	IFStream file(fileName);

	if(!file.is_open()) {
		s_cerr << "Failed to open file " << fileName << "\n";
		return false;
	}

	pixelsBuffer.reserve(expectedSize);
	valuesBuffer.reserve(expectedSize);

	loadLines(file,
			  pixelsBuffer,
			  [&valuesBuffer](auto &file) {
				  uint32_t value;
				  char comma;

				  file >> value >> comma;

				  valuesBuffer.push_back(value);
			  }
	);

	pixels = move(pixelsBuffer);
	values = move(valuesBuffer);

	return true;
}

static bool load(Image::Data &pixels, const char *fileName, size_t expectedSize)
{
	Image::Data pixelsBuffer;
	IFStream file(fileName);

	if(!file.is_open()) {
		s_cerr << "Failed to open file " << fileName << "\n";
		return false;
	}

	pixelsBuffer.reserve(expectedSize);

	loadLines(file, pixelsBuffer, [](auto &) {});

	pixels = move(pixelsBuffer);

	return true;
}

bool DataLoader::loadAll()
{
	using Initializer = Delegate<bool()>;

	const Array<Initializer, 2> Initializers = {
			Initializer(ConnectArg<&DataLoader::loadTrainingData>, this),
			Initializer(ConnectArg<&DataLoader::loadTestData>, this),
	};

	std::atomic_uint remaining = std::size(Initializers);

	std::for_each(std::execution::par_unseq,
				  std::begin(Initializers),
				  std::end(Initializers),
				  [&remaining](const Initializer &initializer) {
					  if(initializer())
						  remaining.fetch_sub(1, std::memory_order_release);
				  }
	);

	return remaining.load(std::memory_order_acquire) == 0;
}

const Image::Data &DataLoader::data(Image::Source::Enum source) const noexcept
{
	return m_imagePixelData[source];
}

const Image::Values &DataLoader::values(Image::Source::Enum source) const noexcept
{
	return m_imageValues[source];
}

bool DataLoader::loadTrainingData() noexcept
{
	return load(m_imagePixelData[Image::Source::Train],
				m_imageValues[Image::Source::Train],
				"data/train.csv",
				42000
	);
}

bool DataLoader::loadTestData() noexcept
{
	return load(m_imagePixelData[Image::Source::Test],
				"data/test.csv",
				28000
	);
}
