#ifndef CSHARP_EVENT_HPP
#define CSHARP_EVENT_HPP

#include <functional>
#include <unordered_map>
#include <mutex>
#include <memory>
#include <algorithm>

namespace Xna::CSharp {
    // ---- Base EventArgs ----
    struct EventArgs {
        static EventArgs Empty;
    };

    // ---- EventHandler<TEventArgs> ----
    template<typename TEventArgs>
    using EventHandler = std::function<void(void* sender, const TEventArgs& args)>;

    template<typename TEventArgs>
    class Event {
    public:
        using Handler = EventHandler<TEventArgs>;
        Event() {
            impl = std::make_shared<Implementation>();
        };

        // Adiciona um handler -> retorna token
        int Add(const Handler& handler) {
            std::lock_guard<std::mutex> lock(impl->mutex);
            int id = impl->nextId++;
            impl->handlers[id] = handler;
            return id;
        }

        // Remove um handler pelo token
        void Remove(int token) {
            std::lock_guard<std::mutex> lock(impl->mutex);
            impl->handlers.erase(token);
        }   

        // Invocação do evento (equivalente ao "OnActivated(...)")
        void Invoke(void* sender, const TEventArgs& args) {
            std::lock_guard<std::mutex> lock(impl->mutex);
            for (auto& [id, handler] : impl->handlers) {
                if(handler)
                    handler(sender, args);
            }                
        }

    private:
        struct Implementation {
            std::unordered_map<int, Handler> handlers;
            int nextId = 0;
            std::mutex mutex;
        };

        std::shared_ptr<Implementation> impl;
    };
}

#endif