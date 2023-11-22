#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <algorithm>
#include <vector>
#include <string_view>
#include <cstdint>
#include "Server.h"

using networking::Connection;

enum class Role : char {
    OWNER, 
    PLAYER, 
    AUDIENCE, 
    NONE
};

enum class UserState : char {
    NEW, 
    INTRO, 
    JOIN_GAME, 
    GAME_SELECT, 
    GAME_CONFIG,
    GAME_WAIT,
    GAME_RUN,
    NONE
};


struct User {
    Connection userID;
    std::string username;
    Role role;
    UserState state;
    uint16_t roomCode;

    User(Connection userID) : userID{userID}, state{UserState::NEW} {};
};

class UserManager {
public:
    UserManager() {};

    void addUser(Connection userID);
    void removeUser(Connection userID);

    void setUserName(Connection userID, std::string_view username);
    void setUserRole(Connection userID, Role role);
    void setUserRoomCode(Connection userID, uint16_t roomCode);
    void setUserState(Connection userID, UserState state);

    std::vector<User> getAllUsers() const;
    std::vector<User> getUsersInGame(uint16_t roomCode) const;
    User getUserByID(Connection userID) const;
    User getRoomOwner(uint16_t roomCode) const;

private:
    std::vector<User> users;
    std::vector<User>::iterator getUserItrByID(Connection userID);
};

#endif