#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>

const auto PARENT_DIR = std::filesystem::path{getenv("HOME")}
                             / ".local" / "share" / "words";
const auto DICTIONARY_JSON_PATH      = PARENT_DIR / "dictionary.json";
const auto ACTIVE_GROUP_JSON_PATH    = PARENT_DIR / "active_group.json";
const auto TMP_FILE_PATH  = std::filesystem::path{getenv("TMPDIR")} / "tmp.txt";

class Input
{
public:
    static void writeInFile(const std::filesystem::path& path, const std::string& text)
    {
        std::ofstream newFile{path};
        newFile << text;
    }

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
