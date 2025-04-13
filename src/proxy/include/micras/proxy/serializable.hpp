/**
 * @file
 */

#ifndef MICRAS_PROXY_SERIALIZABLE_HPP
#define MICRAS_PROXY_SERIALIZABLE_HPP

#include <cstdint>
#include <vector>

namespace micras::proxy {
/**
 * @brief Interface for serializable objects.
 */
class ISerializable {
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~ISerializable() = default;

    /**
     * @brief Serialize the object.
     *
     * @return The serialized data.
     */
    virtual std::vector<uint8_t> serialize() const = 0;

    /**
     * @brief Deserialize the object.
     *
     * @param buffer The serialized data.
     * @param size The size of the serialized data.
     */
    virtual void deserialize(const uint8_t* buffer, uint16_t size) = 0;
};
}  // namespace micras::proxy

#endif  // MICRAS_PROXY_SERIALIZABLE_HPP 
