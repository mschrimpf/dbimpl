#ifndef H_Schema_hpp
#define H_Schema_hpp

#include <vector>
#include <string>
#include "Types.hpp"

struct Schema {
  struct Relation {
    struct Attribute {
      std::string name;
      Types::Tag type;
      unsigned len;
      bool notNull;

      Attribute(std::string name, Types::Tag type, unsigned len, bool notNull)
          : name(name), type(type), len(len), notNull(notNull) { }

      Attribute(std::string name, Types::Tag type, unsigned len)
          : Attribute(name, type, len, true) { }

      Attribute() : len(~0 /* max value */), notNull(true) { }
    };

    std::string name;
    std::vector<Schema::Relation::Attribute> attributes;
    std::vector<unsigned> primaryKey;

    Relation(const std::string &name) : name(name) { }

    void addAttribute(Attribute attribute);
  };

  std::vector<Schema::Relation> relations;

  std::string toString() const;
};

#endif
