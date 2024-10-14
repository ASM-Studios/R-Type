# R-Type project: Comparative Study

In the R-Type project, we aim to recreate the classic R-Type game in a multiplayer version. This project focuses on
both the client and server sides, with an emphasis on the network part. The server will be developed as a game engine,
and the client will be a simple game client. The project is written entirely in C++.

This comparative study evaluates various technologies and techniques that could be used in the development of the R-Type
game. We compare different algorithms, data structures, graphics handling libraries, networking techniques, storage
methods, and security measures to determine the best choices for the project.

---

# 1. Algorithms and Data Structures

In a game like **R-Type**, performance is critical, so the choice of algorithms and data structures must balance
efficiency with simplicity. Here's how algorithms and data structures play a role:

## 1.1 Game Logic Algorithms

### 1.1.1 Collision Detection
- Axis-Aligned Bounding Box (AABB): For 2D collision detection, AABB is efficient and widely used in games
like R-Type. It’s a simple algorithm that checks for overlaps between rectangular bounding boxes, which is suitable
for the type of objects (spaceships, projectiles, enemies) typically found in R-Type.

- Comparison: AABB is preferred over more complex collision detection methods (e.g., Separating Axis Theorem or
pixel-perfect collision detection) due to its simplicity and performance benefits in a fast-paced game. AABB can be
combined with spatial partitioning techniques for better scalability in levels with many entities.

### 1.1.2 Pathfinding
- A\* Algorithm: If enemy AI has complex movement patterns (e.g., enemies following the player or avoiding
obstacles), the A* algorithm would be ideal. A* is a popular choice for pathfinding in games because it combines
the efficiency of Dijkstra’s algorithm with heuristics, making it faster and more targeted.

- Comparison: While simpler pathfinding algorithms like BFS could be used for uniform, grid-based levels, A*
allows for more intelligent behavior, though it’s more computationally expensive.

## 1.2 Data Structures

### 1.2.1 Quadtrees
- Quadtree: Used for partitioning the 2D space of the game world. It divides the game area into smaller regions,
allowing efficient queries about which objects are near each other. This is critical for optimizing collision detection
and rendering, as only nearby objects need to be checked for interactions.

  Comparison: Quadtrees are far more efficient for large, open game areas than brute-force collision checks. They
offer better scalability than simple list-based approaches when many entities are present.

### 1.2.2 Entity-Component System (ECS)
- ECS: An Entity-Component System is a design pattern widely used in game development. It allows for
flexibility by decoupling entity behavior (e.g., player, enemies, power-ups) from the actual objects. This makes it
easier to add or modify behaviors without rewriting large parts of the game engine.
- OOP: In contrast, Object-Oriented Programming (OOP) would involve deep inheritance hierarchies but could be
faster to implement for simpler games.

  Comparison: OOP is prefered for simpler games with fewer entities and behaviors, while ECS is more scalable
and maintainable for complex games.

---

# 2. Graphics Handling and Libraries

The visual elements are central to R-Type's appeal. The choice of graphics libraries influences performance,
rendering quality, and ease of use.

## 2.1 Graphics Libraries

### 2.1.1 SDL2 (Simple DirectMedia Layer)
- SDL2: SDL2 is a cross-platform graphics library that simplifies handling 2D graphics, sound, and input.
For a 2D shooter like R-Type, SDL2 offers efficient rendering and input handling, making it a popular choice
for retro-style games.

  Comparison: SDL2 is lightweight compared to more feature-heavy libraries like Unity or Unreal Engine.
It provides enough functionality for 2D games without unnecessary overhead, making it perfect for R-Type.
SDL also supports hardware acceleration via OpenGL or Direct3D.

### 2.1.2 SFML (Simple and Fast Multimedia Library)
- SFML: Another 2D-focused library similar to SDL but with a slightly more user-friendly API. It supports
window management, rendering, and multimedia (sound and input).

  Comparison: While SFML has a simpler API and might be easier to learn, SDL2 is more widely used in the game
development community, meaning it has more support and third-party tools.

## 2.2 Rendering Techniques

### 2.2.1 Double Buffering
- Double Buffering: Essential for smooth gameplay. Double buffering ensures that the screen isn’t redrawn
mid-frame, which prevents visual tearing.

  Comparison: Single buffering could cause flickering or screen tearing, which would ruin the experience
in a fast-paced game like R-Type. Triple buffering is an alternative but consumes more memory and isn’t necessary
for most 2D games.

---

# 3. Networking Techniques

For multiplayer or co-op versions of R-Type, networking plays a vital role. Here's how networking technologies
were evaluated:

## 3.1 Sockets
- TCP vs UDP: For real-time multiplayer gaming, UDP (User Datagram Protocol) is usually favored because it is
faster and doesn’t require acknowledgment of packet receipt, which reduces latency. However, since UDP doesn’t
guarantee packet delivery, it requires additional logic for critical data (e.g., player positions) to ensure
synchronization.

  Comparison: TCP is more reliable but can introduce delays due to the acknowledgment of packets, making it
unsuitable for real-time games. UDP is preferred for its speed, with custom-built reliability for key actions
like joining or leaving a game.

## 3.2 Peer-to-Peer (P2P) vs Client-Server
- Client-Server Model: Chosen for its simplicity in managing game state and preventing cheating in multiplayer
scenarios. The server holds the authoritative game state, and clients send inputs, which the server processes
and broadcasts back.

  Comparison: While P2P networking could reduce server load, it is more complex and prone to synchronization
issues. For a game like R-Type, where predictability and synchronization are crucial, the client-server model offers
better control over the game state and prevents cheating.

---

# 4. Storage Techniques

For an R-Type game, storage is less about large databases and more about configuration, user progress, and multiplayer
session data.

## 4.1 File-Based Storage
- JSON/XML for Configuration: JSON is lightweight and human-readable, making it ideal for storing game settings,
- high scores, and level configurations. It’s easy to parse and modify, which is useful during the development phase.

  Comparison: JSON is more modern and lightweight compared to XML, which tends to be verbose. Binary formats could
- be used for faster loading but at the cost of readability.

## 4.2 Persistent Storage for Multiplayer
- SQLite: A small, serverless SQL database that can be used for storing user progress, high scores, and matchmaking
- data. For a game like R-Type, SQLite offers sufficient performance without the complexity of setting up a full
- database server.

  Comparison: While more scalable solutions like MySQL/PostgreSQL could be used, they are unnecessary for small
- games with lightweight storage needs. SQLite is more appropriate for single-user settings or small-scale multiplayer.

---

# 5. Security

Security in game development focuses on data integrity, preventing cheating, and securing multiplayer interactions.

## 5.1 Cheat Prevention
- Server-Side Validation: In multiplayer games, all critical game logic (e.g., movement, collision, and scoring)
- is validated server-side to prevent cheating. This ensures that any input from the client is verified by the server
- before being applied to the game state.

  Comparison: Client-side validation is faster but prone to exploitation through client modification, which is why
- server-side validation is essential in multiplayer scenarios.

## 5.2 Data Encryption
- TLS for Data Transmission: All data transmitted between the client and server, especially in multiplayer games,
- should be encrypted using TLS (Transport Layer Security) to prevent man-in-the-middle attacks and tampering with
- game data.

  Comparison: Plaintext transmission is faster but highly insecure. In modern games, even fast-paced ones like
- R-Type, the slight overhead of encryption is worth the security benefits.

---

# Conclusion

The choice of technologies for the R-Type project is centered around performance, security, and maintainability.
SDL2 for graphics, UDP for networking, and AABB for collision detection are all chosen for their balance
between simplicity and performance. On the storage front, SQLite and JSON provide lightweight, easy-to-use
solutions for small datasets, and server-side validation ensures security against cheating. These technologies were
selected based on their suitability for a fast-paced 2D game with both single-player and multiplayer components.
