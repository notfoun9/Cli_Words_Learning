#include "src/3rd_party/json.hpp"
#include "src/commands.h"
#include <cstring>
#include <cstdlib>

using nlohmann::json;

json GetJson()
{
    std::ifstream input;

    input.open(JSON_PATH);
    
    if (input.fail())
    {
        if (!std::filesystem::exists(DIRECTORY_PATH))
        {
            std::filesystem::create_directory(DIRECTORY_PATH);
        }

        std::ofstream newFile{ JSON_PATH };
        newFile << "[\n]" << std::endl;
        newFile.close();

        input.open(JSON_PATH);
    }
    assert(input.fail() == false);

    json res;
    input >> res;

    return res;
}

int main(int argc, char** argv)
{
    json j = GetJson();
    auto dict = j.get<Dictionary>();

    if (argc < 2)
    {
        Commands::GuessTheWord(dict);
    }
    else if (std::strcmp(argv[1], "--add") == 0 || std::strcmp(argv[1], "-a") == 0)
    {
        Commands::AddWord(j, argc, argv);
    }
    else if (std::strcmp(argv[1], "--show") == 0 || std::strcmp(argv[1], "-s") == 0)
    {
        Commands::Show(dict);
    }

    return 0;
}

