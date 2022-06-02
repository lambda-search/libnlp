
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#pragma once

#include "libnlp/base/common.h"
#include "libnlp/base/utf8_util.h"

namespace libnlp::dict {

    class LIBNLP_EXPORT dict_entity {
    public:
        explicit dict_entity(const std::string &key) : _key(key) {}

        dict_entity(const std::string &key, const std::string &value) : _key(key), _values() {
            _values.push_back(value);
        }

        dict_entity(const std::string &key, const std::vector<std::string> &values) : _key(key), _values(values) {
        }

        explicit dict_entity(std::string &&key) : _key(std::move(key)) {}

        dict_entity(std::string &&key, std::string &&value)
                : _key(std::move(key)),
                  _values(std::vector<std::string>{std::move(value)}) {
        }

        dict_entity(std::string &&key, std::vector<std::string> &&values)
                : _key(std::move(key)), _values(std::move(values)) {
        }

        ~dict_entity() = default;

        const std::string &key() const {
            return _key;
        }

        const std::vector<std::string> &values() const {
            return _values;
        }

        const std::string &get_default() const {
            if (num_values() > 0) {
                return _values.at(0);
            } else {
                return _key;
            }
        }

        size_t num_values() const {
            return _values.size();
        }

        std::string to_string() const;

        size_t key_length() const { return _key.length(); }

        bool operator<(const dict_entity &that) const { return _key < that._key; }

        bool operator==(const dict_entity &that) const { return _key == that._key; }

        static bool smart_ptr_less_than(const std::unique_ptr<dict_entity> &a,
                                        const std::unique_ptr<dict_entity> &b) {
            return *a < *b;
        }

        static dict_entity *create(const std::string &key) {
            return new dict_entity(key);
        }

        static dict_entity *create(const dict_entity *entity) {
            return new dict_entity(entity->key(), entity->values());
        }

        static dict_entity *create(const std::string &key, const std::string &value) {
            return new dict_entity(key, value);
        }

        static dict_entity *create(const std::string &key, const std::vector<std::string> &values) {
            return new dict_entity(key, values);
        }

        static dict_entity *create(std::string &&key) {
            return new dict_entity(std::move(key));
        }

        static dict_entity *create(std::string &&key, std::string &&value) {
            return new dict_entity(std::move(key), std::move(value));
        }

        static dict_entity *create(std::string &&key, std::vector<std::string> &&values) {
            return new dict_entity(std::move(key), std::move(values));
        }

    private:
        std::string _key;
        std::vector<std::string> _values;

    };

}  // namespace libnlp::dict

