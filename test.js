const { RoccoDB } = require('./index.js');

console.log("--- roccoDB Real-Time Watcher Test ---");
const db = new RoccoDB("./node_db");
const posts = db.collection("posts");

// 1. SETUP THE REAL-TIME WATCHER (Just like Firebase!)
console.log("Starting real-time listener on 'posts' collection...\n");

const unsubscribe = posts.onChange((event) => {
    console.log("🔔 [REAL-TIME ALERT] Database Changed!");
    console.log(`Action: ${event.type}`);
    console.log("New Data:", event.data);
    console.log("----------------------------------\n");
});

// 2. TRIGGER A CHANGE
console.log("Simulating a new post insertion in 2 seconds...");

setTimeout(() => {
    posts.insert({
        author: "aaryan_dev",
        content: "Just built a real-time event system in my custom database!",
        likes: 0
    });
}, 2000); // Waits 2 seconds so you can see it happen live
