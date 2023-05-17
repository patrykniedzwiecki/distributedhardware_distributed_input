/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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

#include "add_white_list_infos_call_back_stub.h"

#include "string_ex.h"

#include "constants_dinput.h"
#include "dinput_errcode.h"
#include "dinput_log.h"

namespace OHOS {
namespace DistributedHardware {
namespace DistributedInput {
AddWhiteListInfosCallbackStub::AddWhiteListInfosCallbackStub()
{
}

AddWhiteListInfosCallbackStub::~AddWhiteListInfosCallbackStub()
{
}

int32_t AddWhiteListInfosCallbackStub::OnRemoteRequest(
    uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    if (data.ReadInterfaceToken() != GetDescriptor()) {
        DHLOGE("AddWhiteListInfosCallbackStub read token valid failed");
        return ERR_DH_INPUT_IPC_READ_TOKEN_VALID_FAIL;
    }
    IAddWhiteListInfosCallback::Message msgCode = static_cast<IAddWhiteListInfosCallback::Message>(code);
    switch (msgCode) {
        case IAddWhiteListInfosCallback::Message::RESULT: {
            std::string deviceId = data.ReadString();
            std::string strJsion = data.ReadString();
            OnResult(deviceId, strJsion);
            break;
        }
        default:
            return IPCObjectStub::OnRemoteRequest(code, data, reply, option);
    }
    return DH_SUCCESS;
}
} // namespace DistributedInput
} // namespace DistributedHardware
} // namespace OHOS
