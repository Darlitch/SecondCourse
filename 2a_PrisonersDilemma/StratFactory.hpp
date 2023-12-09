#ifndef STRAT_FACTORY_HPP_
#define STRAT_FACTORY_HPP_

#include <iostream>
#include <map>


template <class TypeKey, class Base, class ProductType = Base *(*)()>
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

    Base* CreateStrat(const TypeKey& key) {
      auto it = stratFactory.find(key);
      if (it != stratFactory.end()) {
         return it->second();
      }
      return 0;+
    }

   private:
    StratFactory() {}
    using FactoryMap = std::map<TypeKey, ProductType>;
    FactoryMap stratFactory;
};

#endif