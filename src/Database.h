#pragma once
#include <string>
#include <filesystem>
#include <iostream>
#include "Collection.h" 
#include "Storage.h"

class Database {
private:
    std::string db_path;
    std::filesystem::path collections_path;
    std::filesystem::path storage_path; 

public:
    Database(const std::string& path) : db_path(path) {
        collections_path = std::filesystem::path(db_path) / "collections";
        storage_path = std::filesystem::path(db_path) / "storage";

        if (!std::filesystem::exists(db_path)) {
            std::filesystem::create_directories(db_path);
            std::filesystem::create_directories(collections_path);
            std::filesystem::create_directories(storage_path);
            std::cout << "[roccoDB] Created new advanced architecture at: " << db_path << "\n";
        } else {
            if (!std::filesystem::exists(collections_path)) std::filesystem::create_directories(collections_path);
            if (!std::filesystem::exists(storage_path)) std::filesystem::create_directories(storage_path);
            std::cout << "[roccoDB] Opened existing database at: " << db_path << "\n";
        }
    }

    Collection collection(const std::string& name) {
        std::string col_path = (collections_path / name).string();
        return Collection(col_path);
    }

    Storage storage() {
        return Storage(storage_path);
    }
};