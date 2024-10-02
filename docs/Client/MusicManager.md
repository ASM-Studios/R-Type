# Music Manager

The `MusicManager` class is responsible for managing the background music in the game. It handles loading, playing, and controlling the volume of the music tracks.

## Configuration File

The `MusicManager` uses a configuration file to load the music tracks. The default path for this configuration file is `client/assets/musics_config.cfg`. This path can be changed in the `client/config.json` file.

### Configuration File Format

The configuration file is a `.cfg` file with the following format:

```cfg
musics = (
    {
        name = "main_theme";
        path = "client/assets/musics/main_theme.ogg";
    }
);
```

- name: The name of the music track.
  - This name is the one that should be used with the `setMusic` method. 
- path: The relative path to the music file.

## Example Configuration File

```cfg
musics = (
    {
        name = "main_theme";
        path = "client/assets/musics/main_theme.ogg";
    },
    {
        name = "battle_theme";
        path = "client/assets/musics/battle_theme.ogg";
    }
);
```

## Usage

To use the MusicManager class, follow these steps:  

1. Include the Header: Include the MusicManager.hpp header file in your source file.  
    ```cpp
    #include "MusicManager.hpp"
    ```
2. Instantiate the MusicManager: Create an instance of the MusicManager class.  
    ```cpp
    GUI::MusicManager musicManager;
    ```
3. Set Music: Use the setMusic method to play a music track.  
    ```cpp
    musicManager.setMusic("main_theme");
    ```
4. Control Volume: Use the setVolume and getVolume methods to control the volume.  
    ```cpp
    musicManager.setVolume(75.0f);
    float currentVolume = musicManager.getVolume();
   ```
5. Mute/Unmute: Use the toggleMute and setMute methods to mute or unmute the music.  
    ```cpp
    musicManager.toggleMute();
    musicManager.setMute(true);
    ```
