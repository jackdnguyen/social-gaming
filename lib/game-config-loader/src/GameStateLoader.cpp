#include "GameStateLoader.h"
#include <iostream>
#include <cpp-tree-sitter.h>
#include <string>
#include<algorithm>
#include <queue>

namespace SYMBOL{
    const int NUMBER = 81;
    const int STRING = 84;
    const int BOOL = 123;
    const int LIST = 126;
    const int MAP = 128;
}

GameStateLoader::GameStateLoader(std::string_view source)
:source(source)
{}

bool GameStateLoader::isPrimitive(const ts::Node& node){
    int symbol = node.getSymbol();
    return symbol == SYMBOL::NUMBER || symbol == SYMBOL::STRING || symbol == SYMBOL::BOOL;
}

bool GameStateLoader::isList(const ts::Node& node){
    int symbol = node.getSymbol();
    return symbol == SYMBOL::LIST;
}

bool GameStateLoader::isMap(const ts::Node& node){
    int symbol = node.getSymbol();
    return symbol == SYMBOL::MAP;
}

std::unique_ptr<GameEnvironment::Value> GameStateLoader::convertNodeToValue(const ts::Node& node){
    auto value = std::make_unique<GameEnvironment::Value>(); 
    if (isPrimitive(node)){  
        value = convertNodeToPrimitive(node);
    }
    else if (isList(node)){
        value = convertNodeToList(node);
    }
    else if (isMap(node)){
        value = convertNodeToMap(node);
    }
    else{
        std::runtime_error("Not supported type");
    }
    return value;
}

std::unique_ptr<GameEnvironment::Value> GameStateLoader::convertNodeToPrimitive(const ts::Node& node){
    auto toReturn = std::make_unique<GameEnvironment::Value>();
    if (node.getSymbol() == SYMBOL::NUMBER){
        toReturn->value = std::stoi(std::string(node.getSourceRange(source)));
    }
    else if (node.getSymbol() == SYMBOL::BOOL){
        std::string upperCaseString(node.getSourceRange(source));
        std::transform(upperCaseString.begin(), upperCaseString.end(), upperCaseString.begin(), ::toupper);
        toReturn->value = (upperCaseString == "TRUE");
    }
    else if (node.getSymbol() == SYMBOL::STRING){
        toReturn->value = node.getSourceRange(source);
    }
    return toReturn;
}

std::unique_ptr<GameEnvironment::Value> GameStateLoader::convertNodeToList(const ts::Node& node){
    ts::Node listNode = node.getNamedChild(0);
    auto toReturn = std::make_unique<GameEnvironment::Value>();
    
    if(listNode.isNull()) {
        return toReturn;
    }
    
    auto list = std::make_unique<GameEnvironment::List>();
    for (uint32_t i = 0; i < listNode.getNumNamedChildren(); i++){
        ts::Node elementNode = listNode.getNamedChild(i).getNamedChild(0);
        auto value = convertNodeToValue(elementNode);
        list->value.push_back(std::move(value));
    }
    toReturn->value = std::move(list);
    return toReturn;
}

std::unique_ptr<GameEnvironment::Value> GameStateLoader::convertNodeToMap(const ts::Node& node){
    auto toReturn = std::make_unique<GameEnvironment::Value>();

    auto map = std::make_unique<GameEnvironment::Map>();

    for (uint32_t i = 0; i < node.getNumNamedChildren(); i++){
        ts::Node mapEntry = node.getNamedChild(i);
        ts::Node identifierNode = mapEntry.getNamedChild(0);
        ts::Node valueNode = mapEntry.getNamedChild(1);

        // Convert an identifier of a node to string 
        GameEnvironment::Identifier identifier = identifierNode.getSourceRange(source);
        ts::Node value = valueNode.getNamedChild(0);
        auto toStore = convertNodeToValue(value);
        map->value.insert(std::make_pair(identifier, std::move(toStore)));
    }
    
    toReturn->value = std::move(map);
    return toReturn;
}

std::unique_ptr<GameEnvironment::Environment> GameStateLoader::getEnvironment(const ts::Node& root){
    std::unique_ptr<GameEnvironment::Environment> environment = std::make_unique<GameEnvironment::Environment>();

    // Loop each map entry
    for (uint32_t i = 0; i < root.getNumNamedChildren(); i++){
        // Map enty has identifier should have identifier and expression which is a value of constant
        ts::Node mapEntry = root.getNamedChild(i);
        ts::Node identifierNode = mapEntry.getNamedChild(0);
        ts::Node valueNode = mapEntry.getNamedChild(1);

        // Convert an identifier of a node to string 
        std::string_view identifier = identifierNode.getSourceRange(source);

        ts::Node value = valueNode.getNamedChild(0);
        auto toStore = convertNodeToValue(value);
        environment->insert(std::make_pair(identifier, std::move(toStore)));
    }
    return environment;
}

// Print node by level order
void GameStateLoader::printByLevelOrder(const ts::Node& node){
    if (node.isNull()){
        return;
    }
    std::queue<ts::Node> nodeQueue;
    nodeQueue.push(node);

    while (!nodeQueue.empty()){
        int queueSize = nodeQueue.size();

        for (int i = 0; i < queueSize; i++){
            ts::Node node = nodeQueue.front();
            nodeQueue.pop();
            std::cout << node.getType() << ", ";
            for (uint32_t j = 0; j < node.getNumNamedChildren(); j++){
                nodeQueue.push(node.getNamedChild(j));
            }
        }
        std::cout << "\n";
    }
}