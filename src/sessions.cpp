//  Copyright (c) 2019-2020 Andrey Upadyshev <oliora@gmail.com>
//
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#include "ppconsul/sessions.h"
#include "s11n.h"

namespace ppconsul { namespace sessions {

    struct CreateResult
    {
        std::string id;
    };

    void load(const s11n::Json& src, CreateResult& dst)
    {
        using s11n::load;

        load(src, dst.id, "ID");
    }

    std::string encodeBehavior(InvalidationBehavior behavior)
    {
        switch (behavior) {
        case InvalidationBehavior::Release:
            return "release";
        case InvalidationBehavior::Delete:
            return "delete";
        }
        return {};
    }

    namespace impl {
        using s11n::to_json;

        std::string createBodyJson(const std::string &name, const std::string &node, std::chrono::seconds lockDelay,
                                   InvalidationBehavior behavior, std::chrono::seconds ttl)
        {
            s11n::Json::object obj{
                {"Behavior", encodeBehavior(behavior)},
            };

            if (lockDelay.count() >= 0)
                obj["LockDelay"] = to_json(lockDelay);

            if (!name.empty())
                obj["Name"] = name;

            if (!node.empty())
                obj["Node"] = node;

            if (ttl.count() >= 0)
                obj["TTL"] = to_json(ttl);

            return s11n::Json(std::move(obj)).dump();
        }

        std::string parseCreateResponse(const std::string &resp)
        {
            return s11n::parseJson<CreateResult>(resp).id;
        }

    }
}}
