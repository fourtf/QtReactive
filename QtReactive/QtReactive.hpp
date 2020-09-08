#pragma once

#include <QObject>
#include <functional>
#include <utility>

namespace QtReactiveInternal {
using RefId = void *;
void onAccessed(RefId id);
void onChanged(RefId id);
} // namespace QtReactiveInternal

template <typename T> class Ref {
public:
  Ref() {}
  Ref(const T &val) : t_(val) {}
  Ref(T &&val) : t_(std::move(val)) {}

  Ref<T> &operator=(const T &val) {
    this->t_ = val;

    onChanged(this);
    return *this;
  }
  Ref<T> &operator=(T &&val) {
    this->t_ = std::move(val);
    QtReactiveInternal::onChanged(this);
    return *this;
  }

  operator T() { return this->value(); }

  const T &value() {
    QtReactiveInternal::onAccessed(this);
    return this->t_;
  }

private:
  T t_{};
};

void reactive_watch(QObject *parent, std::function<void()> f);
