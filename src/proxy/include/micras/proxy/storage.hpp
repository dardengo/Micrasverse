#ifndef MICRAS_PROXY_STORAGE_HPP
#define MICRAS_PROXY_STORAGE_HPP

#include <cstdint>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#include "box2d/box2d.h"
#include "micras/proxy/serializable.hpp"
#include "physics/box2d_micrasbody.hpp"

namespace micras::proxy {

class Storage {
public:
    struct Config {
        micrasverse::physics::Box2DMicrasBody* micrasBody = nullptr;
        std::filesystem::path storage_path;
    };
    
    struct PrimitiveVariable {
        uint16_t buffer_address{};
        uint16_t size{};
        const void* ram_pointer{nullptr};
    };

    struct SerializableVariable {
        uint16_t buffer_address{};
        uint16_t size{};
        const ISerializable* ram_pointer{nullptr};
    };

    explicit Storage(const Config& config);

    void create(const std::string& name, const ISerializable& data);

    void sync(const std::string& name, ISerializable& data);

    void save();
    
    void load();

private:

    static constexpr uint16_t start_symbol = 0xAA55;

    template <typename T>
    std::vector<uint8_t> serialize_var_map(const std::unordered_map<std::string, T>& variables);

    template <typename T>
    std::unordered_map<std::string, T> deserialize_var_map(std::vector<uint8_t>& buffer, uint16_t num_vars);

    micrasverse::physics::Box2DMicrasBody* micrasBody;
    
    std::filesystem::path storage_path;

    std::unordered_map<std::string, PrimitiveVariable> primitives;

    std::unordered_map<std::string, SerializableVariable> serializables;

    std::vector<uint8_t> buffer;
};
}  // namespace micras::proxy

#endif  // MICRAS_PROXY_STORAGE_HPP 
