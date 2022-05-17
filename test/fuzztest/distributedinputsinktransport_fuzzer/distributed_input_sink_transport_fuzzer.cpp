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

#include "distributed_input_sink_transport_fuzzer.h"
#include "distributed_input_sink_transport.h"

#include <thread>
#include <functional>
#include <iostream>
#include <refbase.h>

#include "constants_dinput.h"

namespace OHOS {
namespace DistributedHardware {
void RespPrepareRemoteInputFuzzTest(const uint8_t* data, size_t size)
{
    if ((data == nullptr) || (size <= 0)) {
        return;
    }

    int32_t sessionId = *(reinterpret_cast<const int32_t*>(data));
    std::string smsg(reinterpret_cast<const char*>(data), size);
    
    DistributedInput::DistributedInputSinkTransport::GetInstance().RespPrepareRemoteInput(sessionId, smsg);
    DistributedInput::DistributedInputSinkTransport::GetInstance().RespUnprepareRemoteInput(sessionId, smsg);
}

void RespStartRemoteInputFuzzTest(const uint8_t* data, size_t size)
{
    if ((data == nullptr) || (size <= 0)) {
        return;
    }

    int32_t sessionId = *(reinterpret_cast<const int32_t*>(data));
    std::string smsg(reinterpret_cast<const char*>(data), size);
    
    DistributedInput::DistributedInputSinkTransport::GetInstance().RespStartRemoteInput(sessionId, smsg);
    DistributedInput::DistributedInputSinkTransport::GetInstance().RespStopRemoteInput(sessionId, smsg);
}

void OnSessionOpenedFuzzTest(const uint8_t* data, size_t size)
{
    if ((data == nullptr) || (size <= 0)) {
        return;
    }

    int32_t sessionId = *(reinterpret_cast<const int32_t*>(data));
    int32_t result = *(reinterpret_cast<const int32_t*>(data));

    DistributedInput::DistributedInputSinkTransport::GetInstance().OnSessionOpened(sessionId, result);
    DistributedInput::DistributedInputSinkTransport::GetInstance().OnSessionClosed(sessionId);
}

void OnBytesReceivedFuzzTest(const uint8_t* data, size_t size)
{
    if ((data == nullptr) || (size <= 0)) {
        return;
    }

    int32_t sessionId = *(reinterpret_cast<const int32_t*>(data));
    const char *msg = reinterpret_cast<const char *>(data);
    uint16_t dataLen = *(reinterpret_cast<const uint16_t*>(data));
    DistributedInput::DistributedInputSinkTransport::GetInstance().OnBytesReceived(sessionId, msg, dataLen);
}
}
}

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    OHOS::DistributedHardware::RespPrepareRemoteInputFuzzTest(data, size);
    OHOS::DistributedHardware::RespStartRemoteInputFuzzTest(data, size);
    OHOS::DistributedHardware::OnSessionOpenedFuzzTest(data, size);
    OHOS::DistributedHardware::OnBytesReceivedFuzzTest(data, size);
    return 0;
}