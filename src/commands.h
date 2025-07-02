#include "dictionary.h"
#include "input.h"
#include "tools.h"
#include <fstream>

using nlohmann::json;

class Commands
{
public:
    static void GuessTheWord(const Dictionary& dict)
    {
        if (dict.Empty())
        {
            std::cout << "Dictionary is empty. Add some entries first" << std::endl;
            return;
        }

        auto& e = PickRandomEntry(dict);
        std::cout << "Give the definition of:\n" << e.word << '\n';
        std::string tmp;
        std::cin >> tmp;
        std::cout << "The answer is:\n" << e.definition << std::endl;
    }

    static void AddWord(json& j, int argc, char** argv)
    {
        if (argc == 2)
        {
            std::cout << "Error: No word provided" << std::endl;
            std::cout << "The usage is \"words --add <word>\"" << std::endl;
        }

        Entry newWord;
        for (int i = 2; i < argc; ++i)
        {
            newWord.word += argv[i];
        }

        Input::launchVim();
        Input::saveDefinition(newWord.definition);

        std::cout << "\n\nYou've added a new word: \""
                  << newWord.word << "\"\nWith definition: \""
                  << newWord.definition << "\"" << std::endl;

        j.push_back(newWord);

        std::ofstream ofile{ JSON_PATH };
        ofile << j.dump(4);
    }

    static void Show(const Dictionary& dict)
    {
        std::ofstream o{TMP_FILE_PATH};
        for (size_t i = 0, size = dict.Size(); i < size; ++i)
        {
            o << dict[i].word << '\n' << dict[i].definition << '\n';
        }
        o.close();

        Input::launchVim();
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
