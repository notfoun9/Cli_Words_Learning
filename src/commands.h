#include "3rd_party/json.hpp"
#include "dictionary.h"
#include "input.h"
#include "tools.h"
#include <cstdio>
#include <fstream>
#include <optional>

using nlohmann::json;

class Commands
{
public:
    static void GuessTheWord(const json& j)
    {
        if (!j.is_array())
        {
            std::cout << "Dictionary is empty. Add some entries first" << std::endl;
            return;
        }

        auto dict = j.get<Dictionary>();
        if (dict.Empty())
        {
            std::cout << "Dictionary is empty. Add some entries first" << std::endl;
            return;
        }

        auto& e = PickRandomEntry(dict);
        std::cout << "Give the definition of:\n" << e.word << '\n';
        std::getchar();
        std::cout << "The answer is:\n" << e.definition << std::endl;
    }

    static void AddWord(json& j, int argc, char** argv)
    {
        if (argc == 2)
        {
            std::cout << "Error: No word provided" << std::endl;
            std::cout << "The usage is \"words add <word>\"" << std::endl;
            return;
        }

        Entry newWord;
        for (int i = 2; i < argc; ++i)
        {
            newWord.word += argv[i];
        }

        Input::launchVim(TMP_FILE_PATH);
        Input::saveDefinition(TMP_FILE_PATH, newWord.definition);
        Input::deleteFile(TMP_FILE_PATH);

        std::cout << "\n\nYou've added a new word:\n"
                  << newWord.word << " - "
                  << newWord.definition << std::endl;

        j.push_back(newWord);
    }

    static void Show(const json& j)
    {
        static std::string separator(80, '=');

        auto dict = j.get<Dictionary>();
        std::ofstream o{TMP_FILE_PATH};
        for (size_t i = 0, size = dict.Size(); i < size; ++i)
        {
            o << separator << '\n' << dict[i].word << '\n' << dict[i].definition;
        }
        o << separator;
        o.close();

        Input::launchVim(TMP_FILE_PATH);
        Input::deleteFile(TMP_FILE_PATH);
    }

    static void Delete(json& j, int argc, char** argv)
    {
        auto dict = j.get<Dictionary>();
        if (argc < 3)
        {
            std::cout << "Error: No word provided\n"
                      << "The usage is \"words delete <word>\"" << std::endl;
            return;
        }
        dict.Remove(argv[2]);

        j = json(dict);
    }

    static void Edit(json& j, int argc, char** argv)
    {
        auto dict = j.get<Dictionary>();
        if (argc < 3)
        {
            std::cout << "Error: No word provided\n"
                      << "The usage is \"words edit <word>\"" << std::endl;
            return;
        }

        auto idx = dict.GetIdx(argv[2]);
        if (idx == std::nullopt)
        {
            std::cout << "Error: This is no such word in the dictionary" << std::endl;
            return;
        }

        auto& entry = dict[idx.value()];

        Input::writeInFile(TMP_FILE_PATH, entry.definition);
        Input::launchVim(TMP_FILE_PATH);

        Input::saveDefinition(TMP_FILE_PATH, entry.definition);
        Input::deleteFile(TMP_FILE_PATH);

        j = json(dict);
    }

private:
    static const Entry& PickRandomEntry(const Dictionary& dict)
    {
        size_t size = dict.Size();
        size_t idx = Random::Generate(0, size - 1);
        assert(idx <= size);

        return dict[idx];
    }
};
