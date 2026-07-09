const { RoccoDBNative } = require('./build/Release/roccodb-native.node');

class Collection {
    constructor(dbNative, name) {
        this.dbNative = dbNative;
        this.name = name;
        this.listeners = []; // NEW: Stores all active real-time watchers
    }

    // NEW: The Firebase-style real-time listener
    onChange(callback) {
        this.listeners.push(callback);
        // Returns a function to unsubscribe when the user is done listening
        return () => {
            this.listeners = this.listeners.filter(cb => cb !== callback);
        };
    }

    // Internal helper to fire events
    _notify(eventType, data) {
        this.listeners.forEach(cb => cb({ type: eventType, data: data }));
    }

    insert(data) {
        const id = this.dbNative.insert(this.name, JSON.stringify(data));
        // Fire the real-time event instantly after saving to C++
        this._notify("INSERT", { id: id, ...data });
        return id;
    }

    get(id) {
        const res = this.dbNative.get(this.name, id);
        if (res.startsWith("Error:")) return null;
        try { return JSON.parse(res); } catch(e) { return res; }
    }
}

class Storage {
    constructor(dbNative) {
        this.dbNative = dbNative;
    }
    upload(filePath) {
        return this.dbNative.upload(filePath);
    }
}

class RoccoDB {
    constructor(path) {
        this.dbNative = new RoccoDBNative(path);
        this.storage = new Storage(this.dbNative);
    }
    collection(name) {
        return new Collection(this.dbNative, name);
    }
}

module.exports = { RoccoDB };