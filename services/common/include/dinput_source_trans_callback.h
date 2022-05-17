/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#ifndef OHOS_DISTRIBUTED_INPUT_SOURCE_TRANS_H
#define OHOS_DISTRIBUTED_INPUT_SOURCE_TRANS_H

#include <string>

namespace OHOS {
namespace DistributedHardware {
namespace DistributedInput {
class DInputSourceTransCallback {
public:
    virtual void onResponseRegisterDistributedHardware(const std::string deviceId, const std::string dhId,
        bool result) = 0;
    virtual void onResponsePrepareRemoteInput(const std::string deviceId, bool result, const std::string &object) = 0;
    virtual void onResponseUnprepareRemoteInput(const std::string deviceId, bool result) = 0;
    virtual void onResponseStartRemoteInput(const std::string deviceId, const uint32_t inputTypes, bool result) = 0;
    virtual void onResponseStopRemoteInput(const std::string deviceId, const uint32_t inputTypes, bool result) = 0;
    virtual void onReceivedEventRemoteInput(const std::string deviceId, const std::string &object) = 0;
};
}
}
}
#endif