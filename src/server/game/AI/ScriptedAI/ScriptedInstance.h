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

#ifndef SC_INSTANCE_H
#define SC_INSTANCE_H

#include "InstanceData.h"
#include "World.h"
#include "ObjectMgr.h"
#include "Map.h"

#define OUT_SAVE_INST_DATA             debug_log("OSCR: 保存副本 %s 的副本数据(Map %d，副本Id %d)", instance->GetMapName(), instance->GetId(), instance->GetInstanceId())
#define OUT_SAVE_INST_DATA_COMPLETE    debug_log("OSCR: 已保存副本 %s 的副本数据(Map %d，副本Id %d)。", instance->GetMapName(), instance->GetId(), instance->GetInstanceId())
#define OUT_LOAD_INST_DATA(a)          debug_log("OSCR: 正在加载副本 %s 的副本数据(Map %d，实例Id %d)。输入“%s”", instance->GetMapName(), instance->GetId(), instance->GetInstanceId(), a)
#define OUT_LOAD_INST_DATA_COMPLETE    debug_log("OSCR: 副本 %s (Map %d，副本Id: %d)的副本数据加载完成。",instance->GetMapName(), instance->GetId(), instance->GetInstanceId())
#define OUT_LOAD_INST_DATA_FAIL        error_log("OSCR: 无法加载副本 %s 的副本数据(Map %d，副本Id: %d)。",instance->GetMapName(), instance->GetId(), instance->GetInstanceId())

class ScriptedInstance : public InstanceData
{
public:
    ScriptedInstance(Map* pMap) : InstanceData(pMap) {}
    ~ScriptedInstance() {}

    virtual void MovementInform(Creature* /*creature*/, uint32 /*uiType*/, uint32 /*uiPointId*/) { }
};

#endif
