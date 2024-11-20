// ReSharper disable CppNonExplicitConvertingConstructor
#pragma once

#include "variant"
#include "qlogging.h"

template <typename Data, typename Error> class Result : private std::variant<Data, Error>
{
  private:
    mutable bool type_checked = false;

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
        type_checked = true;
        return std::holds_alternative<Error>(*this);
    }

    [[nodiscard]] Error get_error() const
    {
        if (!type_checked)
        {
            qWarning("Result has not been checked for error");
        }
        return std::get<Error>(*this);
    }

    [[nodiscard]] bool has_data() const
    {
        type_checked = true;
        return std::holds_alternative<Data>(*this);
    }

    [[nodiscard]] Data get_data() const
    {
        if (!type_checked)
        {
            qWarning("Result has not been checked for error");
        }
        return std::get<Data>(*this);
    }

    Result(const Data &data) : std::variant<Data, Error>(data) {}    // NOLINT(*-explicit-constructor)
    Result(const Error &error) : std::variant<Data, Error>(error) {} // NOLINT(*-explicit-constructor)
};
