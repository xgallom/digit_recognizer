//
// Created by xgallom on 5/31/20.
//

#ifndef DIGIT_RECOGNIZER_DATA_LOADER_INCLUDE_DATALOADER_H
#define DIGIT_RECOGNIZER_DATA_LOADER_INCLUDE_DATALOADER_H

#include <cstdint>
#include <Array.h>
#include <Vector.h>
#include <UniquePtr.h>

namespace Image {
	static constexpr uint32_t Width = 28, Height = 28, Total = Width * Height;

	namespace Source {
		enum Enum {
			SampleSubmission,
			Test,
			Train,

			Count
		};
	}
	using PixelData = Array<double, Total>;
	using Data = Vector<UniquePtr<PixelData>>;

	using Value = uint8_t;
	using Values = Vector<Value>;
}

class DataLoader {
public:
	bool loadAll();

	[[nodiscard]]
	const Image::Data &data(Image::Source::Enum source) const noexcept;
	[[nodiscard]]
	const Image::Values &values(Image::Source::Enum source) const noexcept;

private:
	Array<Image::Data, Image::Source::Count> m_imagePixelData = {};
	Array<Image::Values, Image::Source::Count> m_imageValues = {};

	bool loadTrainingData() noexcept;
	bool loadTestData() noexcept;
};

#endif //DIGIT_RECOGNIZER_DATA_LOADER_INCLUDE_DATALOADER_H
