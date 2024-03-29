#include "GameCommunicator.h"

// game instance sends the game code (which is also the room Code) with the choices for input
// the function stores the actions along with the roomCode in a vector
void 
GameCommunicator::setGameChoices(uint16_t roomCode, const std::vector<std::string_view>& choices) {
    GameChoices currentGameChoices;
    currentGameChoices.roomCode = roomCode;
    currentGameChoices.choices = choices;

    gameChoices.push_back(currentGameChoices);
}

// stores the current valid user input for a particular user
void 
GameCommunicator::storeCurrentUserInput(const Message& message) {

    auto it = std::find_if(userInputs.begin(), userInputs.end(), [message](const UserInput& userInput) {
        return userInput.userID == message.connection;
    });

    if (it != userInputs.end()) {
        it->input = message.text;
    }
    else {
        UserInput usrInput;
        usrInput.userID = message.connection;
        usrInput.input = message.text;
        userInputs.push_back(usrInput);
    }
}

void 
GameCommunicator::setGameMessage(std::map<UserId, std::string_view> outgoing) {
    for (const auto& [user, message] : outgoing) {
        Message g_message;
        g_message.connection = user;
        g_message.text = message;
        gameMessages.push_back(g_message);
    }
}

// returns the available choices for the game inputs 
std::vector<std::string_view> 
GameCommunicator::getChoicesForUser(uint16_t roomCode) const {

    auto it = std::find_if(gameChoices.begin(), gameChoices.end(), [roomCode](const GameChoices& g_choices) {
        return g_choices.roomCode == roomCode;
    });

    if (it != gameChoices.end()) {
        return it->choices;
    }

    return {};
}

// returns the valid current user input, which will be sent to the game instance in server manager
std::string_view
GameCommunicator::getCurrentUserInput(UserId userID) const {
    
    auto it = std::find_if(userInputs.begin(), userInputs.end(), [userID](const UserInput& userInput) {
        return userInput.userID == userID;
    });

    if (it != userInputs.end()) {
        return it->input;
    }

    return "";
}


