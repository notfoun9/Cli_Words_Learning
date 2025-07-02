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
    static void launchVim(const std::filesystem::path& path)
    {
        if (!std::filesystem::exists(path))
        {
            std::ofstream newFile{path};
        }

        std::string command = "vim " + TMP_FILE_PATH.string();
        int status = system(command.c_str());
        
        if (status != 0)
        {
            std::cerr << "Error: Failed to launch Vim." << std::endl;
            return;
        }
    }

    static void deleteFile(const std::filesystem::path& path)
    {
        std::filesystem::remove(path);
    }

    static void saveDefinition(const std::filesystem::path& source, std::string& dest)
    {
        std::ifstream tmp{source};
        if (!tmp.is_open())
        {
            std::cerr << "Error: Temp file not found." << std::endl;
            return;
        }

        std::stringstream buffer;
        buffer << tmp.rdbuf();
        dest = buffer.str();
    }
};
