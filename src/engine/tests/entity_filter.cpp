#include "engine/entity_filter.h"

#include "engine/entity_manager.h"
#include "engine/tests/test_component.h"
#include "util/make_unique.h"

#include <gtest/gtest.h>

using namespace thrive;

namespace {

class TestEngine : public Engine { };

}

TEST(EntityFilter, Initialization) {
    TestEngine engine;
    engine.init();
    // Add component
    Entity::Id entityId = Entity::generateNewId();
    EntityManager::instance().addComponent(
        entityId,
        make_unique<TestComponent<0>>()
    );
    engine.update();
    // Set up filter
    EntityFilter<TestComponent<0>> filter;
    filter.setEngine(&engine);
    // Check filter
    auto filteredEntities = filter.entities();
    EXPECT_EQ(1, filteredEntities.count(entityId));
    EXPECT_EQ(1, filteredEntities.size());
}

TEST(EntityFilter, Single) {
    TestEngine engine;
    engine.init();
    // Set up filter
    EntityFilter<TestComponent<0>> filter;
    filter.setEngine(&engine);
    // Add component
    Entity::Id entityId = Entity::generateNewId();
    EntityManager::instance().addComponent(
        entityId,
        make_unique<TestComponent<0>>()
    );
    engine.update();
    // Check filter
    auto filteredEntities = filter.entities();
    EXPECT_EQ(1, filteredEntities.count(entityId));
    EXPECT_EQ(1, filteredEntities.size());
    // Remove component
    EntityManager::instance().removeComponent(
        entityId,
        TestComponent<0>::TYPE_ID
    );
    engine.update();
    // Check filter
    filteredEntities = filter.entities();
    EXPECT_EQ(0, filteredEntities.count(entityId));
    EXPECT_EQ(0, filteredEntities.size());
}


TEST(EntityFilter, Multiple) {
    TestEngine engine;
    engine.init();
    // Set up filter
    EntityFilter<
        TestComponent<0>,
        TestComponent<1>
    > filter;
    filter.setEngine(&engine);
    auto filteredEntities = filter.entities();
    // Add first component
    Entity::Id entityId = Entity::generateNewId();
    EntityManager::instance().addComponent(
        entityId,
        make_unique<TestComponent<0>>()
    );
    engine.update();
    // Check filter
    filteredEntities = filter.entities();
    // Add first component
    EXPECT_EQ(0, filteredEntities.count(entityId));
    EXPECT_EQ(0, filteredEntities.size());
    // Add second component
    EntityManager::instance().addComponent(
        entityId,
        make_unique<TestComponent<1>>()
    );
    engine.update();
    // Check filter
    filteredEntities = filter.entities();
    EXPECT_EQ(1, filteredEntities.count(entityId));
    EXPECT_EQ(1, filteredEntities.size());
    // Remove component
    EntityManager::instance().removeComponent(
        entityId,
        TestComponent<1>::TYPE_ID
    );
    engine.update();
    // Check filter
    filteredEntities = filter.entities();
    EXPECT_EQ(0, filteredEntities.count(entityId));
    EXPECT_EQ(0, filteredEntities.size());
}


TEST(EntityFilter, Optional) {
    TestEngine engine;
    engine.init();
    using TestFilter = EntityFilter<
        TestComponent<0>,
        Optional<TestComponent<1>>
    >;
    // Set up filter
    TestFilter filter;
    filter.setEngine(&engine);
    TestFilter::EntityMap filteredEntities = filter.entities();
    // Add first component
    Entity::Id entityId = Entity::generateNewId();
    EntityManager::instance().addComponent(
        entityId,
        make_unique<TestComponent<0>>()
    );
    engine.update();
    // Check filter
    filteredEntities = filter.entities();
    EXPECT_EQ(1, filteredEntities.count(entityId));
    EXPECT_EQ(1, filteredEntities.size());
    // Check group
    TestFilter::ComponentGroup group = filteredEntities[entityId];
    EXPECT_TRUE(std::get<0>(group) != nullptr);
    EXPECT_TRUE(std::get<1>(group) == nullptr);
    // Add second component
    EntityManager::instance().addComponent(
        entityId,
        make_unique<TestComponent<1>>()
    );
    engine.update();
    // Check filter
    filteredEntities = filter.entities();
    EXPECT_EQ(1, filteredEntities.count(entityId));
    EXPECT_EQ(1, filteredEntities.size());
    // Check group
    group = filteredEntities[entityId];
    EXPECT_TRUE(std::get<0>(group) != nullptr);
    EXPECT_TRUE(std::get<1>(group) != nullptr);
    // Remove component
    EntityManager::instance().removeComponent(
        entityId,
        TestComponent<1>::TYPE_ID
    );
    engine.update();
    // Check filter
    filteredEntities = filter.entities();
    EXPECT_EQ(1, filteredEntities.count(entityId));
    EXPECT_EQ(1, filteredEntities.size());
    // Check group
    group = filteredEntities[entityId];
    EXPECT_TRUE(std::get<0>(group) != nullptr);
}


TEST(EntityFilter, OptionalOnly) {
    TestEngine engine;
    engine.init();
    using TestFilter = EntityFilter<
        Optional<TestComponent<0>>
    >;
    // Set up filter
    TestFilter filter;
    filter.setEngine(&engine);
    TestFilter::EntityMap filteredEntities = filter.entities();
    // Add first component
    Entity::Id entityId = Entity::generateNewId();
    EntityManager::instance().addComponent(
        entityId,
        make_unique<TestComponent<1>>()
    );
    engine.update();
    // Check filter
    filteredEntities = filter.entities();
    EXPECT_EQ(0, filteredEntities.count(entityId));
    EXPECT_EQ(0, filteredEntities.size());
    // Check group
    TestFilter::ComponentGroup group = filteredEntities[entityId];
    EXPECT_TRUE(std::get<0>(group) == nullptr);
}
