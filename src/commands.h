#include "3rd_party/json.hpp"
#include "dictionary.h"
#include "input.h"
#include "tools.h"
#include <cstdio>
#include <fstream>

using nlohmann::json;

class Commands
{
public:
    static void GuessTheWord(const json& j)
    {
        auto group = j.get<Group>();
        if (group.Empty())
        {
            std::cout << "Group \"" << group.Name()
                      << "\" is empty. Add some entries first" << std::endl;
            return;
        }

        auto& e = PickRandomEntry(group);
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
        if (newWord.definition.back() != '\n')
        {
            newWord.definition.push_back('\n');
        }
        Input::deleteFile(TMP_FILE_PATH);

        std::cout << "\n\nYou've added a new word:\n"
                  << newWord.word << " - "
                  << newWord.definition << std::endl;

        j["words"].push_back(newWord);
    }

    static void Show(const json& j)
    {
        auto group = j.get<Group>();
        std::ofstream os{TMP_FILE_PATH};
        os << group;
        os.close();
        Input::launchVim(TMP_FILE_PATH);
        Input::deleteFile(TMP_FILE_PATH);
    }

    static void Delete(json& j, int argc, char** argv)
    {
        auto group = j.get<Group>();
        if (argc < 3)
        {
            std::cout << "Error: No word provided\n"
                      << "The usage is \"words delete <word>\"" << std::endl;
            return;
        }
        group.RemoveEntry(argv[2]);

        j = json(group);
    }

    static void Edit(json& j, int argc, char** argv)
    {
        auto group = j.get<Group>();
        if (argc < 3)
        {
            std::cout << "Error: No word provided\n"
                      << "The usage is \"words edit <word>\"" << std::endl;
            return;
        }
        auto word = argv[2];
        if (!group.Contains(word))
        {
            std::cout << "Error: This is no such word in the groupionary" << std::endl;
            return;
        }
        auto& entry = group.GetEntry(word);

        Input::writeInFile(TMP_FILE_PATH, entry.definition);
        Input::launchVim(TMP_FILE_PATH);

        Input::saveDefinition(TMP_FILE_PATH, entry.definition);
        Input::deleteFile(TMP_FILE_PATH);

        j = json(group);
    }

private:
    static const Entry& PickRandomEntry(const Group& group)
    {
        size_t size = group.Size();
        size_t idx = Random::Generate(0, size - 1);
        assert(idx <= size);

        auto iter = group.GetAllEntries().begin();
        std::advance(iter, idx);
        return *iter;
    }
};
