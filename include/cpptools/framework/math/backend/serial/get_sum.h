#pragma once

#include <cpptools/core/traits.hpp>

namespace cpptools::framework::math::backend::serial {




    template<typename Arr>
    auto get_sum(const Arr& arr) {
        namespace core_traits = cpptools::core::traits;

        using T = std::remove_all_extents_t<Arr>;
        T result{};

        core_traits::for_each(const_cast<Arr&>(arr), [&](auto& v) {
            result += v;
            });

        return result;
    }


}