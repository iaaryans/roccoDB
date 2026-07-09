#include <napi.h>
#include "Database.h"

class RoccoDBNative : public Napi::ObjectWrap<RoccoDBNative> {
private:
    Database* db;

public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports) {
        Napi::Function func = DefineClass(env, "RoccoDBNative", {
            InstanceMethod("insert", &RoccoDBNative::Insert),
            InstanceMethod("get", &RoccoDBNative::Get),
            InstanceMethod("upload", &RoccoDBNative::Upload) // NEW: Exposing upload
        });

        Napi::FunctionReference* constructor = new Napi::FunctionReference();
        *constructor = Napi::Persistent(func);
        env.SetInstanceData(constructor);

        exports.Set("RoccoDBNative", func);
        return exports;
    }

    RoccoDBNative(const Napi::CallbackInfo& info) : Napi::ObjectWrap<RoccoDBNative>(info) {
        std::string path = info[0].As<Napi::String>().Utf8Value();
        this->db = new Database(path);
    }

    ~RoccoDBNative() {
        delete this->db;
    }

    Napi::Value Insert(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        std::string col_name = info[0].As<Napi::String>().Utf8Value();
        std::string data = info[1].As<Napi::String>().Utf8Value();
        
        std::string id = this->db->collection(col_name).insert(data);
        return Napi::String::New(env, id);
    }

    Napi::Value Get(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        std::string col_name = info[0].As<Napi::String>().Utf8Value();
        std::string id = info[1].As<Napi::String>().Utf8Value();
        
        std::string data = this->db->collection(col_name).get(id);
        return Napi::String::New(env, data);
    }

    // NEW: JavaScript calls this to upload a file
    Napi::Value Upload(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        std::string source_file = info[0].As<Napi::String>().Utf8Value();
        
        std::string file_id = this->db->storage().upload(source_file);
        return Napi::String::New(env, file_id);
    }
};

Napi::Object InitModule(Napi::Env env, Napi::Object exports) {
    return RoccoDBNative::Init(env, exports);
}
NODE_API_MODULE(roccodb, InitModule)