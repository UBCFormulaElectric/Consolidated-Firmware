// ReSharper disable CppNonExplicitConvertingConstructor
#pragma once

#include "variant"
#include "qlogging.h"

template <typename Data, typename Error> class Result : private std::variant<Data, Error>
{
  private:
    mutable bool checked_error = false;

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

    [[nodiscard]] bool has_error() const
    {
        checked_error = true;
        return std::holds_alternative<Error>(*this);
    }

    [[nodiscard]] Error get_error() const { return std::get<Error>(*this); }

    [[nodiscard]] bool has_data() const
    {
        if (!checked_error)
        {
            qWarning("Result has not been checked for error");
        }
        return std::holds_alternative<Data>(*this);
    }

    [[nodiscard]] Data get_data() const { return std::get<Data>(*this); }

    Result(const Data &data) : std::variant<Data, Error>(data) {}    // NOLINT(*-explicit-constructor)
    Result(const Error &error) : std::variant<Data, Error>(error) {} // NOLINT(*-explicit-constructor)
};
