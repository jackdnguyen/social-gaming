#ifndef GAME_SETUP_LOADER
#define GAME_SETUP_LOADER

#include<string_view>
#include<vector>
#include <memory>
#include <variant>
#include <map>
#include <cpp-tree-sitter.h>
enum KIND{
    INTEGER, STRING, BOOLEAN, ENUM,
};
using Range = std::pair<int,int>;
using ChoiceList = std::map<std::string_view,std::string_view>;
using Domain = std::variant<Range, ChoiceList> ;

class Setup{
public:
    Setup(std::string_view identifier,KIND kind,std::string_view prompt);
    Setup(std::string_view identifier,KIND kind,std::string_view prompt,std::string_view restInfo, Domain domain);

    bool checkResponse(std::string_view response);
    void intProcess();
    void boolProcess();
    void strProcess();
    void enumProcess();
    void processSetup();
    void print();
private:
    std::string_view identifier;
    KIND kind;
    std::string_view prompt;
    //may change to other type;
    std::string_view restInfo;
    Domain domain;

};
struct DomainPrinter {
    void operator()(const Range& range) const ;

    void operator()(const ChoiceList& choices) const ;
};

//for check the response from thr user
struct DomainChecker{
    bool operator()(const Range& range, std::string_view input ) const ;

    bool operator()(const ChoiceList& choices, std::string_view input) const ;
};

class GameSetupLoader{
public:
    GameSetupLoader(std::string_view source);
    std::unique_ptr<Setup> convertNodetoSetup(const ts::Node& node);
    std::vector<std::unique_ptr<Setup>>getGameSetup (const ts::Node& node);
private:
    std::string_view source;

};


#endif