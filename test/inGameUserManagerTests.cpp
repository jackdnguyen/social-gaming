#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "InGameUserManager.h"
TEST(InGameUserManagerTests, BasicUserCount){
    InGameUserManager inGameUManager;
    uint32_t dummyUserID = 1;
    Role dummyRole = Role::PLAYER;
    GameEnvironment::Environment dummyEnvironment;
    
    uint32_t dummyUserID2 = 2;
    Role dummyRole2 = Role::PLAYER;
    GameEnvironment::Environment dummyEnvironment2;

    inGameUManager.addNewUser(dummyUserID, dummyRole, std::move(dummyEnvironment));
    inGameUManager.addNewUser(dummyUserID2, dummyRole2, std::move(dummyEnvironment2));

    ASSERT_EQ(std::move(inGameUManager.getAllUserStates()).size(), 2);
}

TEST(InGameUserManagerTests, CanDeleteAllUsers){
    InGameUserManager inGameUManager;
    uint32_t dummyUserID = 1;
    Role dummyRole = Role::PLAYER;
    GameEnvironment::Environment dummyEnvironment;
    
    uint32_t dummyUserID2 = 2;
    Role dummyRole2 = Role::PLAYER;
    GameEnvironment::Environment dummyEnvironment2;
    
    inGameUManager.addNewUser(dummyUserID, dummyRole, std::move(dummyEnvironment));
    inGameUManager.addNewUser(dummyUserID2, dummyRole2, std::move(dummyEnvironment2));

    inGameUManager.deleteUser(dummyUserID);
    inGameUManager.deleteUser(dummyUserID2);
    ASSERT_EQ(std::move(inGameUManager.getAllUserStates()).size(), 0);
}

TEST(InGameUserManagerTests, CanGetStatesOfUser){
    InGameUserManager inGameUManager;
    uint32_t dummyUserID = 1;
    Role dummyRole = Role::PLAYER;
    
    // We care about using the Identifier so that we can find the user's Value.
    GameEnvironment::Identifier testIdentifier = "testidentifier";
    std::unique_ptr<GameEnvironment::Value> testValue;
    std::map<GameEnvironment::Identifier, std::unique_ptr<GameEnvironment::Value>> testMap;
    testMap.insert({testIdentifier, std::move(testValue)});

    GameEnvironment::Environment nonNullDummyEnvironment = std::move(testMap);
    inGameUManager.addNewUser(dummyUserID, dummyRole, std::move(nonNullDummyEnvironment));
    GameEnvironment::Environment testReturnedStates = inGameUManager.getStatesOfUser(dummyUserID);
    ASSERT_TRUE(std::move(testReturnedStates).find(testIdentifier) != testReturnedStates.end());
}

// Test overwriting an Environment with nothing in it, with an Environment that does have something in it.
TEST(InGameUserManagerTests, CanSetStatesOfExistingUser){
    InGameUserManager inGameUManager;
    uint32_t dummyUserID = 1;
    Role dummyRole = Role::PLAYER;
    
    // We care about using the Identifier so that we can find the user's Value.
    GameEnvironment::Identifier testIdentifier = "testidentifier";

    GameEnvironment::Environment nullEnvironment;

    // Load Value with an int which nonNullDummyEnvironment will get (and nullEnvironment won't)
    GameEnvironment::Value testValue;
    testValue.value = 500;
    std::unique_ptr<GameEnvironment::Value> testValuePtr;
    testValuePtr -> value;
    
    std::map<GameEnvironment::Identifier, std::unique_ptr<GameEnvironment::Value>> testMap;
    testMap.insert({testIdentifier, std::move(testValuePtr)});

    // Begins with nullEnvironment which doesn't have any elements in its map.
    GameEnvironment::Environment nonNullDummyEnvironment = std::move(testMap);
    inGameUManager.addNewUser(dummyUserID, dummyRole, std::move(nullEnvironment));

    inGameUManager.setStatesOfUser(dummyUserID, dummyRole, std::move(nonNullDummyEnvironment));
    GameEnvironment::Environment testReturnedStates = inGameUManager.getStatesOfUser(dummyUserID);

    // These prove that the testIdentifier can be found in testReturnedStates but not nullEnvironmentStates
    // Without using the setStatesOfUser function and therefore nonNullDummyEnvironment, the next assert would fail.
    ASSERT_TRUE(testReturnedStates.find(testIdentifier) != testReturnedStates.end());
    ASSERT_TRUE(nullEnvironment.find(testIdentifier) == nullEnvironment.end());

    // Finally, this test proves that testResturnedStates's first element
    // (testIdentifier) is not equivalent to nullEnvironment's first element (which has nothing).
    ASSERT_TRUE(testReturnedStates.begin() != nullEnvironment.begin());
}