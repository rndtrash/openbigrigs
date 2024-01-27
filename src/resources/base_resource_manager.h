#pragma once

#include <unordered_map>
#include <spdlog/spdlog.h>

#include "resource.h"
#include "file.h"

namespace OBR {

    template<DerivedFromResource T>
    class BaseResourceManager {
    public:
        BaseResourceManager() = default;

        ~BaseResourceManager() = default;

        std::shared_ptr<T> get(const std::string &name) {
            if (registry.contains(name)) {
                if (auto res_ptr = registry[name].lock()) {
                    if (auto file_ptr = std::dynamic_pointer_cast<T>(res_ptr))
                        return file_ptr;
                }

                // we will replace the invalid pointer anyway
            }

            auto deleter = [&](T *resource) {
                spdlog::debug("Deleter triggered {}, size before: {}", resource->get_path(), registry.size());
                registry.erase(resource->get_path());
                spdlog::debug("Deleter triggered, size after: {}", registry.size());
                delete resource;
            };
            auto file = std::shared_ptr<T>{new T(name), deleter};
            auto filepath = file->get_path();
            registry[filepath] = file;
            return file;
        }

    private:
        std::unordered_map<std::string, std::weak_ptr<T>> registry{};
    };

}
