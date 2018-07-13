/******************************************************************************
* Copyright (c) 2018 in-tech GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
******************************************************************************/

#pragma once

#include <unordered_map>

#include "boostGeometryCommon.h"
#include "OWL/LaneGeometryElement.h"

namespace World {
namespace Localization {

using LaneGeomPoint = OWL::Primitive::LaneGeometryJoint;

namespace Internal {

using boxFunction = box_t(const LaneGeomPoint*, const LaneGeomPoint*);
using polygonFunction = polygon_t(const LaneGeomPoint*, const LaneGeomPoint*);


template <typename T>
class LaneGeomCache
{
private:
    std::unordered_map<const LaneGeomPoint*, T> cache;

public:
    const T get(const LaneGeomPoint *p1,
                const LaneGeomPoint *p2,
                T(*get)(const LaneGeomPoint*, const LaneGeomPoint*))
    {
        auto search = cache.find(p1);
        if (search != cache.end()) {
          return search->second;
        }

        T item = get(p1, p2);
        cache[p1] = item;
        return item;
    }

    void clear() {
        cache.clear();
    }
};

} // namespace Internal

class Cache
{
    Internal::LaneGeomCache<box_t> boxCache;
    Internal::LaneGeomCache<polygon_t> polygonCache;

public:
    Cache() = default;

    void Clear() {
        polygonCache.clear();
        boxCache.clear();
    }

    const box_t GetBox(const LaneGeomPoint *p1,
                       const LaneGeomPoint *p2,
                       const Internal::boxFunction* get) {
        return boxCache.get(p1, p2, get);
    }

    const polygon_t GetPolygon(const LaneGeomPoint *p1,
                               const LaneGeomPoint *p2,
                               const Internal::polygonFunction* get) {
        return (const polygon_t) polygonCache.get(p1, p2, get);
    }

private:
    Cache(const Cache&) = delete;
    Cache(Cache&&) = delete;
    Cache& operator=(const Cache&) = delete;
    Cache& operator=(Cache&&) = delete;
};


} // namespace Localization
} // namespace World
