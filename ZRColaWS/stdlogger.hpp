/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2022 Amebis
*/

#pragma once

#include <oatpp/core/base/Environment.hpp>
#include <iostream>
#include <mutex>

class StdLogger : public oatpp::base::Logger
{
private:
    std::mutex m_lock;

public:
    StdLogger() {}

    void log(v_uint32 priority, const std::string& tag, const std::string& message) override
    {
        if (!isLogPriorityEnabled(priority))
            return;
        std::lock_guard<std::mutex> guard(m_lock);
        (priority < oatpp::base::Logger::PRIORITY_W ? std::cout : std::cerr) << tag << ": " << message << std::endl;
    }
};