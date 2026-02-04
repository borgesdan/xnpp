#include "Xna/ContentPipeline/Graphics/MipmapChainCollection.hpp"
#include <stdexcept>

namespace Xna {
	using Base = CSharp::Collection<MipmapChain>;

	MipmapChainCollection::MipmapChainCollection(size_t initialSize, bool fixedSize) {
		for (size_t index = 0; index < initialSize; ++index)
			Add(MipmapChain());

		 this->fixedSize = fixedSize;
	}

	void MipmapChainCollection::ClearItems() {
		if (fixedSize)
			throw std::runtime_error("Cannot resize MipmapChainCollection. This type of texture has a fixed number of faces.");

		Base::Clear();
	}

	void MipmapChainCollection::InsertItem(size_t index, MipmapChain const& item) {
		if (fixedSize)
			throw std::runtime_error("Cannot resize MipmapChainCollection. This type of texture has a fixed number of faces.");		

		Base::InsertItem(index, item);
	}

	void MipmapChainCollection::RemoveItem(size_t index) {
		if (fixedSize)
			throw std::runtime_error("Cannot resize MipmapChainCollection. This type of texture has a fixed number of faces.");

		Base::RemoveItem(index);
	}

	void MipmapChainCollection::SetItem(size_t index, MipmapChain const& item) {
		if (fixedSize)
			throw std::runtime_error("Cannot resize MipmapChainCollection. This type of texture has a fixed number of faces.");		

		Base::SetItem(index, item);
	}
}