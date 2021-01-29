/**
 * @brief Invalid format exception.
 *
 * Copyright (c) 2019 YADRO
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <cstring>
#include <exception>
#include <string>

namespace eSEL
{

/**
 * @class InvalidFormat
 * @brief Invalid format exception.
 */
class InvalidFormat : public std::exception
{
    static constexpr auto defaultMessage = "Invalid format";

  public:
    /**
     * @brief Constructor.
     *
     * @param[in] fmt - error message, printf-like format
     */
    template <typename... A>
    InvalidFormat(const char* fmt, A&&... args)
    {
        if (!fmt)
            msg_ = defaultMessage;
        else
        {
            const int len = snprintf(nullptr, 0, fmt, std::forward<A>(args)...);
            if (len <= 0)
                msg_ = defaultMessage;
            else
            {
                msg_.resize(static_cast<size_t>(len) + 1 /* last null */);
                snprintf(msg_.data(), msg_.size(), fmt,
                         std::forward<A>(args)...);
            }
        }
    }
    InvalidFormat(const char* message)
    {
        msg_ = (!message || strlen(message) == 0) ? defaultMessage : message;
    }

    const char* what() const noexcept override
    {
        return msg_.c_str();
    }

  private:
    /** @brief Error description. */
    std::string msg_;
};

} // namespace eSEL
