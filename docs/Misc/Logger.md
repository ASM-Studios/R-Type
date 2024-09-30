# Logger

The client logger help you to log messages in the console.

## Usage

To log a message, use the following code:

```c++
#include "Logger.hpp"

Logger::log(LogLevel::<LEVEL>, "Message");
```

`<LEVEL>` is the level of the message. It can be one of the following:
- `INFO`
- `WARNING`
- `ERROR`

The LogLevel passed in the `log` function is the minimum level to log.

### Warning

For performance reasons, do not use the `log` function in the main loop function.

### User log level

To set the user log level, please refer to the [Config](Config.md) documentation.
