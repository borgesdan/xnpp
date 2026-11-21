#pragma once

#include <functional>
#include <vector>
#include <mutex>
#include <algorithm>

namespace Xna {

    struct EventArgs {
        static const EventArgs& Empty() noexcept {
            static const EventArgs event = {};
            return event;
        }
    };

    template<typename TEventArgs>
    using EventHandler = std::function<void(void* sender, const TEventArgs& args)>;    

    // Public Interface: Exposes only the ability to subscribe and unsubscribe
    template<typename TEventArgs>
    class IEvent {
    public:
        virtual ~IEvent() = default;
        virtual int Add(const EventHandler<TEventArgs>& handler) = 0;
        virtual void Remove(int token) = 0;

        // Operator overloading in the interface to enable polymorphism
        int operator+=(const EventHandler<TEventArgs>& handler) { return this->Add(handler); }
        void operator-=(int token) { this->Remove(token); }
    };

    // Internal Interface: Exposes only the ability to trigger the event
    template<typename TEventArgs>
    class IEventInvoker {
    public:
        virtual ~IEventInvoker() = default;
        virtual void Invoke(void* sender, const TEventArgs& args) = 0;
    };    

    // Optimized Event class (Few listeners - n <= 10, per event)
    template<typename TEventArgs>
    class Event : public IEvent<TEventArgs>, public IEventInvoker<TEventArgs> {
    public:
        using Handler = EventHandler<TEventArgs>;

        Event() = default;

        // Prevent copying to avoid concurrency issues with std::mutex
        Event(const Event&) = delete;
        Event& operator=(const Event&) = delete;

        // Allow safe movement of the event in memory
        Event(Event&& other) noexcept {
            std::lock_guard<std::mutex> lock(other.m_mutex);
            m_handlers = std::move(other.m_handlers);
            m_nextId = other.m_nextId;
        }

        Event& operator=(Event&& other) noexcept {
            if (this != &other) {
                std::unique_lock<std::mutex> lockThis(m_mutex, std::defer_lock);
                std::unique_lock<std::mutex> lockOther(other.m_mutex, std::defer_lock);
                std::lock(lockThis, lockOther); // Evita deadlocks ao mover

                m_handlers = std::move(other.m_handlers);
                m_nextId = other.m_nextId;
            }
            return *this;
        }        

        // Adds a handler and returns a unique identifier token
        int Add(const Handler& handler) override {
            std::lock_guard<std::mutex> lock(m_mutex);

            int id = m_nextId++;
            m_handlers.push_back({ id, handler });

            return id;
        }

        // Removes a handler based on the token
        void Remove(int token) override {
            std::lock_guard<std::mutex> lock(m_mutex);

            auto it = std::find_if(m_handlers.begin(), m_handlers.end(),
                [token](const RegisteredHandler& rh) { return rh.id == token; });

            if (it != m_handlers.end()) {
                m_handlers.erase(it);
            }
        }        

        // Triggers the event in a way that is safe against reactive deadlocks
        void Invoke(void* sender, const TEventArgs& args) override {           
            std::vector<Handler> handlersToInvoke;

            // Lock scope: Reduced to the minimum necessary to copy only the valid delegates
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                if (m_handlers.empty()) {
                    return; // Optimization for fast paths (Fast Path) with no listeners
                }

                handlersToInvoke.reserve(m_handlers.size());
                for (const auto& rh : m_handlers) {
                    if (rh.handler) {
                        handlersToInvoke.push_back(rh.handler);
                    }
                }
			} // The m_mutex is released here, allowing handlers to add/remove themselves or others without deadlocks

            // Safe invocation outside the critical section (Mutex unlocked)
            for (const auto& handler : handlersToInvoke) {
                handler(sender, args);
            }
        }

    private:
        struct RegisteredHandler {
            int id;
            Handler handler;
        };

        std::vector<RegisteredHandler> m_handlers;
        int m_nextId{ 0 };
        mutable std::mutex m_mutex; // 'mutable' allows for locks within const functions, if necessary
    };
}
