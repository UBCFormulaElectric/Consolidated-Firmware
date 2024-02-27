#pragma once

#include "variant"

template <typename Data, typename Error>
class Result: std::variant<Data, Error>
{
    /**
     * \brief Unwraps the result, throwing an exception if it is an error
     * \return The unwrapped data
     */
    Data unwrap()
    {
        if(std::holds_alternative<Error>(*this))
            throw std::get<Error>(*this);
        return std::get<Data>(*this);
    }
};
