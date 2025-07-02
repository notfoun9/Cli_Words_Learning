#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>

const auto  DIRECTORY_PATH = std::filesystem::path{getenv("HOME")} / ".config" / "words";
const auto  JSON_PATH      = DIRECTORY_PATH / "words.json";
const auto  TMP_FILE_PATH  = DIRECTORY_PATH / "tmp.txt";

class Input
{
public:
    static void launchVim()
    {
        if (!std::filesystem::exists(TMP_FILE_PATH))
        {
            std::ofstream newFile{TMP_FILE_PATH};
        }

        std::string command = "vim " + TMP_FILE_PATH.string();
        int status = system(command.c_str());
        
        if (status != 0)
        {
            std::cerr << "Error: Failed to launch Vim." << std::endl;
            return;
        }
    }

    static void saveDefinition(std::string& def)
    {
        std::ifstream tmp{TMP_FILE_PATH};
        if (!tmp.is_open())
        {
            std::cerr << "Error: Temp file not found." << std::endl;
            return;
        }

        std::stringstream buffer;
        buffer << tmp.rdbuf();
        def = buffer.str();
        tmp.close();

        std::ofstream o{TMP_FILE_PATH};
    }
};
