#ifndef FLOW_H
#define FLOW_H

#include <functional>
#include <vector>
#include <mutex>
#include <algorithm>

template <typename T>
class Flow {
private:
    T state;
    std::vector<std::function<void(const T&)>> subscribers;
    std::mutex mtx;

public:
    explicit Flow(const T& initialState) : state(initialState) {}

    void subscribe(std::function<void(const T&)> subscriber) {
        std::lock_guard<std::mutex> lock(mtx);
        subscribers.push_back(subscriber);
    }

    void unsubscribe(std::function<void(const T&)> subscriber) {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = std::remove(subscribers.begin(), subscribers.end(), subscriber);
        subscribers.erase(it, subscribers.end());
    }

    void setState(T& newState) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            state = newState;
        }
        notify();
    }

    T getState() {
        std::lock_guard<std::mutex> lock(mtx);
        return state;
    }

private:
    void notify() {
        std::lock_guard<std::mutex> lock(mtx);
        for (const auto& subscriber : subscribers) {
            subscriber(state);
        }
    }
};

#endif // FLOW_H
