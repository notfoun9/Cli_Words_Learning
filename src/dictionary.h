#include "3rd_party/json.hpp"
#include <iostream>
#include <string>
#include <list>
#include <unordered_map>

struct Entry
{
    std::string  word;
    std::string  definition;
};

class Group
{
public:
    Group() = default;

    Group(std::list<Entry>&& d)
        : entries(std::move(d))
    {
        for (auto iter = entries.begin(), end = entries.end(); iter != end; ++iter)
        {
            map[iter->word] = iter;
        }
    }

    bool Contains(const std::string& word) const
    {
        return map.find(word) != map.end();
    }

    void AddEntry(Entry&& entry)
    {
        entries.emplace_back(std::move(entry));
        map[entries.back().word] = std::prev(entries.end());
    }

    void RemoveEntry(const std::string& word)
    {
        if (map.find(word) == map.end())
        {
            return;
        }
        entries.erase(map[word]);
        map.erase(word);
    }

    Entry& GetEntry(const std::string& word)
    {
        auto iter = map.find(word);
        return *(iter->second);
    }

    const Entry& GetEntry(const std::string& word) const
    {
        auto iter = map.find(word);
        return *(iter->second);
    }

    const auto& GetAllEntries() const
    {
        return entries;
    }

    size_t Size() const
    {
        return entries.size();
    }

    bool Empty() const
    {
        return entries.empty();
    }

    friend void swap(Group& l, Group& r)
    {
        std::swap(l.map, r.map);
        std::swap(l.entries, r.entries);
    }

private:
    std::list<Entry> entries;
    std::unordered_map<
        std::string, decltype(entries.begin())> map;
};

static std::ostream& operator<<(std::ostream& os, const Group& group)
{
    static std::string separator(80, '=');
    const auto& entries = group.GetAllEntries();
    for (const auto& entry : entries)
    {
        os << separator << '\n' << entry.word << '\n' << entry.definition;
    }
    os << separator;
    return os;
}

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
    struct adl_serializer<Group>
    {
        static void to_json(json& j, const Group& group)
        {
            j = json{};
            auto entries = group.GetAllEntries();
            for (const auto& entry : entries)
            {
                j.push_back(entry);
            }
        }

        static void from_json(const json& j, Group& group)
        {

            try
            {
                auto v = j.get<std::list<Entry>>();
                Group tmp{std::move(v)};

                swap(tmp, group);
            }
            catch(const std::exception& e)
            {
                std::cout << "Invalid JSON format" << e.what() << std::endl;
            }
        }
    };
};

