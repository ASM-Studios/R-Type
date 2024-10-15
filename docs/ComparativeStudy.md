# Comparative Study for the R-Type Project

## Table of Contents
- [Entity-Component-System (ECS) vs. Object-Oriented Programming (OOP)](#ecs-vs-object-oriented-programming-oop)
- [Graphics Library: SFML vs. SDL2](#graphics-library-sfml-vs-sdl2)
- [Networking Library: Boost.Asio vs. SFML Networking](#networking-library-boostasio-vs-sfml-networking)
- [Package Management with vcpkg and dependencies choice](#package-management-with-vcpkg-and-dependencies-choice)
- [Documentation with Markdown and Doxygen](#documentation-with-markdown-and-doxygen)
- [Development Tools](#development-tools)

---

### ECS vs. Object-Oriented Programming (OOP)
The **Entity-Component-System (ECS)** architecture was selected over traditional OOP because:

- **Separation of Concerns**: ECS separates data (components) from behavior (systems), providing flexibility and scalability. This makes managing complex systems easier as data and behavior are not bundled together as in OOP.
- **Dynamic Composition**: ECS allows dynamic changes to an entity’s behavior by adding or removing components, unlike OOP where inheritance can lead to rigid, hard-to-modify hierarchies.
- **Performance Benefits**: ECS’s data-oriented approach promotes better cache usage and reduces the overhead of object-oriented structures. This is particularly useful in gaming environments with many active entities.

Research suggests that while OOP focuses on the behavior and attributes of individual entities, ECS focuses on the data these entities represent, which can be more efficient and flexible for game development.

Sources:
- [flamendless.github.io: ecs-vs-oop](https://flamendless.github.io/ecs-vs-oop/)
- [theknowledgeacademy.com: entity-component-system](https://www.theknowledgeacademy.com/blog/entity-component-system/).

---

### Graphics Library: SFML vs. SDL2
We compared **SFML** and **SDL2** before choosing SFML for our project. Here’s a feature comparison:

| Feature               | SFML                                       | SDL2                                                 |
|-----------------------|--------------------------------------------|------------------------------------------------------|
| **Ease of Use**       | Simple, high-level API, quick setup        | Requires more setup and boilerplate                  |
| **Graphics Handling** | High-level 2D graphics API, OpenGL support | More low-level control but harder to set up OpenGL   |
| **Audio**             | Integrated, easy-to-use audio support      | Requires **SDL\_mixer** for advanced audio features  |
| **Networking**        | Built-in support for TCP/UDP               | No built-in networking; third-party libraries needed |
| **Platform Support**  | Cross-platform, but not as lightweight     | Very lightweight, optimized for performance          |

SFML was chosen because it provides a more streamlined development experience, especially for 2D games like R-Type. Its built-in support for graphics, audio, and networking saves time compared to SDL2, which often requires external libraries like SDL_mixer for advanced audio and third-party solutions for networking  .

---

### Networking Library: Boost.Asio vs. SFML Networking
We chose **Boost.Asio** over **SFML’s networking** for several reasons:

1. **Asynchronous I/O**: Boost.Asio offers robust asynchronous communication, which is crucial for handling real-time multiplayer interactions without blocking the game’s main thread. SFML’s networking, in contrast, is largely synchronous, which can cause performance issues in real-time games.
2. **Scalability and Multithreading**: Boost.Asio excels in managing multiple connections concurrently and efficiently supports multithreaded applications. This is vital for multiplayer games with a large number of players, while SFML’s simpler networking tools are less suited for scaling.
3. **Advanced Control**: Boost.Asio provides more control over low-level networking operations, allowing for custom protocols and advanced optimizations, which SFML lacks.

In summary, **Boost.Asio** offers the performance, flexibility, and scalability needed for a high-performance multiplayer game, which SFML’s built-in networking cannot match.

---

### Package Management with vcpkg and dependencies choice
We selected **vcpkg** as our package manager because it simplifies cross-platform dependency management on Windows and Linux. It allows the project to easily integrate and manage external libraries across different environments, reducing the configuration burden on developers. The key dependencies managed via vcpkg include:

- **Boost**: A versatile library offering utility features like json parser or non-standard library algorithms, which are usefully to speed up the project development.
- **libconfig**: Provides an easy-to-use API for managing game settings through configuration files, crucial for offering flexible and customizable game configurations.
- **libpng**: Ensures efficient handling of PNG images, which are commonly used for game assets due to their lossless compression.
- **SFML**: Chosen for its simplicity and cohesive APIs for graphics, audio, and networking, SFML significantly accelerates game development, making it easier to prototype and build game features.

---

### Documentation with Markdown and Doxygen
We use **Markdown** in combination with **Doxygen** for our documentation strategy to ensure simplicity, consistency, and accessibility:

- **Markdown** provides a lightweight syntax, allowing all team members to contribute to documentation without a steep learning curve.
- **Doxygen** enables automatic generation of documentation from source code comments, keeping the documentation in sync with the codebase.
- **GitHub Pages** is used to host the Doxygen-generated documentation, making it easily accessible to the team and the public. This combination ensures that documentation remains up to date and easy to maintain as the project grows.

---

## Development tools:

The project uses `clang-tidy` to enforce coding standards for local development. We also use **GitHub Actions** to implement CI/CD pipelines, which helps automate critical parts of the development process:
- **Cross-Platform Compilation**: The pipeline automatically checks that the project compiles correctly on both Windows and Linux, ensuring cross-platform compatibility.
- **Automatic Release Creation**: Upon successful builds, the pipeline generates release zip files for both Windows and Linux, simplifying the release process.
- **Documentation Deployment**: The pipeline automatically generates and deploys Doxygen documentation to GitHub Pages, ensuring up-to-date project documentation.

---

By leveraging ECS architecture, streamlined dependency management with vcpkg, a well-organized codebase, and robust CI/CD processes, the R-Type project is set up for scalable, maintainable development, ensuring a polished multiplayer game experience.