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

#ifndef START_STOP_RESULT_CALL_BACK_PROXY_H
#define START_STOP_RESULT_CALL_BACK_PROXY_H

#include "i_start_stop_result_call_back.h"

#include <string>

#include "iremote_proxy.h"

namespace OHOS {
namespace DistributedHardware {
namespace DistributedInput {
class StartStopResultCallbackProxy : public IRemoteProxy<IStartStopResultCallback> {
public:
    explicit StartStopResultCallbackProxy(const sptr<IRemoteObject> &object);
    virtual ~StartStopResultCallbackProxy() override;

    virtual void OnStart(const std::string &srcId, const std::string &sinkId,
        std::vector<std::string> &dhIds) override;
    virtual void OnStop(const std::string &srcId, const std::string &sinkId,
        std::vector<std::string> &dhIds) override;

private:
    static inline BrokerDelegator<StartStopResultCallbackProxy> delegator_;
};
} // namespace DistributedInput
} // namespace DistributedHardware
} // namespace OHOS

#endif // START_STOP_RESULT_CALL_BACK_PROXY_H
