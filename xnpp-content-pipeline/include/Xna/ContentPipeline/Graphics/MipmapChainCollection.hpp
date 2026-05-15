#ifndef XNA_CONTENTPIPELINE_GRAPHICS_MIPMAPCHAINCOLLECTION
#define XNA_CONTENTPIPELINE_GRAPHICS_MIPMAPCHAINCOLLECTION

#include <Xna/CSharp/Collection.hpp>
#include "MipmapChain.hpp"
#include <memory>
#include <optional>
#include <Xna/Internal/Macros.hpp>

namespace Xna {
	class MipmapChainCollection : public CSharp::Collection<MipmapChain> {
    public:
        XNPP_API MipmapChainCollection(size_t initialSize, bool fixedSize);       

        inline MipmapChainCollection(std::nullptr_t) : CSharp::Collection<MipmapChain>(nullptr), fixedSize{ false } { }

    protected:
        XNPP_API void ClearItems() override;
        XNPP_API void InsertItem(size_t index, MipmapChain const& item) override;
        XNPP_API void RemoveItem(size_t index) override;
        XNPP_API void SetItem(size_t index, MipmapChain const& item) override;

    private:
        bool fixedSize{false};
	};
}

#endif