/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2024 Sky UK
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "MediaPipelineProxy.h"
#include "MediaPipelineAndControlClientMock.h"
#include "MediaPipelineTestBase.h"

using ::testing::_;
using ::testing::DoAll;
using ::testing::DoubleEq;
using ::testing::SetArgReferee;
using ::testing::StrEq;

class RialtoClientMediaPipelineProxyTest : public MediaPipelineTestBase
{
protected:
    virtual void SetUp() { MediaPipelineTestBase::SetUp(); }

    virtual void TearDown() { MediaPipelineTestBase::TearDown(); }
};

/**
 * Test that the proxy passes all methods through to the underlying object
 */
TEST_F(RialtoClientMediaPipelineProxyTest, TestPassthrough)
{
    EXPECT_CALL(*m_clientControllerMock, registerClient(_, _))
        .WillOnce(DoAll(SetArgReferee<1>(ApplicationState::RUNNING), Return(true)));
    EXPECT_CALL(*m_clientControllerMock, unregisterClient(_)).WillOnce(Return(true));

    auto mediaPipelineMock = std::make_shared<StrictMock<MediaPipelineAndControlClientMock>>();

    EXPECT_CALL(*mediaPipelineMock, notifyApplicationState(ApplicationState::RUNNING));

    std::shared_ptr<MediaPipelineProxy> proxy;
    EXPECT_NO_THROW(proxy = std::make_shared<MediaPipelineProxy>(mediaPipelineMock, *m_clientControllerMock));

    /////////////////////////////////////////////

    const std::string kMimeType{"mime"};
    const std::string kUrl{"url"};
    const std::unique_ptr<firebolt::rialto::IMediaPipeline::MediaSource> kMediaSource;
    const int kSourceId{2};
    const double kPlaybackRate{2.1};
    const uint32_t kVolumeDuration{1000};
    const double kTargetVolume{3.1};
    const EaseType kEaseType{EaseType::EASE_LINEAR};
    const double kCurrentVolume{4.1};
    const int64_t kPosition1{123};
    const int64_t kPosition2{234};
    const uint32_t kNeedDataRequestId{5};
    constexpr uint64_t kRenderedFrames{5432};
    constexpr uint64_t kDroppedFrames{51};
    constexpr uint32_t kDuration{5432};
    constexpr int64_t kDiscontinuityGap{1};
    constexpr bool kIsAudioAac{false};
    const std::unique_ptr<firebolt::rialto::IMediaPipeline::MediaSegment> kMediaSegment;
    const std::shared_ptr<IMediaPipelineClient> kIMediaPipelineClient;
    constexpr bool kResetTime{false};
    constexpr double kAppliedRate{2.0};
    constexpr bool kEnabled{true};
    constexpr uint32_t kBufferingLimit{5326};
    constexpr uint64_t kStopPosition{4234};

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, load(MediaType::MSE, StrEq(kMimeType), StrEq(kUrl))).WillOnce(Return(true));
    EXPECT_TRUE(proxy->load(MediaType::MSE, kMimeType, kUrl));

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, attachSource(_)).WillOnce(Return(true));
    EXPECT_TRUE(proxy->attachSource(kMediaSource));

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, removeSource(kSourceId)).WillOnce(Return(true));
    EXPECT_TRUE(proxy->removeSource(kSourceId));

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, allSourcesAttached()).WillOnce(Return(true));
    EXPECT_TRUE(proxy->allSourcesAttached());

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, play()).WillOnce(Return(true));
    EXPECT_TRUE(proxy->play());

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, pause()).WillOnce(Return(true));
    EXPECT_TRUE(proxy->pause());

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, stop()).WillOnce(Return(true));
    EXPECT_TRUE(proxy->stop());

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, setPlaybackRate(DoubleEq(kPlaybackRate))).WillOnce(Return(true));
    EXPECT_TRUE(proxy->setPlaybackRate(kPlaybackRate));

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, setPosition(kPosition1)).WillOnce(Return(true));
    EXPECT_TRUE(proxy->setPosition(kPosition1));

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, getPosition(_)).WillOnce(DoAll(SetArgReferee<0>(kPosition2), Return(true)));
    {
        int64_t position;
        EXPECT_TRUE(proxy->getPosition(position));
        EXPECT_EQ(position, kPosition2);
    }

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, getStats(_, _, _))
        .WillOnce(DoAll(SetArgReferee<1>(kRenderedFrames), SetArgReferee<2>(kDroppedFrames), Return(true)));
    {
        uint64_t renderedFrames;
        uint64_t droppedFrames;
        EXPECT_TRUE(proxy->getStats(kSourceId, renderedFrames, droppedFrames));
        EXPECT_EQ(renderedFrames, kRenderedFrames);
        EXPECT_EQ(droppedFrames, kDroppedFrames);
    }

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, setImmediateOutput(kSourceId, true)).WillOnce(Return(true));
    EXPECT_TRUE(proxy->setImmediateOutput(kSourceId, true));

    /////////////////////////////////////////////

    {
        const bool kImmediateOutputState{true};
        bool returnImmediateOutputState;
        EXPECT_CALL(*mediaPipelineMock, getImmediateOutput(kSourceId, _))
            .WillOnce(DoAll(SetArgReferee<1>(kImmediateOutputState), Return(true)));
        EXPECT_TRUE(proxy->getImmediateOutput(kSourceId, returnImmediateOutputState));
        EXPECT_EQ(returnImmediateOutputState, kImmediateOutputState);
    }

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, setVideoWindow(1, 2, 3, 4)).WillOnce(Return(true));
    EXPECT_TRUE(proxy->setVideoWindow(1, 2, 3, 4));

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, haveData(MediaSourceStatus::CODEC_CHANGED, kNeedDataRequestId)).WillOnce(Return(true));
    EXPECT_TRUE(proxy->haveData(MediaSourceStatus::CODEC_CHANGED, kNeedDataRequestId));

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, addSegment(kNeedDataRequestId, _)).WillOnce(Return(AddSegmentStatus::OK));
    EXPECT_EQ(proxy->addSegment(kNeedDataRequestId, kMediaSegment), AddSegmentStatus::OK);

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, renderFrame()).WillOnce(Return(true));
    EXPECT_TRUE(proxy->renderFrame());

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, setVolume(DoubleEq(kTargetVolume), kVolumeDuration, kEaseType)).WillOnce(Return(true));
    EXPECT_TRUE(proxy->setVolume(kTargetVolume, kVolumeDuration, kEaseType));

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, getVolume(_)).WillOnce(DoAll(SetArgReferee<0>(kCurrentVolume), Return(true)));
    {
        // The EXPECT_CALL above returns kCurrentVolume
        double currentVolume;
        EXPECT_TRUE(proxy->getVolume(currentVolume));
        EXPECT_EQ(currentVolume, kCurrentVolume);
    }

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, setMute(kSourceId, true)).WillOnce(Return(true));
    EXPECT_TRUE(proxy->setMute(kSourceId, true));

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, getMute(kSourceId, _)).WillOnce(DoAll(SetArgReferee<1>(false), Return(true)));
    {
        // The EXPECT_CALL above returns false
        bool mute;
        EXPECT_TRUE(proxy->getMute(kSourceId, mute));
        EXPECT_FALSE(mute);
    }

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, setLowLatency(true)).WillOnce(Return(true));
    EXPECT_TRUE(proxy->setLowLatency(true));

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, setSync(true)).WillOnce(Return(true));
    EXPECT_TRUE(proxy->setSync(true));

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, getSync(_)).WillOnce(DoAll(SetArgReferee<0>(false), Return(true)));
    {
        bool sync;
        EXPECT_TRUE(proxy->getSync(sync));
        EXPECT_FALSE(sync);
    }

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, setSyncOff(true)).WillOnce(Return(true));
    EXPECT_TRUE(proxy->setSyncOff(true));

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, setStreamSyncMode(kSourceId, 1)).WillOnce(Return(true));
    EXPECT_TRUE(proxy->setStreamSyncMode(kSourceId, 1));

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, getStreamSyncMode(_)).WillOnce(DoAll(SetArgReferee<0>(1), Return(true)));
    {
        int32_t streamSyncMode;
        EXPECT_TRUE(proxy->getStreamSyncMode(streamSyncMode));
        EXPECT_EQ(streamSyncMode, 1);
    }

    /////////////////////////////////////////////

    bool isAsync{false};
    EXPECT_CALL(*mediaPipelineMock, flush(kSourceId, true, _)).WillOnce(Return(true));
    EXPECT_TRUE(proxy->flush(kSourceId, true, isAsync));

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, setSourcePosition(kSourceId, kPosition1, kResetTime, kAppliedRate, kStopPosition))
        .WillOnce(Return(true));
    EXPECT_TRUE(proxy->setSourcePosition(kSourceId, kPosition1, kResetTime, kAppliedRate, kStopPosition));

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, processAudioGap(kPosition1, kDuration, kDiscontinuityGap, kIsAudioAac))
        .WillOnce(Return(true));
    EXPECT_TRUE(proxy->processAudioGap(kPosition1, kDuration, kDiscontinuityGap, kIsAudioAac));

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, setBufferingLimit(kBufferingLimit)).WillOnce(Return(true));
    EXPECT_TRUE(proxy->setBufferingLimit(kBufferingLimit));

    /////////////////////////////////////////////

    uint32_t bufferingLimit{0};
    EXPECT_CALL(*mediaPipelineMock, getBufferingLimit(bufferingLimit)).WillOnce(Return(true));
    EXPECT_TRUE(proxy->getBufferingLimit(bufferingLimit));

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, setUseBuffering(kEnabled)).WillOnce(Return(true));
    EXPECT_TRUE(proxy->setUseBuffering(kEnabled));

    /////////////////////////////////////////////

    bool useBuffering{false};
    EXPECT_CALL(*mediaPipelineMock, getUseBuffering(useBuffering)).WillOnce(Return(true));
    EXPECT_TRUE(proxy->getUseBuffering(useBuffering));

    /////////////////////////////////////////////

    const std::string kTextTrackIdentifier{"Identifier"};
    EXPECT_CALL(*mediaPipelineMock, setTextTrackIdentifier(kTextTrackIdentifier)).WillOnce(Return(true));
    EXPECT_TRUE(proxy->setTextTrackIdentifier(kTextTrackIdentifier));

    /////////////////////////////////////////////

    {
        std::string textTrackIdentifier;
        EXPECT_CALL(*mediaPipelineMock, getTextTrackIdentifier(_))
            .WillOnce(DoAll(SetArgReferee<0>(kTextTrackIdentifier), Return(true)));
        EXPECT_TRUE(proxy->getTextTrackIdentifier(textTrackIdentifier));
        EXPECT_EQ(textTrackIdentifier, kTextTrackIdentifier);
    }

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, getClient()).WillOnce(Return(kIMediaPipelineClient));
    EXPECT_EQ(proxy->getClient().lock(), kIMediaPipelineClient);

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, notifyApplicationState(ApplicationState::RUNNING));
    proxy->notifyApplicationState(ApplicationState::RUNNING);

    /////////////////////////////////////////////

    EXPECT_CALL(*mediaPipelineMock, switchSource(_)).WillOnce(Return(true));
    EXPECT_TRUE(proxy->switchSource(kMediaSource));
}
