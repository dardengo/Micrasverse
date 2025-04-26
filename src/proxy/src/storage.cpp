#ifndef MICRAS_PROXY_STORAGE_CPP
#define MICRAS_PROXY_STORAGE_CPP

#include "micras/proxy/storage.hpp"
#include <fstream>
#include <filesystem>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <bit>

namespace micras::proxy {

Storage::Storage(const Config& config) : storage_path{config.storage_path} {
    // Create storage directory if it doesn't exist
    std::filesystem::create_directories(storage_path);
    this->load();
}

void Storage::create(const std::string& name, const core::ISerializable& data) {
    this->serializables[name].ram_pointer = &data;
    this->serializables[name].size = data.serialize().size();
}

void Storage::sync(const std::string& name, core::ISerializable& data) {
    if (this->serializables.contains(name) && this->serializables.at(name).ram_pointer == nullptr) {
        const auto& serializable = this->serializables.at(name);
        data.deserialize(&this->buffer.at(serializable.buffer_address), serializable.size);
    }

    this->create(name, data);
}

void Storage::save() {
    this->buffer.clear();
    
    for (auto it = this->primitives.begin(); it != this->primitives.end();) {
        auto& [name, variable] = *it;

        if (variable.ram_pointer == nullptr) {
            it = this->primitives.erase(it);
            continue;
        }

        const auto* aux = static_cast<const uint8_t*>(variable.ram_pointer);
        variable.buffer_address = buffer.size();

        this->buffer.insert(this->buffer.end(), aux, aux + variable.size);
        it++;
    }

    for (auto it = this->serializables.begin(); it != this->serializables.end();) {
        auto& [name, variable] = *it;

        if (variable.ram_pointer == nullptr) {
            it = this->serializables.erase(it);
            continue;
        }

        std::vector<uint8_t> aux = variable.ram_pointer->serialize();
        variable.buffer_address = this->buffer.size();
        variable.size = aux.size();
        this->buffer.insert(this->buffer.end(), aux.begin(), aux.end());
        it++;
    }

    auto serialized_serializables = serialize_var_map<SerializableVariable>(this->serializables);
    this->buffer.insert(this->buffer.begin(), serialized_serializables.begin(), serialized_serializables.end());

    auto serialized_primitives = serialize_var_map<PrimitiveVariable>(this->primitives);
    this->buffer.insert(this->buffer.begin(), serialized_primitives.begin(), serialized_primitives.end());

    // Create header (total size, num_primitives, num_serializables)
    const uint16_t total_size = this->buffer.size() / 8 + (this->buffer.size() % 8 == 0 ? 0 : 1);
    const uint16_t num_primitives = this->primitives.size();
    const uint16_t num_serializables = this->serializables.size();
    
    uint64_t header = static_cast<uint64_t>(start_symbol) << 48;
    header |= static_cast<uint64_t>(total_size) << 32;
    header |= static_cast<uint64_t>(num_primitives) << 16;
    header |= num_serializables;
    
    // Pad the buffer to be multiple of 8 bytes
    this->buffer.insert(this->buffer.end(), (8 - (this->buffer.size() % 8)) % 8, 0);
    
    // Write to file
    std::filesystem::path file_path = storage_path / "storage.bin";
    std::ofstream file(file_path, std::ios::binary);
    
    // Write header
    file.write(reinterpret_cast<const char*>(&header), sizeof(header));
    
    // Write buffer
    file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
}

void Storage::load() {
    std::filesystem::path file_path = storage_path / "storage.bin";
    if (!std::filesystem::exists(file_path)) {
        return;
    }
    
    // Read file
    std::ifstream file(file_path, std::ios::binary);
    
    // Read header
    uint64_t header = 0;
    file.read(reinterpret_cast<char*>(&header), sizeof(header));
    
    if ((header >> 48) != start_symbol) {
        std::cerr << "Invalid storage file format" << std::endl;
        return;
    }
    
    const uint16_t total_size = (header >> 32) & 0xFFFF;
    const uint16_t num_primitives = (header >> 16) & 0xFFFF;
    const uint16_t num_serializables = header & 0xFFFF;
    
    // Read buffer
    this->buffer.resize(total_size * 8);
    file.read(reinterpret_cast<char*>(buffer.data()), total_size * 8);
    
    // Deserialize primitives and serializables
    this->primitives = deserialize_var_map<PrimitiveVariable>(this->buffer, num_primitives);
    this->serializables = deserialize_var_map<SerializableVariable>(this->buffer, num_serializables);
}

template <typename T>
std::vector<uint8_t> Storage::serialize_var_map(const std::unordered_map<std::string, T>& variables) {
    std::vector<uint8_t> buffer;

    for (const auto& [name, variable] : variables) {
        buffer.push_back(name.size());
        buffer.insert(buffer.end(), name.begin(), name.end());

        buffer.push_back(variable.buffer_address & 0xFF);
        buffer.push_back((variable.buffer_address >> 8) & 0xFF);

        buffer.push_back(variable.size & 0xFF);
        buffer.push_back((variable.size >> 8) & 0xFF);
    }

    return buffer;
}

template <typename T>
std::unordered_map<std::string, T> Storage::deserialize_var_map(std::vector<uint8_t>& buffer, uint16_t num_vars) {
    std::unordered_map<std::string, T> variables;
    uint16_t current_addr = 0;

    for (uint16_t decoded_vars = 0; decoded_vars < num_vars && current_addr < buffer.size(); decoded_vars++) {
        const uint8_t var_name_len = buffer.at(current_addr);
        current_addr++;

        const std::string var_name(buffer.begin() + current_addr, buffer.begin() + current_addr + var_name_len);
        current_addr += var_name_len;

        variables[var_name].buffer_address = buffer.at(current_addr) | (buffer.at(current_addr + 1) << 8);
        current_addr += 2;

        variables.at(var_name).size = buffer.at(current_addr) | (buffer.at(current_addr + 1) << 8);
        current_addr += 2;
    }

    buffer.erase(buffer.begin(), buffer.begin() + current_addr);
    return variables;
}

// Explicit instantiation of template functions
template std::vector<uint8_t>
    Storage::serialize_var_map(const std::unordered_map<std::string, PrimitiveVariable>& variables);

template std::unordered_map<std::string, Storage::PrimitiveVariable>
    Storage::deserialize_var_map(std::vector<uint8_t>& buffer, uint16_t num_vars);

template std::vector<uint8_t>
    Storage::serialize_var_map(const std::unordered_map<std::string, SerializableVariable>& variables);

template std::unordered_map<std::string, Storage::SerializableVariable>
    Storage::deserialize_var_map(std::vector<uint8_t>& buffer, uint16_t num_vars);

}  // namespace micras::proxy

#endif  // MICRAS_PROXY_STORAGE_CPP 
