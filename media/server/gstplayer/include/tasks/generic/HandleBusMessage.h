/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2022 Sky UK
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

#ifndef FIREBOLT_RIALTO_SERVER_TASKS_GENERIC_HANDLE_BUS_MESSAGE_H_
#define FIREBOLT_RIALTO_SERVER_TASKS_GENERIC_HANDLE_BUS_MESSAGE_H_

#include "GenericPlayerContext.h"
#include "IFlushWatcher.h"
#include "IGlibWrapper.h"
#include "IGstGenericPlayerClient.h"
#include "IGstGenericPlayerPrivate.h"
#include "IGstWrapper.h"
#include "IPlayerTask.h"
#include <gst/gst.h>
#include <memory>

namespace firebolt::rialto::server::tasks::generic
{
class HandleBusMessage : public IPlayerTask
{
public:
    HandleBusMessage(GenericPlayerContext &context, IGstGenericPlayerPrivate &player, IGstGenericPlayerClient *client,
                     const std::shared_ptr<firebolt::rialto::wrappers::IGstWrapper> &gstWrapper,
                     const std::shared_ptr<firebolt::rialto::wrappers::IGlibWrapper> &glibWrapper, GstMessage *message,
                     const IFlushWatcher &flushWatcher);
    ~HandleBusMessage() override;
    void execute() const override;

private:
    bool allSourcesEos() const;

    GenericPlayerContext &m_context;
    IGstGenericPlayerPrivate &m_player;
    IGstGenericPlayerClient *m_gstPlayerClient;
    std::shared_ptr<firebolt::rialto::wrappers::IGstWrapper> m_gstWrapper;
    std::shared_ptr<firebolt::rialto::wrappers::IGlibWrapper> m_glibWrapper;
    GstMessage *m_message;
    const IFlushWatcher &m_flushWatcher;
    bool m_isFlushOngoingDuringCreation;
    bool m_isAsyncFlushOngoingDuringCreation;
};
} // namespace firebolt::rialto::server::tasks::generic

#endif // FIREBOLT_RIALTO_SERVER_TASKS_GENERIC_HANDLE_BUS_MESSAGE_H_
