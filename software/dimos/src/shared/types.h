// ReSharper disable CppNonExplicitConvertingConstructor
#pragma once

#include "variant"

template <typename Data, typename Error> class Result : public std::variant<Data, Error>
{
    /**
     * \brief Unwraps the result, throwing an exception if it is an error
     * \return The unwrapped data
     */
    Data unwrap()
    {
        if (std::holds_alternative<Error>(*this))
            throw std::get<Error>(*this);
        return std::get<Data>(*this);
    }

  public:
    Result(const Data &data) : std::variant<Data, Error>(data) {}    // NOLINT(*-explicit-constructor)
    Result(const Error &error) : std::variant<Data, Error>(error) {} // NOLINT(*-explicit-constructor)
};
