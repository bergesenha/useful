#pragma once

#include "handle_map.hpp"


namespace useful
{
template <class T>
class dag
{
public:
    dag() = default;

private:
    handle_map<T> nodes_;
};
} // namespace useful
