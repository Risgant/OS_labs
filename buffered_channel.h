#ifndef BUFFERED_CHANNEL_H_
#define BUFFERED_CHANNEL_H_
#include <queue>
#include <mutex>

template<class T>
class BufferedChannel {
public:
    explicit BufferedChannel(int size) {
        this->size = size;
    }

    void Send(T value) {
        if (closed) throw std::runtime_error("channel closed\n");
        std::unique_lock<std::mutex> locker(lock);
        while (q.size() == size) {
            check.wait(locker);
        }
        q.push(value);
        check.notify_all();
    }

    std::pair<T, bool> Recv() {
        std::unique_lock<std::mutex> locker(lock);
        while (q.size() == 0) {
            if (closed) return std::pair<T, bool>(T(), false);
            check.wait(locker);
        }
        std::pair<T, bool> pair_ = std::pair<T, bool>(q.front(), true);
        q.pop();
        check.notify_all();
        return pair_;
    }

    void Close() {
        closed = true;
    }

private:
    std::queue<T> q;
    std::mutex lock;
    std::condition_variable check;
    bool closed;
    int size;
};

#endif // BUFFERED_CHANNEL_H_