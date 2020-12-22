/*
 * This file is part of the OregonCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Log.h"
#include "../Master.h"
#include "RegressionTest.h"

bool AddRG_Misc();

RegressionTestSuite::RegressionTestSuite()
{
}

RegressionTestSuite::~RegressionTestSuite()
{
}

bool RegressionTestSuite::RunAll()
{
    sLog.outString("运行回归测试...");

    Run(&RegressionTestSuite::TestBreathingIssues, "Breathing issues Maraudon");

    sLog.outString("回归测试完成。 总数: %u, 通过: %u, 失败: %u",
                    m_passedTestsCounter + m_failedTestsCounter,
                    m_passedTestsCounter,
                    m_failedTestsCounter);

    return m_failedTestsCounter == 0;
}

bool RegressionTestSuite::Run(bool(RegressionTestSuite::*test)(), const char* comment)
{
    if ((this->*test)())
    {
        sLog.outString("  [OK] 测试 %s 通过.", comment);
        ++m_passedTestsCounter;
        return true;
    }
    else
    {
        sLog.outString("  [ERR] 测试 %s 未通过.", comment);
        ++m_failedTestsCounter;
        return false;
    }
}
