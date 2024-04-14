// ReSharper disable CppNonExplicitConvertingConstructor
#pragma once

#include "variant"

template <typename Data, typename Error> class Result : private std::variant<Data, Error>
{
  public:
    /**
     * \brief Unwraps the result, throwing an exception if it is an error
     * \return The unwrapped data
     */
    [[nodiscard]] Data unwrap() const
    {
        if (std::holds_alternative<Error>(*this))
            throw std::get<Error>(*this);
        return std::get<Data>(*this);
    }

    [[nodiscard]] bool has_error() const { return std::holds_alternative<Error>(*this); }

    [[nodiscard]] Error get_error() const { return std::get<Error>(*this); }

    [[nodiscard]] bool has_data() const { return std::holds_alternative<Data>(*this); }

    [[nodiscard]] Data get_data() const { return std::get<Data>(*this); }

    Result(const Data &data) : std::variant<Data, Error>(data) {}    // NOLINT(*-explicit-constructor)
    Result(const Error &error) : std::variant<Data, Error>(error) {} // NOLINT(*-explicit-constructor)
};
