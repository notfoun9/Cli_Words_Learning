#include "src/3rd_party/json.hpp"
#include "src/commands.h"
#include <cstring>
#include <cstdlib>

using nlohmann::json;

json GetJson(const std::filesystem::path& path)
{
    std::ifstream input;

    input.open(path);
    if (input.fail())
    {
        if (!std::filesystem::exists(PARENT_DIR))
        {
            std::filesystem::create_directory(PARENT_DIR);
        }

        std::ofstream newFile{ path };
        input.open(path);
    }
    assert(input.fail() == false);

    json res;
    input >> res;

    return res;
}

int main(int argc, char** argv)
{
    json dictionaryJson = GetJson(DICTIONARY_JSON_PATH);
    if (dictionaryJson.empty())
    {
        dictionaryJson = Dictionary();
        std::ofstream ofile{ DICTIONARY_JSON_PATH };
        ofile << dictionaryJson.dump(4);
    }

    json activeGroupJson = GetJson(ACTIVE_GROUP_JSON_PATH);
    if (activeGroupJson.empty())
    {
        activeGroupJson = Group();
        std::ofstream ofile{ ACTIVE_GROUP_JSON_PATH };
        ofile << activeGroupJson.dump(4);
    }

    if (argc < 2)
    {
        Commands::GuessTheWord(activeGroupJson);
    }
    else if (std::strcmp(argv[1], "add") == 0)
    {
        Commands::AddWord(activeGroupJson, argc, argv);

        std::ofstream ofile{ ACTIVE_GROUP_JSON_PATH };
        ofile << activeGroupJson.dump(4);
    }
    else if (std::strcmp(argv[1], "show") == 0)
    {
        Commands::Show(activeGroupJson);
    }
    else if (std::strcmp(argv[1], "edit") == 0)
    {
        Commands::Edit(activeGroupJson, argc, argv);

        std::ofstream ofile{ ACTIVE_GROUP_JSON_PATH };
        ofile << activeGroupJson.dump(4);
    }
    else if (std::strcmp(argv[1], "delete") == 0)
    {
        Commands::Delete(activeGroupJson, argc, argv);

        std::ofstream ofile{ ACTIVE_GROUP_JSON_PATH };
        ofile << activeGroupJson.dump(4);
    }

    return 0;
}

