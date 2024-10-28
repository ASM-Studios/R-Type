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

static void handleUpdatePlayer(std::shared_ptr<network::Client> client, RawRequest request) {
    TypedQuery<UpdatePlayer> query = request.getQuery();
    ecs::Entity entity(0, ecs::RegistryManager::getInstance().getRegistry());
    ecs::RegistryManager::getInstance().getRegistry().setComponent(entity, query.getPayload().position);
}

static void handleUpdateTeamPlayer(std::shared_ptr<network::Client> client, RawRequest request) {
    TypedQuery<UpdateTeamPlayer> query = request.getQuery();
    ecs::Entity entity(query.getPayload().id, ecs::RegistryManager::getInstance().getRegistry());
    ecs::RegistryManager::getInstance().getRegistry().setComponent(entity, query.getPayload().input);
    ecs::RegistryManager::getInstance().getRegistry().setComponent(entity, query.getPayload().position);
}

static void handleCreateEntity(std::shared_ptr<network::Client> client, RawRequest request) {
    TypedQuery<CreateEntity> tq = request.getQuery();
    auto payload = tq.getPayload();
    if (ecs::component::Tags::hasTag(payload.tags, ecs::component::Tag::Ally) && ecs::component::Tags::hasTag(payload.tags, ecs::component::Tag::Player)) {
        ecs::Entity entity = EntitySchematic::createTeamPlayerClient(payload.id, payload.position.x, payload.position.y, payload.spriteID);
        return;
    }
    if (ecs::component::Tags::hasTag(payload.tags, ecs::component::Tag::Ally) && ecs::component::Tags::hasTag(payload.tags, ecs::component::Tag::Bullet)) {
        ecs::Entity shooter = ecs::Entity(client->getID(), ecs::RegistryManager::getInstance().getRegistry());
        ecs::Entity bullet = EntitySchematic::createBullet();
        ecs::RegistryManager::getInstance().getRegistry().setComponent(bullet, payload.position);
        return;
    }
}

static void handlePing(std::shared_ptr<network::Client> client, RawRequest request) {
   auto timestamp = std::chrono::system_clock::now().time_since_epoch();
    TypedQuery<decltype(timestamp)> typedQuery = request.getQuery();
    timestamp = timestamp - typedQuery.getPayload();
    int ping = std::chrono::duration_cast<std::chrono::milliseconds>(timestamp).count();
    Logger::log(LogLevel::INFO, std::format("Ping {} ms", ping)); //TODO CHANGE FOR THE FUTURE
}

static void handleStart(std::shared_ptr<network::Client> client, RawRequest request) {
    Logger::log(LogLevel::ERR, "STARTING");
    GameLogicManager::getInstance().get().start();
}

const std::map<RequestType, void (*)(std::shared_ptr<network::Client> client, RawRequest rawRequest)> udpRequestAction = {
    {RequestType::UPDATE_PLAYER, &handleUpdatePlayer},
    {RequestType::UPDATE_TEAM_PLAYER, &handleUpdateTeamPlayer},
    {RequestType::CREATE_ENTITY, &handleCreateEntity},
    {RequestType::PING, &handlePing},
    {RequestType::START, &handleStart}
};
