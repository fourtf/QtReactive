#include "QtReactive.hpp"

#include <unordered_map>
#include <unordered_set>

using namespace QtReactiveInternal;

using Func = std::function<void()>;
using FuncId = size_t;

namespace {

std::unordered_map<RefId, std::vector<FuncId>> dependencies;
std::unordered_map<FuncId, std::unordered_set<RefId>> refsOfFuncs;
std::unordered_map<FuncId, Func> funcs;

FuncId executingFunc = 0;

// unique ids
size_t currentId = 1;
size_t uniqueId() { return currentId++; };

// funcs
FuncId registerFunc(Func &func) {
  auto id = uniqueId();
  funcs[id] = func;
  return id;
}
void unregisterFunc(FuncId id) {
  funcs.erase(id);

  // clear dependencies of func
  if (auto it = refsOfFuncs.find(id); it != refsOfFuncs.end()) {
    for (RefId rId : it->second) {
      dependencies.erase(rId);
    }
  }
}

// void remove;
void registerDependency(RefId id) {
  dependencies[id].push_back(executingFunc);
  refsOfFuncs[executingFunc].erase(id);
}
} // namespace

namespace QtReactiveInternal {
void onAccessed(RefId id) {
  if (executingFunc == 0)
    return;

  // track as dependency
  registerDependency(id);
}

void onChanged(RefId id) {
  if (auto dIt = dependencies.find(id); dIt != dependencies.end()) {
    for (auto &&fId : dIt->second)
      if (auto fIt = funcs.find(fId); fIt != funcs.end())
        fIt->second();
  }
}

} // namespace QtReactiveInternal

void reactive_watch(QObject *parent, std::function<void()> f) {
  auto funcId = registerFunc(f);

  QObject::connect(parent, &QObject::destroyed,
                   [=] { unregisterFunc(funcId); });

  executingFunc = funcId;
  f();
  executingFunc = 0;
}
