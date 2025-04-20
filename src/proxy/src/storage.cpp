#ifndef MICRAS_PROXY_STORAGE_CPP
#define MICRAS_PROXY_STORAGE_CPP

#include "micras/proxy/storage.hpp"
#include <fstream>
#include <filesystem>
#include <cstring>
#include <algorithm>
#include <iostream>

namespace micras::proxy {

Storage::Storage(const Config& config) :
    micrasBody{config.micrasBody},
    storage_path{config.storage_path} {
    // Create storage directory if it doesn't exist
    std::filesystem::create_directories(storage_path);
    this->load();
}

void Storage::create(const std::string& name, const ISerializable& data) {
    this->serializables[name].ram_pointer = &data;
    this->serializables[name].size = data.serialize().size();
}

void Storage::sync(const std::string& name, ISerializable& data) {
    if (this->serializables.contains(name) and this->serializables.at(name).ram_pointer == nullptr) {
        data.deserialize(this->buffer.data() + this->serializables.at(name).buffer_address, 
                        this->serializables.at(name).size);
    }

    this->create(name, data);
}

void Storage::save() {
    std::vector<uint8_t> file_buffer;
    
    // Add start symbol
    file_buffer.push_back(static_cast<uint8_t>(start_symbol >> 8));
    file_buffer.push_back(static_cast<uint8_t>(start_symbol & 0xFF));
    
    // Serialize primitives
    std::vector<uint8_t> primitives_buffer = serialize_var_map<PrimitiveVariable>(this->primitives);
    file_buffer.insert(file_buffer.end(), primitives_buffer.begin(), primitives_buffer.end());
    
    // Serialize serializables
    std::vector<uint8_t> serializables_buffer = serialize_var_map<SerializableVariable>(this->serializables);
    file_buffer.insert(file_buffer.end(), serializables_buffer.begin(), serializables_buffer.end());
    
    // Add buffer data
    file_buffer.insert(file_buffer.end(), this->buffer.begin(), this->buffer.end());
    
    // Write to file
    std::ofstream file(storage_path / "storage.bin", std::ios::binary);
    file.write(reinterpret_cast<const char*>(file_buffer.data()), file_buffer.size());
}

void Storage::load() {
    std::filesystem::path file_path = storage_path / "storage.bin";
    if (!std::filesystem::exists(file_path)) {
        return;
    }
    
    // Read file
    std::ifstream file(file_path, std::ios::binary);
    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<uint8_t> file_buffer(file_size);
    file.read(reinterpret_cast<char*>(file_buffer.data()), file_size);
    
    // Check start symbol
    uint16_t read_symbol = (static_cast<uint16_t>(file_buffer[0]) << 8) | file_buffer[1];
    if (read_symbol != start_symbol) {
        std::cerr << "Invalid storage file format" << std::endl;
        return;
    }
    
    size_t offset = 2;  // Skip start symbol
    
    // Deserialize primitives
    uint16_t num_primitives = (static_cast<uint16_t>(file_buffer[offset]) << 8) | file_buffer[offset + 1];
    offset += 2;
    this->primitives = deserialize_var_map<PrimitiveVariable>(file_buffer, num_primitives);
    
    // Deserialize serializables
    uint16_t num_serializables = (static_cast<uint16_t>(file_buffer[offset]) << 8) | file_buffer[offset + 1];
    offset += 2;
    this->serializables = deserialize_var_map<SerializableVariable>(file_buffer, num_serializables);
    
    // Read buffer data
    this->buffer.assign(file_buffer.begin() + offset, file_buffer.end());
}

template <typename T>
std::vector<uint8_t> Storage::serialize_var_map(const std::unordered_map<std::string, T>& variables) {
    std::vector<uint8_t> buffer;
    
    // Add number of variables
    uint16_t num_vars = static_cast<uint16_t>(variables.size());
    buffer.push_back(static_cast<uint8_t>(num_vars >> 8));
    buffer.push_back(static_cast<uint8_t>(num_vars & 0xFF));
    
    // Serialize each variable
    for (const auto& [name, var] : variables) {
        // Add name length and name
        uint16_t name_length = static_cast<uint16_t>(name.length());
        buffer.push_back(static_cast<uint8_t>(name_length >> 8));
        buffer.push_back(static_cast<uint8_t>(name_length & 0xFF));
        buffer.insert(buffer.end(), name.begin(), name.end());
        
        // Add variable data
        if constexpr (std::is_same_v<T, PrimitiveVariable>) {
            buffer.push_back(static_cast<uint8_t>(var.buffer_address >> 8));
            buffer.push_back(static_cast<uint8_t>(var.buffer_address & 0xFF));
            buffer.push_back(static_cast<uint8_t>(var.size >> 8));
            buffer.push_back(static_cast<uint8_t>(var.size & 0xFF));
        } else if constexpr (std::is_same_v<T, SerializableVariable>) {
            buffer.push_back(static_cast<uint8_t>(var.buffer_address >> 8));
            buffer.push_back(static_cast<uint8_t>(var.buffer_address & 0xFF));
            buffer.push_back(static_cast<uint8_t>(var.size >> 8));
            buffer.push_back(static_cast<uint8_t>(var.size & 0xFF));
        }
    }
    
    return buffer;
}

template <typename T>
std::unordered_map<std::string, T> Storage::deserialize_var_map(std::vector<uint8_t>& buffer, uint16_t num_vars) {
    std::unordered_map<std::string, T> variables;
    size_t offset = 0;
    
    for (uint16_t i = 0; i < num_vars; ++i) {
        // Read name length and name
        uint16_t name_length = (static_cast<uint16_t>(buffer[offset]) << 8) | buffer[offset + 1];
        offset += 2;
        
        std::string name(buffer.begin() + offset, buffer.begin() + offset + name_length);
        offset += name_length;
        
        // Read variable data
        T var;
        if constexpr (std::is_same_v<T, PrimitiveVariable>) {
            var.buffer_address = (static_cast<uint16_t>(buffer[offset]) << 8) | buffer[offset + 1];
            offset += 2;
            var.size = (static_cast<uint16_t>(buffer[offset]) << 8) | buffer[offset + 1];
            offset += 2;
        } else if constexpr (std::is_same_v<T, SerializableVariable>) {
            var.buffer_address = (static_cast<uint16_t>(buffer[offset]) << 8) | buffer[offset + 1];
            offset += 2;
            var.size = (static_cast<uint16_t>(buffer[offset]) << 8) | buffer[offset + 1];
            offset += 2;
        }
        
        variables[name] = var;
    }
    
    return variables;
}

// Explicit template instantiations
template std::vector<uint8_t> Storage::serialize_var_map<Storage::PrimitiveVariable>(
    const std::unordered_map<std::string, Storage::PrimitiveVariable>&);
template std::vector<uint8_t> Storage::serialize_var_map<Storage::SerializableVariable>(
    const std::unordered_map<std::string, Storage::SerializableVariable>&);
template std::unordered_map<std::string, Storage::PrimitiveVariable> Storage::deserialize_var_map<Storage::PrimitiveVariable>(
    std::vector<uint8_t>&, uint16_t);
template std::unordered_map<std::string, Storage::SerializableVariable> Storage::deserialize_var_map<Storage::SerializableVariable>(
    std::vector<uint8_t>&, uint16_t);

}  // namespace micras::proxy

#endif  // MICRAS_PROXY_STORAGE_CPP 
