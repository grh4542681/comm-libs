#ifndef __TEMP_MULTITON_H__
#define __TEMP_MULTITON_H__

#include <map>
#include <string>
#include <memory>
#include <utility>
#include "thread_mutex_lock.h"

namespace odp {

#if 0
template < typename T, typename K = string>
class Multiton
{
public:
    template<typename... Args>
    static std::shared_ptr<T> getInstance(const K& key, Args&&... args)
    {
        return _getInstance(key, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static std::shared_ptr<T> getInstance(K&& key, Args&&... args)
    {
        return _getInstance(key, std::forward<Args>(args)...);
    }
private:
    template<typename Key, typename... Args>
    static std::shared_ptr<T> _getInstance(Key&& key, Args&&...args)
    {
        std::shared_ptr<T> instance = NULL;
        auto it = instance_map_.find(key);
        if (it == instance_map_.end()) {
            instance = std::make_shared<T>(std::forward<Args>(args)...);
            instance_map_.emplace(key, instance);
        } else {
            instance = it->second;
        }

        return instance;
    }

    Multiton(void);
    virtual ~Multiton(void);
    Multiton(const Multiton&);
    Multiton& operator = (const Multiton&);

    static map<K, std::shared_ptr<T>> instance_map_;
};
template <typename T, typename K> map<K, std::shared_ptr<T>> Multiton<T, K>::instance_map_;
#endif

template < typename T, typename K = std::string>
class Multiton
{
public:
    template<typename ... Args>
    static T* getInstance(const K& key, Args&& ... args)
    {
        return _getInstance(key, std::forward<Args>(args)...);
    }

    template<typename ... Args>
    static T* getInstance(K&& key, Args&& ... args)
    {
        return _getInstance(key, std::forward<Args>(args)...);
    }

    static void delInstance(const K& key)
    {
        _delInstance(key);
    }

    static void delInstance(K&& key)
    {
        _delInstance(key);
    }

    template<typename ... Args>
    static T* resetInstance(const K& key, Args&& ... args)
    {
        return _resetInstance(key, std::forward<Args>(args)...);
    }

    template<typename ... Args>
    static T* resetInstance(K&& key, Args&& ... args)
    {
        return _resetInstance(key, std::forward<Args>(args)...);
    }

    static int getSize()
    {
        return instance_map_.size();
    }

private:

    /**
    * @brief _getInstance - Get an instance of the specified key value.
    *
    * @tparam [Key] - Type of key.
    * @tparam [Args] - Indefinite length parameter template.
    * @param [key] - Key of instance.
    * @param [args] - Parameters for create instance.
    *
    * @returns  Instance pointer.
    */
    template<typename Key, typename ... Args>
    static T* _getInstance(Key&& key, Args&& ... args)
    {
        T* instance = NULL;
        auto it = instance_map_.find(key);
        if (it == instance_map_.end()) {
            mutex_.lock();
            it = instance_map_.find(key);
            if (it == instance_map_.end()) {
                instance = new T(std::forward<Args>(args)...);
                instance_map_.emplace(key, instance);
            } else {
                instance = it->second;
            }
            mutex_.unlock();
        } else {
            instance = it->second;
        }

        return instance;
    }

    /**
    * @brief _delInstance - Delete an instance of the specified key value.
    *
    * @tparam [Key] - Type of the key.
    * @param [key] - Instance key value.
    */
    template<typename Key>
    static void _delInstance(Key&& key)
    {
        auto it = instance_map_.find(key);
        if (it != instance_map_.end()) {
            mutex_.lock();
            it = instance_map_.find(key);
            if (it != instance_map_.end()) {
                T* instance = it->second;
                instance_map_.erase(it);
                delete instance;
                instance = NULL;
            }
            mutex_.unlock();
        }
    }

    /**
    * @brief _resetInstance - Reset an instance of the specified key value.
    *
    * @tparam [Key] - Type of key.
    * @tparam [Args] - Indefinite length parameter template.
    * @param [key] - Key of instance.
    * @param [args] - Parameters for create instance.
    *
    * @returns  Instance pointer.
    */
    template<typename Key, typename ... Args>
    static T* _resetInstance(Key&& key, Args&& ... args)
    {
        T* instance = new T(std::forward<Args>(args)...);
        auto it = instance_map_.find(key);
        if (it != instance_map_.end()) {
            mutex_.lock();
            it = instance_map_.find(key);
            if (it != instance_map_.end()) {
                T* old = it->second;
                delete old;
                it->second = instance;
            } else {
                instance_map_.emplace(key, instance);
            }
            mutex_.unlock();
        } else {
            instance_map_.emplace(key, instance);
        }
        return instance;
    }

    Multiton(void);
    virtual ~Multiton(void);
    Multiton(const Multiton&);
    Multiton& operator = (const Multiton&);

    static thread::mutex::ThreadMutexLock mutex_;
    static std::map<K, T*> instance_map_;
};
template <typename T, typename K> thread::mutex::ThreadMutexLock Multiton<T, K>::mutex_;
template <typename T, typename K> std::map<K, T*> Multiton<T, K>::instance_map_;

} //namespace odp end

#endif
