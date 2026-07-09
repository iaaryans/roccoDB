#pragma once
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <random> // NEW: For generating unique IDs

namespace fs = std::filesystem;

class Collection {
private:
    std::string collection_path;
    fs::path data_file;
    fs::path wal_file;
    std::unordered_map<std::string, std::streampos> index;

    // NEW: Generates a random 12-character hex string (e.g., "a3f9b2c1d4e5")
    std::string generate_id() {
        const char chars[] = "0123456789abcdef";
        std::string id;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 15);
        for (int i = 0; i < 12; ++i) id += chars[dis(gen)];
        return id;
    }

    void build_index() {
        std::ifstream infile(data_file, std::ios::binary);
        if (!infile) return;

        while (infile.peek() != EOF) {
            std::streampos current_pos = infile.tellg();
            bool is_deleted;
            size_t id_len = 0, data_len = 0;
            
            if (!infile.read(reinterpret_cast<char*>(&is_deleted), sizeof(is_deleted))) break;
            
            infile.read(reinterpret_cast<char*>(&id_len), sizeof(id_len));
            std::string id(id_len, '\0');
            infile.read(&id[0], id_len);
            
            infile.read(reinterpret_cast<char*>(&data_len), sizeof(data_len));
            infile.seekg(data_len, std::ios::cur); 

            if (is_deleted) {
                index.erase(id);
            } else {
                index[id] = current_pos;
            }
        }
    }

    void append_record(const std::string& id, const std::string& data, bool is_deleted) {
        std::ofstream wal_out(wal_file, std::ios::app);
        wal_out << (is_deleted ? "DELETE " : "INSERT ") << id << "\n";
        wal_out.flush();

        std::ofstream outfile(data_file, std::ios::app | std::ios::binary);
        if (!outfile) return;

        outfile.seekp(0, std::ios::end);
        std::streampos start_pos = outfile.tellp();

        size_t id_len = id.size();
        size_t data_len = data.size();

        outfile.write(reinterpret_cast<const char*>(&is_deleted), sizeof(is_deleted));
        outfile.write(reinterpret_cast<const char*>(&id_len), sizeof(id_len));
        outfile.write(id.c_str(), id_len);
        outfile.write(reinterpret_cast<const char*>(&data_len), sizeof(data_len));
        outfile.write(data.c_str(), data_len);
        outfile.flush();

        if (is_deleted) {
            index.erase(id);
        } else {
            index[id] = start_pos;
        }

        std::ofstream clear_wal(wal_file, std::ios::trunc);
    }

public:
    Collection(const std::string& path) : collection_path(path) {
        if (!fs::exists(collection_path)) fs::create_directories(collection_path);
        data_file = fs::path(collection_path) / "data.bin";
        wal_file = fs::path(collection_path) / "wal.log";
        build_index();
    }

    // ORIGINAL: Insert with manual ID (Used for Updates)
    void insert(const std::string& id, const std::string& data) {
        append_record(id, data, false);
    }

    // NEW API FEATURE: Insert without an ID, returns auto-generated ID
    std::string insert(const std::string& data) {
        std::string new_id = generate_id();
        append_record(new_id, data, false);
        return new_id;
    }
    
    void update(const std::string& id, const std::string& new_data) { insert(id, new_data); }
    
    void remove(const std::string& id) { append_record(id, "", true); }

    std::string get(const std::string& id) {
        if (index.find(id) == index.end()) return "Error: Record not found";

        std::ifstream infile(data_file, std::ios::binary);
        if (!infile) return "Error: Cannot open file";

        infile.seekg(index[id]); 

        bool is_deleted;
        size_t id_len = 0, data_len = 0;
        
        infile.read(reinterpret_cast<char*>(&is_deleted), sizeof(is_deleted));
        
        infile.read(reinterpret_cast<char*>(&id_len), sizeof(id_len));
        std::string read_id(id_len, '\0');
        infile.read(&read_id[0], id_len);
        
        infile.read(reinterpret_cast<char*>(&data_len), sizeof(data_len));
        std::string data(data_len, '\0');
        infile.read(&data[0], data_len);

        return data;
    }
};