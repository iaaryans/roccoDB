# roccoDB

A local, folder-based NoSQL document database built from scratch in C++20 and exposed to Node.js through a native Node-API binding.

> **Status:** Experimental / educational release. roccoDB is not production-ready yet.

## What is roccoDB?

roccoDB is an embedded database that runs directly inside your Node.js application.

It does not require MySQL, MongoDB, SQLite, Firebase, or a separate database server.

Your data stays locally on the machine or server where your application is running.

```text
Node.js Application
        ↓
roccoDB JavaScript API
        ↓
Node-API Native Binding
        ↓
C++20 Database Engine
        ↓
Local Database Folder
```

## Features

- Local and embedded database
- Custom C++20 storage engine
- Persistent folder-based storage
- Append-only data storage
- In-memory ID index for fast lookups
- Basic Write-Ahead Logging (WAL)
- JavaScript object storage
- Insert and get documents by ID
- Local collection change events
- Separate image and file storage
- No external database server required

## Installation

```bash
npm install roccodb-iaaryan
```

roccoDB contains a native C++ addon, so native build tools may be required on your system.

## Quick Start

```javascript
const { RoccoDB } = require("roccodb-iaaryan");

const db = new RoccoDB("./my_database");

const users = db.collection("users");

const id = users.insert({
  name: "Aaryan",
  age: 20,
  active: true
});

console.log("Inserted ID:", id);

const user = users.get(id);

console.log("User:", user);
```

roccoDB automatically creates and manages the local database folder.

## Collections

Organize documents into separate collections:

```javascript
const users = db.collection("users");
const posts = db.collection("posts");
const messages = db.collection("messages");
```

Insert a document:

```javascript
const postId = posts.insert({
  title: "Hello roccoDB",
  content: "My first local database post",
  published: true
});
```

Read it back using its ID:

```javascript
const post = posts.get(postId);

console.log(post);
```

## Local Change Events

Listen for changes inside a collection:

```javascript
const messages = db.collection("messages");

messages.onChange((event) => {
  console.log("Database changed:", event);
});

messages.insert({
  text: "Hello from roccoDB!"
});
```

`onChange()` provides local change notifications inside the running application.

It is not cloud synchronization and does not automatically synchronize data between different machines or remote clients.

## Image and File Storage

roccoDB keeps larger files separate from normal document records.

```javascript
const imageRef = db.storage.upload("./images/profile.jpg");

const users = db.collection("users");

users.insert({
  name: "Aaryan",
  profileImage: imageRef
});
```

The document stores a lightweight reference while the file is managed separately by roccoDB storage.

## Example: Social Media Post

```javascript
const { RoccoDB } = require("roccodb-iaaryan");

const db = new RoccoDB("./social_database");

const posts = db.collection("posts");

const imageRef = db.storage.upload("./images/post.jpg");

const postId = posts.insert({
  author: "Aaryan",
  caption: "Building my own database from scratch 🚀",
  image: imageRef,
  likes: 0
});

const post = posts.get(postId);

console.log(post);
```

## Current JavaScript API

### Create a Database

```javascript
const db = new RoccoDB("./data");
```

### Open a Collection

```javascript
const users = db.collection("users");
```

### Insert a Document

```javascript
const id = users.insert({
  name: "Aaryan"
});
```

### Get a Document

```javascript
const user = users.get(id);
```

### Watch Collection Changes

```javascript
users.onChange((event) => {
  console.log(event);
});
```

### Store a File

```javascript
const fileRef = db.storage.upload("./photo.jpg");
```

## How Data Is Stored

roccoDB stores data locally inside the database path you provide:

```javascript
const db = new RoccoDB("./my_database");
```

The database engine manages its own local collections, storage files, indexes, and system data inside that directory.

The exact internal storage structure may evolve between experimental versions.

## Current Limitations

roccoDB v1.0.0 is an experimental release.

Currently:

- JavaScript API exposes `insert()` and `get()`
- Advanced queries and filtering are not available yet
- JavaScript-level update and delete APIs are not available yet
- Change events are local, not network-based realtime sync
- Advanced concurrent access is not yet supported
- Cross-platform compatibility is still evolving
- It should not be used for sensitive or critical production data

## Good Use Cases

roccoDB is currently suitable for:

- Learning database internals
- Local Node.js applications
- Prototypes
- Experimental backend projects
- Small developer tools
- Exploring C++ and JavaScript native integration

## Project Goal

The goal of roccoDB is to explore how a database works from the storage layer upward:

```text
Binary Storage
      ↓
Indexing
      ↓
Write-Ahead Logging
      ↓
C++ Database Engine
      ↓
Node.js Native Binding
      ↓
JavaScript Developer API
```

Future versions may expand the JavaScript API with richer CRUD operations, querying, indexing, storage features, and improved reliability.

## License

ISC

## Author

Built by Aaryan.

---

If you find a bug or want to contribute, contributions and feedback are welcome.

## Keywords

roccoDB
NoSQL Database
Embedded Database
Database Engine
Storage Engine
C++20
Node.js
Node API
N-API
Native Addon
Write Ahead Logging
WAL
Append Only Storage
Document Database
Local Database
Filesystem Database
High Performance Database
JavaScript Database
Developer Tools
Database From Scratch
