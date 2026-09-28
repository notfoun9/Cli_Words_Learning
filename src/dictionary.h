#include "3rd_party/json.hpp"
#include <iostream>
#include <string>
#include <list>
#include <unordered_map>

const auto DEFAULT_GROUP_NAME = "Ungrouped";
struct Entry
{
    std::string  word;
    std::string  definition;
};

class Group
{
public:
    Group() = default;

    Group(std::string&& name, std::list<Entry>&& d)
        : name(std::move(name)), entries(std::move(d))
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

    const std::string& Name() const
    {
        return name;
    }

    bool Empty() const
    {
        return entries.empty();
    }

private:
    std::string name = DEFAULT_GROUP_NAME;
    std::list<Entry> entries;
    std::unordered_map<
        std::string, decltype(entries.begin())> map;
};

static std::ostream& operator<<(std::ostream& os, const Group& group)
{
    static std::string separator(80, '=');
    const auto& entries = group.GetAllEntries();
    os << "Group  \"" << group.Name() << "\"\n\n";
    for (const auto& entry : entries)
    {
        os << separator << '\n' << entry.word << '\n' << entry.definition;
    }
    os << separator;
    return os;
}

class Dictionary
{
public:
    Dictionary()
    {
        groups[activeGroup];
    }

    Dictionary(std::string&& activeGroup, std::vector<Group>& v)
        : activeGroup(std::move(activeGroup))
    {
        for (auto& group : v)
        {
            groups[group.Name()] = std::move(group);
        }
    }

    bool Contains(const std::string& name) const
    {
        return groups.find(name) != groups.end();
    }

    bool Size() const
    {
        return groups.size();
    }

    bool Empty() const
    {
        return groups.empty();
    }

    auto& GetGroup(const std::string& name)
    {
        return groups[name];
    }

    const auto& ActiveGroupName() const
    {
        return activeGroup;
    }

    void ChangeActiveGroup(const std::string& name)
    {
        groups[name];
        activeGroup = name;
    }

    const auto& GetActiveGroup() const
    {
        return groups.find(activeGroup)->second;
    }

    const auto& GetAllGroups() const
    {
        return groups;
    }
private:
    std::string activeGroup = DEFAULT_GROUP_NAME;
    std::unordered_map<std::string, Group> groups;
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
    struct adl_serializer<Group>
    {
        static void to_json(json& j, const Group& group)
        {
            j = json{};
            j["name"] = group.Name();
            j["words"] = std::list<Entry>();

            auto entries = group.GetAllEntries();
            for (const auto& entry : entries)
            {
                j["words"].push_back(entry);
            }
        }

        static void from_json(const json& j, Group& group)
        {
            try
            {
                auto name = j["name"].get<std::string>();
                auto entries = j["words"].get<std::list<Entry>>();
                group = Group(std::move(name), std::move(entries));
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
        static void to_json(json& j, const Dictionary& dictionary)
        {
            j = json{};
            j["activeGroup"] = dictionary.ActiveGroupName();
            j["groups"] = [&dictionary]{
                std::vector<Group> v;
                v.reserve(dictionary.Size());
                return v;
            }();
            auto groups = dictionary.GetAllGroups();
            for (const auto& [_, group] : groups)
            {
                j["groups"].push_back(group);
            }
        }

        static void from_json(const json& j, Dictionary& dictionary)
        {
            try
            {
                auto groups = j["groups"].get<std::vector<Group>>();
                dictionary = Dictionary{j["activeGroup"], groups};
            }
            catch(const std::exception& e)
            {
                std::cout << "Invalid JSON format" << e.what() << std::endl;
            }
        }
    };
};

