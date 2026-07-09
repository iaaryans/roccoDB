#include <iostream>
#include "Database.h"
#define WIN32_LEAN_AND_MEAN 
#include "httplib.h" 

int main() {
    Database db("./my_database");
    auto users = db.collection("users");
    httplib::Server svr;

    // IMPORTANT: Serve our HTML file to the browser!
    svr.set_mount_point("/", "../public");

    // C: Create
    svr.Post("/users", [&](const httplib::Request& req, httplib::Response& res) {
        std::string id = users.insert(req.body);
        std::cout << "API Call: Created " << id << "\n";
        res.set_content("{\"id\": \"" + id + "\"}", "application/json");
    });

    // R: Read
    svr.Get(R"(/users/(.+))", [&](const httplib::Request& req, httplib::Response& res) {
        std::string id = req.matches[1];
        std::string data = users.get(id);
        std::cout << "API Call: Fetched " << id << "\n";
        if (data == "Error: Record not found") {
            res.status = 404;
            res.set_content("{\"error\": \"Not found or Deleted\"}", "application/json");
        } else {
            res.set_content(data, "application/json");
        }
    });

    // U: Update
    svr.Put(R"(/users/(.+))", [&](const httplib::Request& req, httplib::Response& res) {
        std::string id = req.matches[1];
        users.update(id, req.body);
        std::cout << "API Call: Updated " << id << "\n";
        res.set_content("{\"status\": \"Updated Successfully\"}", "application/json");
    });

    // D: Delete
    svr.Delete(R"(/users/(.+))", [&](const httplib::Request& req, httplib::Response& res) {
        std::string id = req.matches[1];
        users.remove(id);
        std::cout << "API Call: Deleted " << id << "\n";
        res.set_content("{\"status\": \"Deleted Successfully (Tombstoned)\"}", "application/json");
    });

    std::cout << "[roccoDB] Web Server is LIVE!\n";
    std::cout << "Open your browser to: http://localhost:8080/index.html\n";
    
    svr.listen("0.0.0.0", 8080); 
    return 0;
}