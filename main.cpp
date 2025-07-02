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

    if (argc < 2)
    {
        Commands::GuessTheWord(j);
    }
    else if (std::strcmp(argv[1], "add") == 0)
    {
        Commands::AddWord(j, argc, argv);

        std::ofstream ofile{ JSON_PATH };
        ofile << j.dump(4);
    }
    else if (std::strcmp(argv[1], "show") == 0)
    {
        Commands::Show(j);
    }
    else if (std::strcmp(argv[1], "edit") == 0)
    {
        Commands::Edit(j, argc, argv);

        std::ofstream ofile{ JSON_PATH };
        ofile << j.dump(4);
    }
    else if (std::strcmp(argv[1], "delete") == 0)
    {
        Commands::Delete(j, argc, argv);

        std::ofstream ofile{ JSON_PATH };
        ofile << j.dump(4);
    }

    return 0;
}

