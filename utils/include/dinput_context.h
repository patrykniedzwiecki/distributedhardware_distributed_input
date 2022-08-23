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

#ifndef OHOS_DISTRIBUTED_INPUT_CONTEXT_H
#define OHOS_DISTRIBUTED_INPUT_CONTEXT_H

#include <cstdint>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include "single_instance.h"

#include "distributed_hardware_fwk_kit.h"
#include "distributed_hardware_log.h"

#include "input_hub.h"

namespace OHOS {
namespace DistributedHardware {
namespace DistributedInput {
struct LocalAbsInfo {
    int32_t absXMin = 0;
    int32_t absXMax = 0;
    int32_t absYMin = 0;
    int32_t absYMax = 0;
    int32_t absPressureMin = 0;
    int32_t absPressureMax = 0;
    int32_t absMtTouchMajorMin = 0;
    int32_t absMtTouchMajorMax = 0;
    int32_t absMtTouchMinorMin = 0;
    int32_t absMtTouchMinorMax = 0;
    int32_t absMtOrientationMin = 0;
    int32_t absMtOrientationMax = 0;
    int32_t absMtPositionXMin = 0;
    int32_t absMtPositionXMax = 0;
    int32_t absMtPositionYMin = 0;
    int32_t absMtPositionYMax = 0;
    int32_t absMtBlobIdMin = 0;
    int32_t absMtBlobIdMax = 0;
    int32_t absMtTrackingIdMin = 0;
    int32_t absMtTrackingIdMax = 0;
    int32_t absMtPressureMin = 0;
    int32_t absMtPressureMax = 0;
    InputDevice deviceInfo;
};

struct LocalTouchScreenInfo {
    uint32_t sinkShowWidth = 0;
    uint32_t sinkShowHeight = 0;
    uint32_t sinkPhyWidth = 0;
    uint32_t sinkPhyHeight = 0;
    LocalAbsInfo localAbsInfo;
};

struct SrcScreenInfo {
    std::string devId = "";             // source device id
    std::string uuid = "";              // source device uuid
    int32_t sessionId = 0;              // source device session id
    uint64_t sourceWinId = 0;           // source projection window id
    uint32_t sourceWinWidth = 0;        // source projection window width
    uint32_t sourceWinHeight = 0;       // source projection window height
    std::string sourcePhyId = "";       // source virtual screen driver id
    uint32_t sourcePhyFd = 0;           // source virtual screen driver fd
    uint32_t sourcePhyWidth = 0;        // source virtual screen driver width
    uint32_t sourcePhyHeight = 0;       // source virtual screen driver height
};

struct TransformInfo {
    uint32_t sinkWinPhyX = 0;           // projection area X coordinate in touch coordinate
    uint32_t sinkWinPhyY = 0;           // projection area Y coordinate in touch coordinate
    uint32_t sinkProjPhyWidth = 0;      // projection area width in touch coordinate
    uint32_t sinkProjPhyHeight = 0;     // projection area height in touch coordinate
    double coeffWidth = 0.0;            // sink width transform source coefficient
    double coeffHeight = 0.0;           // sink height transform source coefficient
};

struct SinkScreenInfo {
    uint32_t sinkShowWidth = 0;         // sink show width
    uint32_t sinkShowHeight = 0;        // sink show height
    uint32_t sinkPhyWidth = 0;          // sink touch screen width
    uint32_t sinkPhyHeight = 0;         // sink touch screen height
    uint64_t sinkShowWinId = 0;         // sink show window id
    uint32_t sinkWinShowX = 0;          // sink show window x coordinate
    uint32_t sinkWinShowY = 0;          // sink show window y coordinate
    uint32_t sinkProjShowWidth = 0;     // sink show window width
    uint32_t sinkProjShowHeight = 0;    // sink show window height
    SrcScreenInfo srcScreenInfo;
    TransformInfo transformInfo;
};

class DInputContext {
DECLARE_SINGLE_INSTANCE_BASE(DInputContext);
public:
    std::string GetScreenInfoKey(const std::string &devId, const uint64_t sourceWinId);
    int32_t RemoveSinkScreenInfo(const std::string &screenInfoKey);
    int32_t UpdateSinkScreenInfo(const std::string &screenInfoKey, const SinkScreenInfo &sinkScreenInfo);
    SinkScreenInfo GetSinkScreenInfo(const std::string &screenInfoKey);
    const std::unordered_map<std::string, SinkScreenInfo>& GetAllSinkScreenInfo();

    int32_t RemoveSrcScreenInfo(const std::string &screenInfoKey);
    int32_t UpdateSrcScreenInfo(const std::string &screenInfoKey, const SrcScreenInfo &srcScreenInfo);
    SrcScreenInfo GetSrcScreenInfo(const std::string &screenInfoKey);

    void SetLocalTouchScreenInfo(const LocalTouchScreenInfo &localTouchScreenInfo);
    LocalTouchScreenInfo GetLocalTouchScreenInfo();
    std::shared_ptr<DistributedHardwareFwkKit> GetDHFwkKit();

private:
    int32_t CalculateTransformInfo(SinkScreenInfo &sinkScreenInfo);

private:
    DInputContext() = default;
    ~DInputContext();

    /* the key is Combination of sink's localDeviceId and windowId, the value is sinkScreenInfo */
    std::unordered_map<std::string, SinkScreenInfo> sinkScreenInfoMap_;

    /* the key is Combination of source's localDeviceId and windowId, the value is sourceScreenInfo */
    std::unordered_map<std::string, SrcScreenInfo> srcScreenInfoMap_;
    LocalTouchScreenInfo localTouchScreenInfo_;
    std::mutex srcMapMutex_;
    std::mutex sinkMapMutex_;
    std::mutex localTouchScreenInfoMutex_;
    std::shared_ptr<DistributedHardwareFwkKit> dhFwkKit_;
    std::mutex dhFwkKitMutex_;
};
} // namespace DistributedInput
} // namespace DistributedHardware
} // namespace OHOS

#endif // OHOS_DISTRIBUTED_INPUT_CONTEXT_H