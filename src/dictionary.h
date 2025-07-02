#include "3rd_party/json.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

struct Entry
{
    std::string  word;
    std::string  definition;
};

class Dictionary
{
public:
    Dictionary() = default;

    Dictionary(std::vector<Entry>&& d)
        : entries(std::move(d))
    {
        for (size_t i = 0, size = entries.size(); i < size; ++i)
        {
            map[entries[i].word] = i;
        }
    }

    bool Contains(const std::string& word) const
    {
        return map.find(word) != map.end();
    }

    void AddEntry(Entry&& entry)
    {
        entries.emplace_back(std::move(entry));
        map[entries.back().word] = Size() - 1;
    }

    size_t Size() const
    {
        return entries.size();
    }

    bool Empty() const
    {
        return entries.empty();
    }

    const Entry& operator[](size_t idx) const
    {
        return entries[idx];
    }

    Entry& operator[](size_t idx)
    {
        return entries[idx];
    }

    friend void swap(Dictionary& l, Dictionary& r)
    {
        std::swap(l.map, r.map);
        std::swap(l.entries, r.entries);
    }

private:
    std::vector<Entry>                       entries;
    std::unordered_map<std::string, size_t>  map;
};

namespace nlohmann
{
    template<>
    struct adl_serializer<Entry>
    {
        static void to_json(json& j, const Entry& entry)
        {
            j = json{
                {"word", entry.word},
                {"definition", entry.definition}
            };
        }

        static void from_json(const json& j, Entry& entry)
        {
            try
            {
                entry.word = j["word"].get<std::string>();
                entry.definition = j["definition"].get<std::string>();
            }
            catch(const std::exception& e)
            {
                std::cout << "Invalid JSON format" << e.what() << std::endl;
            }
        }
    };

    template<>
    struct adl_serializer<Dictionary>
    {
        static void to_json(json& j, const Dictionary& dict)
        {
            j = json{};
            for (size_t i = 0, size = dict.Size(); i < size; ++i)
            {
                j.push_back(dict[i]);
            }
        }

        static void from_json(const json& j, Dictionary& dict)
        {

            try
            {
                auto v = j.get<std::vector<Entry>>();
                Dictionary tmp{std::move(v)};

                swap(tmp, dict);
            }
            catch(const std::exception& e)
            {
                std::cout << "Invalid JSON format" << e.what() << std::endl;
            }
        }
    };
};

