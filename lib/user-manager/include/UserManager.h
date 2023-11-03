#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <algorithm>
#include <vector>
#include <string_view>
#include <cstdint>
#include "Server.h"

using networking::Connection;

enum class Role : char {
    OWNER, PLAYER, AUDIENCE, NONE
};

struct User {
    Connection userID;
    std::string_view username;
    Role role;
    uint8_t roomCode;

    User(Connection userID) : userID{userID} {};
};

class UserManager {
public:
    UserManager() {};
    void addUser(Connection userID);
    void setUserName(Connection userID, std::string_view username);
    void setUserRole(Connection userID, Role role);
    void setUserRoomCode(Connection userID, uint8_t roomCode);
    void removeUser(Connection userID);
    std::vector<User> getUsersInGame(Connection userID);
    uint8_t getUserGameCode(Connection userID);
    std::vector<User> getAllUsers() const { return users; }
    std::vector<User>::iterator findUserByID(Connection userID);

private:
    std::vector<User> users;
};

#endif