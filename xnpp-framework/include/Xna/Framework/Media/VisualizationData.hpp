#ifndef XNA_FRAMEWORK_MEDIA_VISUALITIONDATA_HPP
#define XNA_FRAMEWORK_MEDIA_VISUALITIONDATA_HPP

#include <vector>
#include <memory>

namespace Xna {
	//Encapsulates visualization (frequency and sample) data for the currently-playing song.
	class VisualizationData final {
	public:
		//Initializes a new instance of the VisualizationData class.
		VisualizationData() { impl = std::make_shared<Implementation>(); }

		//Returns a collection of floats that contain frequency data.
		inline std::vector<float> Frequencies() const {
			return impl->frequenciesCollection;
		}
		//Returns a collection of floats that contain sample data.
		inline std::vector<float> Samples() const {
			return impl->samplesCollection;
		}

	private:
		struct Implementation {
			std::vector<float> frequencies;
			std::vector<float> samples;
			std::vector<float> frequenciesCollection;
			std::vector<float> samplesCollection;
		};

		std::shared_ptr<Implementation> impl = nullptr;
	};
}

#endif