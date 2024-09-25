# Config

The client configuration is stored in the [`client/config.json`](../../client/config.json) file.

Fields aren't enforced, but the following fields are used by the client:

- "sprites_path": The path to the sprites' directory.
- "sprites_config_path": The path to the sprites' configuration file.
  - By default: `client/assets/sprites_config.cfg`.
- "fullscreen":
  - `true` | `false`
- "width": The window's width.
  - By default: Screen width.
- "height": The window's height.
  - By default: Screen height.
- "logLevel":
  - `NONE` | `INFO` | `WARNING` | `ERROR`
- "frameRateLimit": The frame rate limit.
  - By default: 60

## JSON Example

```json
{
  "sprites_path": "client/assets/textures/",
  "sprites_config_path": "client/assets/sprites_config.cfg",
  "fullscreen": false,
  "width": 1920,
  "height": 1080,
  "logLevel": "ERROR",
  "frameRateLimit": 60
}
```

## Usage

To access the configuration, use the following code:

```c++
#include "Config.hpp"

Config& config = Config::getInstance("client/config.json");

const auto value = config.get("value").value_or("default");
```

"value" is the key in the JSON file.<br>
"default" is the default value if the key doesn't exist.
