# ECS

The ECS is the entity manager of the project (server-side).

## Usage

**NEVER INSTANCIATE AN ENTITY BY YOURSELF, USE `ecs::Registry::createEntity` INSTEAD.**<br>
Registry keep a trace of all created entities.

## Factory

Apart from specific cases, you do not need to manually create entities. Use the factory instead.<br>
The default factory is [LevelFactory](LevelFactory.md).

### Entity

To create an entity, use the following code:

```cpp
ecs::RegistryManager::getInstance().getRegistry().createEntity<>(0);
```

To add more component, you can either use

```cpp
entity.addComponent<Component1>();
```

or

```cpp
entity.addComponents<Component, Component2, ...>();
```

To check if an entity contains a component, use:

```cpp
entity.contains<Component1>();
```

To get a component, use:

```cpp
entity.getComponent<Component1>()
```

getComponent throw an error if the component doesn't exist in the entity.

```cpp
try {
	Component component = entity.getComponent<Component>();
} catch (ecs::Registry::NoComponent &error) {
    Logger::log(Logger::ERR, error.what());
}
```

### Registry

You don't need to keep a reference of entity. Entity is an "id" class.

ecs::Entity's methods are just "redirections" of ecs::Registry's methods.

For example,

```cpp
entity.contains<Component>();
```

stands for

```cpp
ecs::Registry::contains<Component>(entity);
```

All entities are stored in ecs::Registry class

To fetch them all, use

```cpp
const auto entities = ecs::RegistryManager::getInstance().getRegistry().getEntities();
```
