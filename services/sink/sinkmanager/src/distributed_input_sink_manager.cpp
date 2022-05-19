/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#include "distributed_input_sink_manager.h"

#include <if_system_ability_manager.h>
#include <iservice_registry.h>
#include <system_ability_definition.h>
#include "nlohmann/json.hpp"
#include "distributed_input_sink_transport.h"
#include "distributed_input_collector.h"
#include "distributed_input_sink_switch.h"
#include "distributed_hardware_log.h"
#include "white_list_util.h"

namespace OHOS {
namespace DistributedHardware {
namespace DistributedInput {
REGISTER_SYSTEM_ABILITY_BY_ID(DistributedInputSinkManager, DISTRIBUTED_HARDWARE_INPUT_SINK_SA_ID, true);

DistributedInputSinkManager::DistributedInputSinkManager(int32_t saId, bool runOnCreate)
    : SystemAbility(saId, runOnCreate)
{
    input_types_ = 0;
}

DistributedInputSinkManager::DInputSinkListener::DInputSinkListener(DistributedInputSinkManager *manager)
{
    sinkManagerObj_ = manager;
    sinkManagerObj_->SetInputTypes(INPUT_TYPE_NULL);
    DHLOGI("DInputSinkListener init.");
}

DistributedInputSinkManager::DInputSinkListener::~DInputSinkListener()
{
    sinkManagerObj_->SetInputTypes(INPUT_TYPE_NULL);
    sinkManagerObj_ = nullptr;
    DHLOGI("DInputSinkListener destory.");
}

void DistributedInputSinkManager::DInputSinkListener::onPrepareRemoteInput(
    const int32_t& sessionId, const std::string &deviceId)
{
    DHLOGI("onPrepareRemoteInput called, sessionId: %d", sessionId);

    nlohmann::json jsonStr;
    jsonStr[DINPUT_SOFTBUS_KEY_CMD_TYPE] = TRANS_SINK_MSG_ONPREPARE;
    std::string smsg = "";
    int ret = DistributedInputCollector::GetInstance().Init(
        DistributedInputSinkTransport::GetInstance().GetEventHandler());
    if (ret != SUCCESS) {
        DHLOGE("DInputSinkListener init InputCollector error.");
        jsonStr[DINPUT_SOFTBUS_KEY_RESP_VALUE] = false;
        jsonStr[DINPUT_SOFTBUS_KEY_WHITE_LIST] = "";
        smsg = jsonStr.dump();
        DistributedInputSinkTransport::GetInstance().RespPrepareRemoteInput(sessionId, smsg);
        return;
    }

    DistributedInputSinkSwitch::GetInstance().AddSession(sessionId);

    // send prepare result and if result ok, send white list
    TYPE_WHITE_LIST_VEC vecFilter;
    if (sinkManagerObj_->GetInitWhiteListFlag() == false) {
        if (SUCCESS != WhiteListUtil::GetInstance().Init(deviceId)) {
            DHLOGE("%s called, init white list fail!", __func__);
            jsonStr[DINPUT_SOFTBUS_KEY_RESP_VALUE] = false;
            jsonStr[DINPUT_SOFTBUS_KEY_WHITE_LIST] = "";
            smsg = jsonStr.dump();
            DistributedInputSinkTransport::GetInstance().RespPrepareRemoteInput(sessionId, smsg);
            return;
        }
        sinkManagerObj_->SetInitWhiteListFlag(true);
    }

    WhiteListUtil::GetInstance().GetWhiteList(deviceId, vecFilter);
    if (vecFilter.empty() || vecFilter[0].empty() || vecFilter[0][0].empty()) {
        DHLOGE("onPrepareRemoteInput called, white list is null.");
        jsonStr[DINPUT_SOFTBUS_KEY_RESP_VALUE] = false;
        jsonStr[DINPUT_SOFTBUS_KEY_WHITE_LIST] = "";
        smsg = jsonStr.dump();
        DistributedInputSinkTransport::GetInstance().RespPrepareRemoteInput(sessionId, smsg);
        return;
    }
    nlohmann::json filterMsg(vecFilter);
    std::string object = filterMsg.dump();
    jsonStr[DINPUT_SOFTBUS_KEY_RESP_VALUE] = true;
    jsonStr[DINPUT_SOFTBUS_KEY_WHITE_LIST] = object;
    smsg = jsonStr.dump();
    DistributedInputSinkTransport::GetInstance().RespPrepareRemoteInput(sessionId, smsg);
}

void DistributedInputSinkManager::DInputSinkListener::onUnprepareRemoteInput(const int32_t& sessionId)
{
    DHLOGI("onUnprepareRemoteInput called, sessionId: %d", sessionId);
    onStopRemoteInput(sessionId, INPUT_TYPE_ALL);
    DistributedInputSinkSwitch::GetInstance().RemoveSession(sessionId);

    nlohmann::json jsonStr;
    jsonStr[DINPUT_SOFTBUS_KEY_CMD_TYPE] = TRANS_SINK_MSG_ONUNPREPARE;
    jsonStr[DINPUT_SOFTBUS_KEY_RESP_VALUE] = true;
    std::string smsg = jsonStr.dump();
    DistributedInputSinkTransport::GetInstance().RespUnprepareRemoteInput(sessionId, smsg);
}

void DistributedInputSinkManager::DInputSinkListener::onStartRemoteInput(
    const int32_t& sessionId, const uint32_t& inputTypes)
{
    int32_t curSessionId = DistributedInputSinkSwitch::GetInstance().GetSwitchOpenedSession();
    DHLOGI("onStartRemoteInput called, curSessionId:%d, new sessionId: %d",
        curSessionId, sessionId);
    // set new session
    int32_t startRes = DistributedInputSinkSwitch::GetInstance().StartSwitch(sessionId);

    sinkManagerObj_->SetStartTransFlag((startRes == SUCCESS) ?
        DInputServerType::SINK_SERVER_TYPE :
        DInputServerType::NULL_SERVER_TYPE);

    bool result = (startRes == SUCCESS) ? true : false;
    nlohmann::json jsonStrSta;
    jsonStrSta[DINPUT_SOFTBUS_KEY_CMD_TYPE] = TRANS_SINK_MSG_ONSTART;
    jsonStrSta[DINPUT_SOFTBUS_KEY_INPUT_TYPE] = inputTypes;
    jsonStrSta[DINPUT_SOFTBUS_KEY_RESP_VALUE] = result;
    std::string smsgSta = jsonStrSta.dump();

    DistributedInputSinkTransport::GetInstance().RespStartRemoteInput(sessionId, smsgSta);

    // Notify the interrupted master status
    if (FAILURE == curSessionId) {
        DHLOGI("onStartRemoteInput called, this is the only session.");
    } else if (result) {
        DHLOGI("onStartRemoteInput called, notify curSessionId:%d Interrupted.", curSessionId);
        nlohmann::json jsonStrStp;
        jsonStrStp[DINPUT_SOFTBUS_KEY_CMD_TYPE] = TRANS_SINK_MSG_ONSTOP;
        jsonStrStp[DINPUT_SOFTBUS_KEY_INPUT_TYPE] = inputTypes;
        jsonStrStp[DINPUT_SOFTBUS_KEY_RESP_VALUE] = false;
        std::string smsgStp = jsonStrStp.dump();
        DistributedInputSinkTransport::GetInstance().RespStopRemoteInput(curSessionId, smsgStp);
        sinkManagerObj_->SetInputTypes(INPUT_TYPE_NULL);
        DistributedInputCollector::GetInstance().SetInputTypes(sinkManagerObj_->GetInputTypes());
    } else {
        DHLOGI("onStartRemoteInput called, new session is error, not notify curSession.");
    }

    // add the input type
    if (startRes == SUCCESS) {
        sinkManagerObj_->SetInputTypes(sinkManagerObj_->GetInputTypes() | inputTypes);
        DistributedInputCollector::GetInstance().SetInputTypes(sinkManagerObj_->GetInputTypes());
    }

    IStartDInputServerCallback *startServerCB = sinkManagerObj_->GetStartDInputServerCback();
    if (startServerCB == nullptr) {
        DHLOGE("onStartRemoteInput called, startServerCB is null.");
    } else {
        startServerCB->OnResult(
            static_cast<int32_t>(sinkManagerObj_->GetStartTransFlag()), sinkManagerObj_->GetInputTypes());
    }
}

void DistributedInputSinkManager::DInputSinkListener::onStopRemoteInput(
    const int32_t& sessionId, const uint32_t& inputTypes)
{
    DHLOGI("onStopRemoteInput called, sessionId: %d, inputTypes: %d", sessionId, inputTypes);

    sinkManagerObj_->SetInputTypes(sinkManagerObj_->GetInputTypes() -
        (sinkManagerObj_->GetInputTypes() & inputTypes));
    DistributedInputCollector::GetInstance().SetInputTypes(sinkManagerObj_->GetInputTypes());
    nlohmann::json jsonStr;
    jsonStr[DINPUT_SOFTBUS_KEY_CMD_TYPE] = TRANS_SINK_MSG_ONSTOP;
    jsonStr[DINPUT_SOFTBUS_KEY_INPUT_TYPE] = inputTypes;
    jsonStr[DINPUT_SOFTBUS_KEY_RESP_VALUE] = true;
    std::string smsg = jsonStr.dump();
    DistributedInputSinkTransport::GetInstance().RespStopRemoteInput(sessionId, smsg);

    if (sinkManagerObj_->GetInputTypes() == INPUT_TYPE_NULL) {
        DistributedInputSinkSwitch::GetInstance().StopSwitch(sessionId);
        if (DistributedInputSinkSwitch::GetInstance().GetSwitchOpenedSession() == FAILURE) {
            DHLOGI("onStartRemoteInput called, all session is stop.");
            sinkManagerObj_->SetStartTransFlag(DInputServerType::NULL_SERVER_TYPE);
        }
    }

    IStartDInputServerCallback *startServerCB = sinkManagerObj_->GetStartDInputServerCback();
    if (startServerCB == nullptr) {
        DHLOGE("onStartRemoteInput called, startServerCB is null.");
    } else {
        startServerCB->OnResult(
            static_cast<int32_t>(sinkManagerObj_->GetStartTransFlag()), sinkManagerObj_->GetInputTypes());
    }
}

void DistributedInputSinkManager::OnStart()
{
    if (serviceRunningState_ == ServiceSinkRunningState::STATE_RUNNING) {
        DHLOGI("dinput Manager Service has already started.");
        return;
    }
    DHLOGI("dinput Manager Service started.");
    if (!InitAuto()) {
        DHLOGI("failed to init service.");
        return;
    }
    serviceRunningState_ = ServiceSinkRunningState::STATE_RUNNING;
    runner_->Run();
    /* Publish service maybe failed, so we need call this function at the last,
     * so it can't affect the TDD test program */
    bool ret = Publish(this);
    if (!ret) {
        return;
    }

    DHLOGI("DistributedInputSinkManager  start success.");
}

bool DistributedInputSinkManager::InitAuto()
{
    runner_ = AppExecFwk::EventRunner::Create(true);
    if (runner_ == nullptr) {
        return false;
    }

    handler_ = std::make_shared<DistributedInputSinkEventHandler>(runner_);
    DHLOGI("init success");
    return true;
}

void DistributedInputSinkManager::OnStop()
{
    DHLOGI("stop service");
    runner_.reset();
    handler_.reset();
    serviceRunningState_ = ServiceSinkRunningState::STATE_NOT_START;
}

/**
 * get event handler
 *
 * @return event handler object.
 */
std::shared_ptr<DistributedInputSinkEventHandler> DistributedInputSinkManager::GetEventHandler()
{
    return handler_;
}

int32_t DistributedInputSinkManager::Init()
{
    DHLOGI("enter");
    isStartTrans_ = DInputServerType::NULL_SERVER_TYPE;
    // transport init session
    int32_t ret = DistributedInputSinkTransport::GetInstance().Init();
    if (SUCCESS != ret) {
        return FAILURE;
    }

    statuslistener_ = std::make_shared<DInputSinkListener>(this);
    DistributedInputSinkTransport::GetInstance().RegistSinkRespCallback(statuslistener_);

    serviceRunningState_ = ServiceSinkRunningState::STATE_RUNNING;

    return SUCCESS;
}

int32_t DistributedInputSinkManager::Release()
{
    DHLOGI("exit");

    // 1.stop all session switch
    DistributedInputSinkSwitch::GetInstance().StopAllSwitch();
    // 2.close all session
    DistributedInputSinkTransport::GetInstance().CloseAllSession();
    // 3.stop event collect
    // DistributedInputCollector::GetInstance().;

    // 4.notify callback servertype
    SetStartTransFlag(DInputServerType::NULL_SERVER_TYPE);
    IStartDInputServerCallback *startServerCB = GetStartDInputServerCback();
    if (startServerCB == nullptr) {
        DHLOGE("Release() called, startServerCB is null.");
    } else {
        startServerCB->OnResult(0, 0);
    }

    serviceRunningState_ = ServiceSinkRunningState::STATE_NOT_START;
    DHLOGI("exit dinput sink sa.");
    exit(0);

    return SUCCESS;
}

int32_t DistributedInputSinkManager::IsStartDistributedInput(
    const uint32_t& inputType, sptr<IStartDInputServerCallback> callback)
{
    if (callback != nullptr) {
        startServerCallback_ = callback;
    }

    if (inputType & GetInputTypes()) {
        return static_cast<int32_t>(isStartTrans_);
    } else {
        return static_cast<int32_t>(DInputServerType::NULL_SERVER_TYPE);
    }
}

IStartDInputServerCallback* DistributedInputSinkManager::GetStartDInputServerCback()
{
    return startServerCallback_;
}

DInputServerType DistributedInputSinkManager::GetStartTransFlag()
{
    return isStartTrans_;
}
void DistributedInputSinkManager::SetStartTransFlag(const DInputServerType flag)
{
    isStartTrans_ = flag;
}

bool DistributedInputSinkManager::GetInitWhiteListFlag()
{
    return isAlreadyInitWhiteList_;
}

void DistributedInputSinkManager::SetInitWhiteListFlag(bool isInit)
{
    isAlreadyInitWhiteList_ = isInit;
}

int32_t DistributedInputSinkManager::GetInputTypes()
{
    return input_types_;
}
void DistributedInputSinkManager::SetInputTypes(const uint32_t& inputTypess)
{
    input_types_ = inputTypess;
}
} // namespace DistributedInput
} // namespace DistributedHardware
} // namespace OHOS
