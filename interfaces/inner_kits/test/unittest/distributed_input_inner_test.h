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

#ifndef DISRIBUTED_INPUT_INNER_TEST_H
#define DISRIBUTED_INPUT_INNER_TEST_H

#include <thread>
#include <functional>
#include <iostream>
#include <gtest/gtest.h>
#include <refbase.h>

#include "distributed_input_kit.h"
#include "i_distributed_sink_input.h"
#include "i_distributed_source_input.h"
#include "prepare_d_input_call_back_stub.h"
#include "unprepare_d_input_call_back_stub.h"
#include "start_d_input_call_back_stub.h"
#include "stop_d_input_call_back_stub.h"
#include "constants_dinput.h"

#include "iservice_registry.h"
#include "ipc_skeleton.h"
#include "system_ability_definition.h"

namespace OHOS {
namespace DistributedHardware {
namespace DistributedInput {
class DistributedInputInnerTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    virtual void SetUp() override;
    virtual void TearDown() override;
    int CheckSourceProxy() const;
    int CheckSinkProxy() const;

    class TestPrepareDInputCallback : public
        OHOS::DistributedHardware::DistributedInput::PrepareDInputCallbackStub {
    public:
        TestPrepareDInputCallback() = default;
        virtual ~TestPrepareDInputCallback() = default;
        void OnResult(const std::string& deviceId, const int32_t& status);
    };

    class TestUnprepareDInputCallback : public
        OHOS::DistributedHardware::DistributedInput::UnprepareDInputCallbackStub {
    public:
        TestUnprepareDInputCallback() = default;
        virtual ~TestUnprepareDInputCallback() = default;
        void OnResult(const std::string& deviceId, const int32_t& status);
    };

    class TestStartDInputCallback : public
        OHOS::DistributedHardware::DistributedInput::StartDInputCallbackStub {
    public:
        TestStartDInputCallback() = default;
        virtual ~TestStartDInputCallback() = default;
        void OnResult(const std::string& deviceId, const uint32_t& inputTypes, const int32_t& status);
    };

    class TestStopDInputCallback : public
        OHOS::DistributedHardware::DistributedInput::StopDInputCallbackStub {
    public:
        TestStopDInputCallback() = default;
        virtual ~TestStopDInputCallback() = default;
        void OnResult(const std::string& deviceId, const uint32_t& inputTypes, const int32_t& status);
    };
};
} // namespace DistributedInput
} // namespace DistributedHardware
} // namespace OHOS
#endif // DISRIBUTED_INPUT_INNER_TEST_H