/*
 * Copyright 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <compositionengine/impl/CompositionEngine.h>
#include <ui/GraphicBuffer.h>

#include "BufferQueueLayer.h"
#include "BufferStateLayer.h"
#include "ColorLayer.h"
#include "ContainerLayer.h"
#include "DisplayDevice.h"
#include "Layer.h"
#include "NativeWindowSurface.h"
#include "StartPropertySetThread.h"
#include "SurfaceFlingerDefaultFactory.h"
#include "SurfaceInterceptor.h"

#include "DisplayHardware/ComposerHal.h"
#include "Scheduler/DispSync.h"
#include "Scheduler/EventControlThread.h"
#include "Scheduler/MessageQueue.h"
#include "Scheduler/PhaseOffsets.h"
#include "Scheduler/Scheduler.h"

namespace android::surfaceflinger {

DefaultFactory::~DefaultFactory() = default;

std::unique_ptr<DispSync> DefaultFactory::createDispSync(const char* name, bool hasSyncFramework) {
    return std::make_unique<android::impl::DispSync>(name, hasSyncFramework);
}

std::unique_ptr<EventControlThread> DefaultFactory::createEventControlThread(
        SetVSyncEnabled setVSyncEnabled) {
    return std::make_unique<android::impl::EventControlThread>(std::move(setVSyncEnabled));
}

std::unique_ptr<HWComposer> DefaultFactory::createHWComposer(const std::string& serviceName) {
    return std::make_unique<android::impl::HWComposer>(
            std::make_unique<Hwc2::impl::Composer>(serviceName));
}

std::unique_ptr<MessageQueue> DefaultFactory::createMessageQueue() {
    return std::make_unique<android::impl::MessageQueue>();
}

std::unique_ptr<scheduler::PhaseOffsets> DefaultFactory::createPhaseOffsets() {
    return std::make_unique<scheduler::impl::PhaseOffsets>();
}

std::unique_ptr<Scheduler> DefaultFactory::createScheduler(
        SetVSyncEnabled setVSyncEnabled, const scheduler::RefreshRateConfigs& configs) {
    return std::make_unique<Scheduler>(std::move(setVSyncEnabled), configs);
}

std::unique_ptr<SurfaceInterceptor> DefaultFactory::createSurfaceInterceptor(
        SurfaceFlinger* flinger) {
    return std::make_unique<android::impl::SurfaceInterceptor>(flinger);
}

sp<StartPropertySetThread> DefaultFactory::createStartPropertySetThread(
        bool timestampPropertyValue) {
    return new StartPropertySetThread(timestampPropertyValue);
}

sp<DisplayDevice> DefaultFactory::createDisplayDevice(DisplayDeviceCreationArgs&& creationArgs) {
    return new DisplayDevice(std::move(creationArgs));
}

sp<GraphicBuffer> DefaultFactory::createGraphicBuffer(uint32_t width, uint32_t height,
                                                      PixelFormat format, uint32_t layerCount,
                                                      uint64_t usage, std::string requestorName) {
    return new GraphicBuffer(width, height, format, layerCount, usage, requestorName);
}

void DefaultFactory::createBufferQueue(sp<IGraphicBufferProducer>* outProducer,
                                       sp<IGraphicBufferConsumer>* outConsumer,
                                       bool consumerIsSurfaceFlinger) {
    BufferQueue::createBufferQueue(outProducer, outConsumer, consumerIsSurfaceFlinger);
}

std::unique_ptr<surfaceflinger::NativeWindowSurface> DefaultFactory::createNativeWindowSurface(
        const sp<IGraphicBufferProducer>& producer) {
    return surfaceflinger::impl::createNativeWindowSurface(producer);
}

std::unique_ptr<compositionengine::CompositionEngine> DefaultFactory::createCompositionEngine() {
    return compositionengine::impl::createCompositionEngine();
}

sp<ContainerLayer> DefaultFactory::createContainerLayer(const LayerCreationArgs& args) {
    return new ContainerLayer(args);
}

sp<BufferQueueLayer> DefaultFactory::createBufferQueueLayer(const LayerCreationArgs& args) {
    return new BufferQueueLayer(args);
}

sp<BufferStateLayer> DefaultFactory::createBufferStateLayer(const LayerCreationArgs& args) {
    return new BufferStateLayer(args);
}

sp<ColorLayer> DefaultFactory::createColorLayer(const LayerCreationArgs& args) {
    return new ColorLayer(args);
}

} // namespace android::surfaceflinger
