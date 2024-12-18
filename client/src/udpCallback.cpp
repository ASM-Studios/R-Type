#include "EntitySchematic.hpp"
#include "GameLogicManager.hpp"
#include "RegistryManager.hpp"
#include "Tags.hpp"
#include "WindowManager.hpp"
#include "query/Header.hpp"
#include "query/NonTypedQuery.hpp"
#include "query/Payloads.hpp"
#include "query/RawRequest.hpp"
#include "query/TypedQuery.hpp"
#include <cstdio>
#include "SystemMetrics.hpp"

int pingValue = 0;

static void handleUpdatePlayer(std::shared_ptr<network::Client> client, RawRequest request) {
    TypedQuery<UpdatePlayer> query = request.getQuery();
    ecs::Entity entity(0, ecs::RegistryManager::getInstance().getRegistry(0));
    ecs::RegistryManager::getInstance().getRegistry(0)->setComponent(entity, query.getPayload().position);
}

static void handleUpdateTeamPlayer(std::shared_ptr<network::Client> client, RawRequest request) {
    TypedQuery<UpdateTeamPlayer> query = request.getQuery();
    ecs::Entity entity(query.getPayload().id, ecs::RegistryManager::getInstance().getRegistry(0));
    //ecs::RegistryManager::getInstance().getRegistry().setComponent(entity, query.getPayload().input);
    ecs::RegistryManager::getInstance().getRegistry(0)->setComponent(entity, query.getPayload().position);
}

static void handleCreateEntity(std::shared_ptr<network::Client> client, RawRequest request) {
    TypedQuery<CreateEntity> tq = request.getQuery();
    CreateEntity payload = tq.getPayload();
    auto registry = ecs::RegistryManager::getInstance().getRegistry(0);

    if (ecs::component::Tags::hasTag(payload.tags, ecs::component::Tag::Ally) && ecs::component::Tags::hasTag(payload.tags, ecs::component::Tag::Player)) {
        Logger::log(LogLevel::INFO, std::format("Creating new entity of id {}", payload.id));
        ecs::Entity player = EntitySchematic::createTeamPlayerClient(registry, payload.id, payload.position.x, payload.position.y, payload.spriteID);
        return;
    }
    if (ecs::component::Tags::hasTag(payload.tags, ecs::component::Tag::Ally) && ecs::component::Tags::hasTag(payload.tags, ecs::component::Tag::Bullet)) {
        ecs::Entity shooter = ecs::Entity(client->getID(), ecs::RegistryManager::getInstance().getRegistry(0));
        ecs::Entity bullet = EntitySchematic::createBullet(registry);
        ecs::RegistryManager::getInstance().getRegistry(0)->setComponent(bullet, payload.position);
        return;
    }
    if (ecs::component::Tags::hasTag(payload.tags, ecs::component::Tag::Enemy)) {
        TypedQuery<CreateEnemyEntity> tq = request.getQuery();
        auto fullPayload = (static_cast<TypedQuery<CreateEnemyEntity>>(request.getQuery())).getPayload();
        ecs::Entity enemy = EntitySchematic::createEnemy(registry, payload.id, payload.position.x, payload.position.y, payload.spriteID, payload.stateID, ecs::component::AIModelToString.at(fullPayload.model), getResolution());
    }
}

static void handleDestroyEntity(std::shared_ptr<network::Client> client, RawRequest request) {
    TypedQuery<DestroyEntity> tq = request.getQuery();
    auto registry = ecs::RegistryManager::getInstance().getRegistry(0);
    ecs::Entity entity(tq.getPayload().id, registry);
    registry->removeEntity(entity);
}

static void handlePing(std::shared_ptr<network::Client> client, RawRequest request) {
    auto timestamp = std::chrono::system_clock::now().time_since_epoch();
    TypedQuery<decltype(timestamp)> typedQuery = request.getQuery();
    timestamp = timestamp - typedQuery.getPayload();
    pingValue = std::chrono::duration_cast<std::chrono::milliseconds>(timestamp).count();
}

const std::map<RequestType, void (*)(std::shared_ptr<network::Client> client, RawRequest rawRequest)> udpRequestAction = {
    {RequestType::UPDATE_PLAYER, &handleUpdatePlayer},
    {RequestType::UPDATE_TEAM_PLAYER, &handleUpdateTeamPlayer},
    {RequestType::CREATE_ENTITY, &handleCreateEntity},
    {RequestType::DESTROY_ENTITY, &handleDestroyEntity},
    {RequestType::PING, &handlePing}
};
