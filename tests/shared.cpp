#include <criterion/criterion.h>
#include "Registry.hpp"

/* Registry */

Test(shared, empty_registry) {
    ecs::Registry registry;
    cr_assert_eq(registry.getEntities().size(), 0);
}

Test(shared, add_entity) {
    auto registry = std::make_shared<ecs::Registry>();
    auto entity = registry->createEntity(registry);
    cr_assert_eq(registry->getEntities().size(), 1);
    cr_assert_eq(*registry->getEntities().begin(), entity);
}

Test(shared, remove_entity) {
    auto registry = std::make_shared<ecs::Registry>();
    auto entity = registry->createEntity(registry);
    registry->removeEntity(entity);
    cr_assert_eq(registry->getEntities().size(), 0);
}

Test(shared, is_entity_valid) {
    auto registry = std::make_shared<ecs::Registry>();
    auto entity = registry->createEntity(registry);
    cr_assert(registry->isEntityValid(entity));
    registry->removeEntity(entity);
    cr_assert_not(registry->isEntityValid(entity));
}

Test(shared, add_component) {
    auto registry = std::make_shared<ecs::Registry>();
    auto entity = registry->createEntity(registry);
    registry->addComponent<ecs::component::Position>(entity);
    cr_assert(registry->contains<ecs::component::Position>(entity));
}

Test(shared, set_component) {
    auto registry = std::make_shared<ecs::Registry>();
    auto entity = registry->createEntity(registry);
    ecs::component::Position position{10, 20};
    registry->setComponent<ecs::component::Position>(entity, position);
    cr_assert_eq(registry->getComponent<ecs::component::Position>(entity).x, 10);
    cr_assert_eq(registry->getComponent<ecs::component::Position>(entity).y, 20);
}

Test(shared, get_no_component) {
    auto registry = std::make_shared<ecs::Registry>();
    auto entity = registry->createEntity(registry);
    cr_assert_eq(registry->getNoComponent(entity), 0);
    registry->addComponent<ecs::component::Position>(entity);
    cr_assert_eq(registry->getNoComponent(entity), 1);
}

Test(shared, reset_entity) {
    auto registry = std::make_shared<ecs::Registry>();
    auto entity = registry->createEntity(registry);
    registry->addComponent<ecs::component::Position>(entity);
    registry->reset(entity);
    cr_assert_not(registry->contains<ecs::component::Position>(entity));
}
