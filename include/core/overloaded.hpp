#ifndef CBB_CORE_OVERLOADED_HPP
#define CBB_CORE_OVERLOADED_HPP

namespace cbb {

namespace core {

// seems to be a component of implementing the vistor pattern for std::variant
// no clue how this works tho 
// js know that it does
template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;
    
} // namespace core

    
} // namespace cbb


#endif /// CBB_CORE_OVERLOADED_HPP