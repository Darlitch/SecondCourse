#ifndef STRAT_FACTORY_HPP_
#define STRAT_FACTORY_HPP_

#include <functional>
#include <iostream>
#include <map>

template <class TypeKey, class Base, class ProductType = std::function<Base*()>>  // std::invokable
class StratFactory {
   public:
    ~StratFactory() {}
    StratFactory(StratFactory&) = delete;
    StratFactory& operator=(StratFactory&) = delete;
    // Добавление стратегии в Map
    bool RegisterStrat(const TypeKey& key, ProductType Func) {
        auto it = stratFactory.find(key);
        if (it == stratFactory.end()) {
            stratFactory[key] = Func;
            return true;
        }
        return false;
    }
    // Обращение в map для создания стратегии
    Base* CreateStrat(const TypeKey& key) {
        auto it = stratFactory.find(key);
        if (it != stratFactory.end()) {
            return it->second();
        }
        return 0;
    }
    // обращение к фабрике(паттерн Синглтон)
    static StratFactory* GetInstance() {
        if (factory == nullptr) {
            factory = new StratFactory<TypeKey, Base, ProductType>;
        }
        return factory;
    }

   private:
    StratFactory() {}
    static StratFactory<TypeKey, Base, ProductType>* factory;
    using FactoryMap = std::map<TypeKey, ProductType>;
    FactoryMap stratFactory;
};

template <class TypeKey, class Base, class ProductType>
StratFactory<TypeKey, Base, ProductType>* StratFactory<TypeKey, Base, ProductType>::factory = nullptr;

#endif