
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
    /**
     * Key-values pair entry
     * @ingroup libnlp_dict_api
     */
    class LIBNLP_EXPORT dict_entry {
    public:
        virtual ~dict_entry() {}

        virtual std::string key() const = 0;

        virtual std::vector<std::string> values() const = 0;

        virtual std::string get_default() const = 0;

        virtual size_t num_values() const = 0;

        virtual std::string to_string() const = 0;

        size_t key_length() const { return key().length(); }

        bool operator<(const dict_entry &that) const { return key() < that.key(); }

        bool operator==(const dict_entry &that) const { return key() == that.key(); }

        static bool UPtrLessThan(const std::unique_ptr<dict_entry> &a,
                                 const std::unique_ptr<dict_entry> &b) {
            return *a < *b;
        }
    };

    class LIBNLP_EXPORT non_value_dict_entry : public dict_entry {
    public:
        explicit non_value_dict_entry(const std::string &k) : _key(k) {}

        ~non_value_dict_entry() override = default;

        std::string key() const override { return _key; }

        std::vector<std::string> values() const override {
            return std::vector<std::string>();
        }

        std::string get_default() const override { return _key; }

        size_t num_values() const override { return 0; }

        std::string to_string() const override { return _key; }

    private:
        std::string _key;
    };

    class LIBNLP_EXPORT single_value_dict_entry : public dict_entry {
    public:
        virtual std::string value() const = 0;

        std::vector<std::string> values() const override {
            return std::vector<std::string>{value()};
        }

        std::string get_default() const override { return value(); }

        size_t num_values() const override { return 1; }

        std::string to_string() const override {
            return std::string(key()) + "\t" + value();
        }
    };

    class LIBNLP_EXPORT str_single_value_dict_entry : public single_value_dict_entry {
    public:
        str_single_value_dict_entry(const std::string &k, const std::string &v)
                : _key(k), _value(v) {}

        ~str_single_value_dict_entry() override = default;

        std::string key() const override { return _key; }

        std::string value() const override { return _value; }

    private:
        std::string _key;
        std::string _value;
    };

    class LIBNLP_EXPORT multi_value_dict_entry : public dict_entry {
    public:
        virtual std::string get_default() const {
            if (num_values() > 0) {
                return values().at(0);
            } else {
                return key();
            }
        }

        virtual std::string to_string() const;
    };

    class LIBNLP_EXPORT str_multi_value_dict_entry : public multi_value_dict_entry {
    public:
        str_multi_value_dict_entry(const std::string &k,
                                   const std::vector<std::string> &vs)
                : _key(k), _values(vs) {}

        ~str_multi_value_dict_entry() override = default;

        std::string key() const override { return _key; }

        size_t num_values() const override { return _values.size(); }

        std::vector<std::string> values() const override { return _values; }

    private:
        std::string _key;
        std::vector<std::string> _values;
    };

    class LIBNLP_EXPORT dict_entry_factory {
    public:
        static dict_entry *create(const std::string &key) {
            return new non_value_dict_entry(key);
        }

        static dict_entry *create(const std::string &key, const std::string &value) {
            return new str_single_value_dict_entry(key, value);
        }

        static dict_entry *create(const std::string &key,
                                  const std::vector<std::string> &values) {
            if (values.size() == 0) {
                return create(key);
            } else if (values.size() == 1) {
                return create(key, values.front());
            }
            return new str_multi_value_dict_entry(key, values);
        }

        static dict_entry *create(const dict_entry *entry) {
            if (entry->num_values() == 0) {
                return new non_value_dict_entry(entry->key());
            } else if (entry->num_values() == 1) {
                return new str_single_value_dict_entry(entry->key(), entry->values().front());
            } else {
                return new str_multi_value_dict_entry(entry->key(), entry->values());
            }
        }
    };
} // namespace libnlp::cc
