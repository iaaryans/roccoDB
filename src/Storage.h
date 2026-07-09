#pragma once
#include <string>
#include <filesystem>
#include <random>
#include <iostream>

class Storage {
private:
    std::filesystem::path storage_path;

    std::string generate_file_id() {
        const char chars[] = "0123456789abcdef";
        std::string id;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 15);
        for (int i = 0; i < 16; ++i) id += chars[dis(gen)];
        return id;
    }

public:
    Storage(const std::filesystem::path& path) : storage_path(path) {}

    std::string upload(const std::string& source_file_path) {
        if (!std::filesystem::exists(source_file_path)) {
            return "Error: Source file does not exist";
        }

        std::filesystem::path source_path(source_file_path);
        std::string ext = source_path.extension().string(); 
        
        std::string file_id = generate_file_id() + ext;
        std::filesystem::path destination = storage_path / file_id;

        std::filesystem::copy(source_path, destination, std::filesystem::copy_options::overwrite_existing);

        std::cout << "[roccoDB] Safely stored media file: " << file_id << "\n";
        return file_id; 
    }
};