# ECS

The ECS is the entity manager of the project (server-side).

## Usage

NEVER INSTANCIATE AN ENTITY BY YOURSELF, USE `ecs::Registry::createEntity` INSTEAD.
Registry keep a trace of all created entities.

### Entity

To create an entity, use the following code:

```cpp
//proto
template <typename... Components>
static ecs::Entity ecs::Registry::createEntity();

//usage
auto entity = ecs::Registry::createEntity<Component1, Component2, ...>();
```

To add more component, you can either use

```cpp
//proto
template <typename Component>
void ecs::Entity::addComponent();

//usage
entity.addComponent<Component1>();
```

or

```cpp
//proto
template <typename... Components>
void ecs::Entity::addComponents();

//usage
entity.addComponents<Component, Component2, ...>();
```

To check if an entity contains a component, use:

```cpp
//proto
template <typename Component>
bool ecs::Entity::contains();

//usage
entity.contains<Component1>();
```

To get a component, use:

```cpp
//proto
template <typename Component>
Component& ecs::Entity::getComponent();

//usage
entity.getComponent<Component1>()
```

getComponent throw an error if the component doesn't exist in the entity.

```cpp
try {
	Component component = entity.getComponent<Component>();
} catch (ecs::Registry::NoComponent &error) {
	std::cerr << e.what() << std::endl;
}
```

### Registry

All ecs::Registry's methods are static.

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
//proto
static std::set<ecs::Entity> ecs::Registry::getEntities();

//usage
auto entities = ecs::Registry::getEntities();
```
