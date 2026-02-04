#ifndef XNA_CONTENTPIPELINE_GRAPHICS_MIPMAPCHAINCOLLECTION
#define XNA_CONTENTPIPELINE_GRAPHICS_MIPMAPCHAINCOLLECTION

#include <Xna/CSharp/Collection.hpp>
#include "MipmapChain.hpp"
#include <memory>
#include <optional>

namespace Xna {
	class MipmapChainCollection : public CSharp::Collection<MipmapChain> {
    public:
        MipmapChainCollection(size_t initialSize, bool fixedSize);       

        inline MipmapChainCollection(std::nullptr_t) : CSharp::Collection<MipmapChain>(nullptr), fixedSize{ false } { }

    protected:
        void ClearItems() override;
        void InsertItem(size_t index, MipmapChain const& item) override;
        void RemoveItem(size_t index) override;
        void SetItem(size_t index, MipmapChain const& item) override;

    private:
        bool fixedSize{false};
	};
}

#endif