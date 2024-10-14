# R-Type

This project aim to recreate the R-Type game in a multiplayer version.

It will emphasize on both the client and server side, with a focus on the network part.

The server will be developed has a game engine, and the client will be a simple game client.

The project is made solely in C++.

## Requirements

- Doxygen: [see documentation](https://www.doxygen.nl/manual/install.html)
- CMake: [see documentation](https://cmake.org/install/)
- C++ Compiler: [see documentation](https://gcc.gnu.org/install/)
- Ninja: [see documentation](https://ninja-build.org/)
- VCPKG: [see documentation](Misc/VCPKG.md)

## Installation

```sh
git clone git@github.com:EpitechPromo2027/B-CPP-500-LYN-5-1-rtype-mael.rabot.git && mv B-CPP-500-LYN-5-1-rtype-mael.rabot rtype
cd rtype
```

## Build

Executables are built in root folder.

### Debug

Use debug.sh script to create a debug build.

```sh
./debug.sh
```

or

```powershell
debug.bat
```

### Release

Use release.sh script to create a release build.

```sh
./release.sh
```

or

```powershell
release.bat
```

## Documentation

This project is documented using Doxygen.

You can find additional information in the [`docs`](docs/Hub.md) folder.

For developers, refer to the [Developpers.md](docs/Developers) file.

## Authors

| Name            | Github                                              |
|-----------------|-----------------------------------------------------|
| Mathieu Coulet  | [Djangss](https://github.com/Djangss)               |
| Mathieu Borel   | [mat0904](https://github.com/mat0904)               |
| Yohann Mangenot | [YohannMgt](https://github.com/YohannMgt)           |
| Charles Madjeri | [charlesmadjeri](https://github.com/charlesmadjeri) |
| Maël RABOT      | [Mael-RABOT](https://github.com/Mael-RABOT)         |
