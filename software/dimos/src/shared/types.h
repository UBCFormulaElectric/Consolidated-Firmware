#pragma once

#include "variant"

template <typename Data, typename Error> using Result = std::variant<Data, Error>;
