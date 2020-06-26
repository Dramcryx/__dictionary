#ifndef MAP_DICTIONARY_H
#define MAP_DICTIONARY_H

#include "dictionary.h"

#include <map>

template<class TKey>
class MapNotFoundException : public NotFoundException<TKey>
{
public:
    MapNotFoundException(const TKey & k):
        m_key(k)
    {}
    virtual const char* what() const noexcept
    {
        return "Missing key in dictionary";
    }
    const TKey& GetKey() const noexcept override
    {
        return m_key;
    }

private:
    TKey m_key;
};


template <typename TValue, typename VALUE_TYPE>
std::enable_if_t<std::is_lvalue_reference<TValue>::value, VALUE_TYPE> cast_in(const TValue & v)
{
    return (VALUE_TYPE)&v;
}

template <typename TValue, typename VALUE_TYPE>
std::enable_if_t<!std::is_lvalue_reference<TValue>::value, VALUE_TYPE> cast_in(const TValue & v)
{
    return (VALUE_TYPE)v;
}

template <typename TValue, typename VALUE_TYPE>
std::enable_if_t<std::is_lvalue_reference<TValue>::value, const TValue &> cast_out(const VALUE_TYPE & v)
{
    return *v;
}

template <typename TValue, typename VALUE_TYPE>
std::enable_if_t<!std::is_lvalue_reference<TValue>::value, const TValue &> cast_out(const VALUE_TYPE & v)
{
    return v;
}

template<class TKey, class TValue>
class MapDictionary: public Dictionary<TKey, TValue>
{
    // раз реализация "не ограничена", то от внешнего мира спрячем тот факт,
    // что ссылки будут храниться как указатели
    using KEY_TYPE = std::conditional_t<std::is_reference<TKey>::value,
                                      std::add_pointer_t<std::remove_reference_t<TKey>>,
                                      TKey>;
    using VALUE_TYPE = std::conditional_t<std::is_reference<TValue>::value,
                                      std::add_pointer_t<std::remove_reference_t<TValue>>,
                                      TValue>;
public:
    MapDictionary() = default;
    const TValue& Get(const TKey& key) const override
    {
        auto it = m_dict.find(cast_in<TKey, KEY_TYPE>(key));
        if (it != m_dict.end())
        {
            return cast_out<TValue, VALUE_TYPE>(it->second);
        }
        else
        {
            throw MapNotFoundException<TKey>{key};
        }
    }
    void Set(const TKey& key, const TValue& value) override
    {
        m_dict[cast_in<TKey, KEY_TYPE>(key)] = cast_in<TValue, VALUE_TYPE>(value);
    }
    bool IsSet(const TKey& key) const override
    {
        return m_dict.find(cast_in<TKey, KEY_TYPE>(key)) != m_dict.end();
    }

private:
    std::map<KEY_TYPE, VALUE_TYPE> m_dict;

};

#endif
