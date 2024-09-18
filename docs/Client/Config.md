# Config

The client configuration is stored in the [`client/config.json`](../../client/config.json) file.

Fields aren't enforced, but the following fields are used by the client:

- "sprites_path": The path to the sprites' directory.
- "sprites_config_path": The path to the sprites' configuration file.
  - By default: `client/assets/sprites_config.cfg`.
- "fullscreen":
  - `true` | `false`
- "logLevel":
  - `NONE` | `INFO` | `WARNING` | `ERROR`

## JSON Example

```json
{
  "sprites_path": "client/assets/sprites/",
  "sprites_config_path": "client/assets/sprites_config.cfg",
  "fullscreen": false,
  "logLevel": "ERROR"
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
