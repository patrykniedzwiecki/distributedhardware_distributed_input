/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

#include "dinput_errcode.h"
#include "distributed_input_sourcetrans_test.h"

using namespace testing::ext;
using namespace OHOS::DistributedHardware::DistributedInput;
using namespace std;
namespace OHOS {
namespace DistributedHardware {
namespace DistributedInput {
void DistributedInputSourceTransTest::SetUp()
{
}

void DistributedInputSourceTransTest::TearDown()
{
}

void DistributedInputSourceTransTest::SetUpTestCase()
{
}

void DistributedInputSourceTransTest::TearDownTestCase()
{
}

HWTEST_F(DistributedInputSourceTransTest, OpenInputSoftbus01, testing::ext::TestSize.Level0)
{
    std::string remoteDevId = "";
    int32_t ret = DistributedInputSourceTransport::GetInstance().OpenInputSoftbus(remoteDevId);

    EXPECT_EQ(ERR_DH_INPUT_SERVER_SOURCE_TRANSPORT_OPEN_SESSION_FAIL, ret);
}

HWTEST_F(DistributedInputSourceTransTest, OpenInputSoftbus02, testing::ext::TestSize.Level0)
{
    std::string remoteDevId = "f6d4c08647073e02e7a78f09473aa122ff57fc81c00981fcf5be989e7d112591";
    int32_t ret = DistributedInputSourceTransport::GetInstance().OpenInputSoftbusForRelay(remoteDevId);

    EXPECT_EQ(DH_SUCCESS, ret);
}

HWTEST_F(DistributedInputSourceTransTest, OpenInputSoftbus03, testing::ext::TestSize.Level0)
{
    std::string remoteDevId = "f6d4c08647073e02e7a78f09473aa122ff57fc81c00981fcf5be989e7d112591";
    int32_t ret = DistributedInputSourceTransport::GetInstance().Init();
    EXPECT_EQ(DH_SUCCESS, ret);
    ret = DistributedInputSourceTransport::GetInstance().OpenInputSoftbus(remoteDevId);

    EXPECT_EQ(DH_SUCCESS, ret);
}

HWTEST_F(DistributedInputSourceTransTest, Init, testing::ext::TestSize.Level0)
{
    int32_t ret = DistributedInputSourceTransport::GetInstance().Init();
    EXPECT_EQ(DH_SUCCESS, ret);
}

HWTEST_F(DistributedInputSourceTransTest, PrepareRemoteInput01, testing::ext::TestSize.Level0)
{
    std::string deviceId = "";
    int32_t ret = DistributedInputSourceTransport::GetInstance().PrepareRemoteInput(deviceId);
    EXPECT_EQ(ERR_DH_INPUT_SERVER_SOURCE_TRANSPORT_PREPARE_FAIL, ret);
}

HWTEST_F(DistributedInputSourceTransTest, PrepareRemoteInput02, testing::ext::TestSize.Level0)
{
    std::string deviceId = "tttt0001";
    int32_t ret = DistributedInputSourceTransport::GetInstance().PrepareRemoteInput(deviceId);
    EXPECT_EQ(ERR_DH_INPUT_SERVER_SOURCE_TRANSPORT_PREPARE_FAIL, ret);
}

HWTEST_F(DistributedInputSourceTransTest, PrepareRemoteInput03, testing::ext::TestSize.Level1)
{
    std::string deviceId = "f6d4c08647073e02e7a78f09473aa122ff57fc81c00981fcf5be989e7d112591";
    int32_t ret = DistributedInputSourceTransport::GetInstance().PrepareRemoteInput(deviceId);
    EXPECT_EQ(DH_SUCCESS, ret);
}

HWTEST_F(DistributedInputSourceTransTest, UnprepareRemoteInput01, testing::ext::TestSize.Level0)
{
    std::string deviceId = "";
    int32_t ret = DistributedInputSourceTransport::GetInstance().UnprepareRemoteInput(deviceId);
    EXPECT_EQ(ERR_DH_INPUT_SERVER_SOURCE_TRANSPORT_UNPREPARE_FAIL, ret);
}

HWTEST_F(DistributedInputSourceTransTest, UnprepareRemoteInput02, testing::ext::TestSize.Level0)
{
    std::string deviceId = "tttt0001";
    int32_t ret = DistributedInputSourceTransport::GetInstance().UnprepareRemoteInput(deviceId);
    EXPECT_EQ(ERR_DH_INPUT_SERVER_SOURCE_TRANSPORT_UNPREPARE_FAIL, ret);
}

HWTEST_F(DistributedInputSourceTransTest, UnprepareRemoteInput03, testing::ext::TestSize.Level0)
{
    std::string deviceId = "f6d4c08647073e02e7a78f09473aa122ff57fc81c00981fcf5be989e7d112591";
    int32_t ret = DistributedInputSourceTransport::GetInstance().UnprepareRemoteInput(deviceId);
    EXPECT_EQ(DH_SUCCESS, ret);
}

HWTEST_F(DistributedInputSourceTransTest, StartRemoteInput01, testing::ext::TestSize.Level0)
{
    std::string deviceId = "tttt0001";
    int32_t ret = DistributedInputSourceTransport::GetInstance().StartRemoteInput(
        deviceId, static_cast<uint32_t>(DInputDeviceType::ALL));
    EXPECT_EQ(ERR_DH_INPUT_SERVER_SOURCE_TRANSPORT_START_FAIL, ret);
}

HWTEST_F(DistributedInputSourceTransTest, StartRemoteInput02, testing::ext::TestSize.Level0)
{
    std::string deviceId = "f6d4c08647073e02e7a78f09473aa122ff57fc81c00981fcf5be989e7d112591";
    int32_t ret = DistributedInputSourceTransport::GetInstance().StartRemoteInput(
        deviceId, static_cast<uint32_t>(DInputDeviceType::ALL));
    EXPECT_EQ(DH_SUCCESS, ret);
}

HWTEST_F(DistributedInputSourceTransTest, StopRemoteInput01, testing::ext::TestSize.Level0)
{
    std::string deviceId = "tttt0001";
    int32_t ret = DistributedInputSourceTransport::GetInstance().StopRemoteInput(
        deviceId, static_cast<uint32_t>(DInputDeviceType::ALL));
    EXPECT_EQ(ERR_DH_INPUT_SERVER_SOURCE_TRANSPORT_STOP_FAIL, ret);
}

HWTEST_F(DistributedInputSourceTransTest, StopRemoteInput02, testing::ext::TestSize.Level0)
{
    std::string deviceId = "f6d4c08647073e02e7a78f09473aa122ff57fc81c00981fcf5be989e7d112591";
    int32_t sessionId = 2;
    int32_t ret = DistributedInputSourceTransport::GetInstance().StopRemoteInput(
        deviceId, static_cast<uint32_t>(DInputDeviceType::ALL));
    EXPECT_EQ(DH_SUCCESS, ret);
    DistributedInputSourceTransport::GetInstance().CloseInputSoftbus(sessionId);
}

HWTEST_F(DistributedInputSourceTransTest, NotifyOriginPrepareResult01, testing::ext::TestSize.Level0)
{
    std::string srcId = "f6d4c08647073e02e7a78f09473aa122ff57fc81c00981fcf5be989e7d112591";
    std::string sinkId = "networkidc08647073e02e7a78f09473aa122ff57fc81c00";
    int32_t sessionId = 2;
    int32_t ret = DistributedInputSourceTransport::GetInstance().NotifyOriginPrepareResult(
        sessionId, srcId, sinkId, DH_SUCCESS);
    EXPECT_EQ(DH_SUCCESS, ret);
}

HWTEST_F(DistributedInputSourceTransTest, NotifyOriginUnprepareResult01, testing::ext::TestSize.Level0)
{
    std::string srcId = "f6d4c08647073e02e7a78f09473aa122ff57fc81c00981fcf5be989e7d112591";
    std::string sinkId = "networkidc08647073e02e7a78f09473aa122ff57fc81c00";
    int32_t sessionId = 2;
    int32_t ret = DistributedInputSourceTransport::GetInstance().NotifyOriginUnprepareResult(
        sessionId, srcId, sinkId, DH_SUCCESS);
    EXPECT_EQ(DH_SUCCESS, ret);
}

HWTEST_F(DistributedInputSourceTransTest, NotifyOriginStartDhidResult01, testing::ext::TestSize.Level0)
{
    std::string srcId = "f6d4c08647073e02e7a78f09473aa122ff57fc81c00981fcf5be989e7d112591";
    std::string sinkId = "networkidc08647073e02e7a78f09473aa122ff57fc81c00";
    int32_t sessionId = 2;
    std::string dhIds = "Input_1ds56v18e1v21v8v1erv15r1v8r1j1ty8";
    int32_t ret = DistributedInputSourceTransport::GetInstance().NotifyOriginStartDhidResult(
        sessionId, srcId, sinkId, DH_SUCCESS, dhIds);
    EXPECT_EQ(DH_SUCCESS, ret);
}

HWTEST_F(DistributedInputSourceTransTest, NotifyOriginStopDhidResult01, testing::ext::TestSize.Level0)
{
    std::string srcId = "f6d4c08647073e02e7a78f09473aa122ff57fc81c00981fcf5be989e7d112591";
    std::string sinkId = "networkidc08647073e02e7a78f09473aa122ff57fc81c00";
    int32_t sessionId = 2;
    std::string dhIds = "Input_1ds56v18e1v21v8v1erv15r1v8r1j1ty8";
    int32_t ret = DistributedInputSourceTransport::GetInstance().NotifyOriginStopDhidResult(
        sessionId, srcId, sinkId, DH_SUCCESS, dhIds);
    EXPECT_EQ(DH_SUCCESS, ret);
}
} // namespace DistributedInput
} // namespace DistributedHardware
} // namespace OHOS
