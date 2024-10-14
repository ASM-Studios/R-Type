#include "EntitySchematic.hpp"
#include "RegistryManager.hpp"
#include "Tags.hpp"
#include "WindowManager.hpp"
#include "query/Header.hpp"
#include "query/NonTypedQuery.hpp"
#include "query/Payloads.hpp"
#include "query/RawRequest.hpp"
#include "query/TypedQuery.hpp"

static void updatePlayer(network::Client client, RawRequest request) {
    TypedQuery<UpdatePlayer> query = request.getQuery();
    ecs::Entity entity(0, ecs::RegistryManager::getInstance().getRegistry());
    ecs::RegistryManager::getInstance().getRegistry().setComponent(entity, query.getPayload().position);
}

static void updateTeamPlayer(network::Client client, RawRequest request) {
    Logger::log(LogLevel::ERR, "Receiving pos");
    TypedQuery<UpdateTeamPlayer> query = request.getQuery();
    ecs::Entity entity(query.getPayload().id, ecs::RegistryManager::getInstance().getRegistry());
    ecs::RegistryManager::getInstance().getRegistry().setComponent(entity, query.getPayload().input);
    ecs::RegistryManager::getInstance().getRegistry().setComponent(entity, query.getPayload().position);
}

static void createEntity(network::Client client, RawRequest request) {
    TypedQuery<CreateEntity> tq = request.getQuery();
    auto payload = tq.getPayload();
    if (ecs::component::Tags::hasTag(payload.tags, ecs::component::Tag::Ally) && !ecs::component::Tags::hasTag(payload.tags, ecs::component::Tag::Bullet)) {
        ecs::Entity entity = EntitySchematic::createTeamPlayerClient(payload.id, payload.position.x, payload.position.y, payload.spriteID);
    }
}

const std::map<RequestType, void (*)(network::Client client, RawRequest rawRequest)> requestAction = {
    {UPDATE_PLAYER, &updatePlayer},
    {UPDATE_TEAM_PLAYER, &updateTeamPlayer},
    {CREATE_ENTITY, &createEntity},
};

int main() {
    try {
        GUI::WindowManager windowManager;
        windowManager.run();
    } catch (const GUI::GuiException& e) {
        Logger::log(LogLevel::ERR, e.what());
        return 1;
    }
    return 0;
}
