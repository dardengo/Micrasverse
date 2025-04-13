/**
 * @file
 */

#ifndef MICRAS_PROXY_STORAGE_HPP
#define MICRAS_PROXY_STORAGE_HPP

#include <cstdint>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#include "box2d/box2d.h"
#include "micras/proxy/serializable.hpp"

namespace micras::proxy {
/**
 * @brief Class for storing variables and classes in a simulation.
 */
class Storage {
public:
    /**
     * @brief Configuration struct for the Storage class.
     */
    struct Config {
        /**
         * @brief Path to store the data.
         */
        std::filesystem::path storage_path;
    };

    /**
     * @brief Type to store a primitive variable.
     */
    struct PrimitiveVariable {
        /**
         * @brief Address of the variable in the buffer.
         */
        uint16_t buffer_address{};

        /**
         * @brief Size of the variable in bytes.
         */
        uint16_t size{};

        /**
         * @brief Pointer to the variable in RAM.
         */
        const void* ram_pointer{nullptr};
    };

    /**
     * @brief Type to store a serializable variable.
     */
    struct SerializableVariable {
        /**
         * @brief Address of the variable in the buffer.
         */
        uint16_t buffer_address{};

        /**
         * @brief Size of the variable in bytes.
         */
        uint16_t size{};

        /**
         * @brief Pointer to the variable in RAM.
         */
        const ISerializable* ram_pointer{nullptr};
    };

    /**
     * @brief Construct a new Storage object.
     *
     * @param config The configuration for the Storage class.
     * @param bodyId The ID of the body to store the data for.
     */
    Storage(const Config& config, const b2BodyId bodyId);

    /**
     * @brief Create a new serializable variable.
     *
     * @param name The name of the variable.
     * @param data The data to store.
     */
    void create(const std::string& name, const ISerializable& data);

    /**
     * @brief Sync a serializable variable with the storage.
     *
     * @param name The name of the variable.
     * @param data The data to sync.
     */
    void sync(const std::string& name, ISerializable& data);

    /**
     * @brief Save the storage to disk.
     */
    void save();

    /**
     * @brief Load the storage from disk.
     */
    void load();

private:
    /**
     * @brief Start symbol for the storage file.
     */
    static constexpr uint16_t start_symbol = 0xAA55;

    /**
     * @brief Serialize a map of variables.
     *
     * @tparam T The type of the variables.
     * @param variables The map of variables to serialize.
     * @return The serialized data.
     */
    template <typename T>
    std::vector<uint8_t> serialize_var_map(const std::unordered_map<std::string, T>& variables);

    /**
     * @brief Deserialize a map of variables.
     *
     * @tparam T The type of the variables.
     * @param buffer The buffer to deserialize from.
     * @param num_vars The number of variables to deserialize.
     * @return The deserialized map of variables.
     */
    template <typename T>
    std::unordered_map<std::string, T> deserialize_var_map(std::vector<uint8_t>& buffer, uint16_t num_vars);

    /**
     * @brief ID of the body to store the data for.
     */
    b2BodyId bodyId;

    /**
     * @brief Path to store the data.
     */
    std::filesystem::path storage_path;

    /**
     * @brief Map of primitive variables.
     */
    std::unordered_map<std::string, PrimitiveVariable> primitives;

    /**
     * @brief Map of serializable variables.
     */
    std::unordered_map<std::string, SerializableVariable> serializables;

    /**
     * @brief Buffer to store the data.
     */
    std::vector<uint8_t> buffer;
};
}  // namespace micras::proxy

#endif  // MICRAS_PROXY_STORAGE_HPP 
