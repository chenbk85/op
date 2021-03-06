/*
 
 Copyright (c) 2012, SMB Phone Inc.
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 The views and conclusions contained in the software and documentation are those
 of the authors and should not be interpreted as representing official policies,
 either expressed or implied, of the FreeBSD Project.
 
 */

#pragma once

#include <hookflash/services/hookflashTypes.h>
#include <zsLib/IPAddress.h>
#include <zsLib/Proxy.h>

#define HOOKFLASH_SERVICES_IRUDPLISTENER_DEFAULT_REALM "hookflash.com"

namespace hookflash
{
  namespace services
  {
    interaction IRUDPListener
    {
      typedef zsLib::PUID PUID;
      typedef zsLib::WORD WORD;
      typedef zsLib::IPAddress IPAddress;
      typedef zsLib::IMessageQueuePtr IMessageQueuePtr;

      enum RUDPListenerStates
      {
        RUDPListenerState_Listening,
        RUDPListenerState_ShuttingDown,
        RUDPListenerState_Shutdown,
      };

      static const char *toString(RUDPListenerStates state);

      static IRUDPListenerPtr create(
                                     IMessageQueuePtr queue,
                                     IRUDPListenerDelegatePtr delegate,
                                     WORD port = 0,
                                     const char *realm = HOOKFLASH_SERVICES_IRUDPLISTENER_DEFAULT_REALM
                                     );

      virtual PUID getID() const = 0;

      virtual IPAddress getListenerIP() = 0;

      virtual RUDPListenerStates getState() const = 0;

      //-----------------------------------------------------------------------
      // PURPOSE: Close the socket and cause all sessions to become closed.
      virtual void shutdown() = 0;

      virtual IRUDPChannelPtr acceptChannel(IRUDPChannelDelegatePtr delegate) = 0;
    };

    interaction IRUDPListenerDelegate
    {
      typedef services::IRUDPListenerPtr IRUDPListenerPtr;
      typedef IRUDPListener::RUDPListenerStates RUDPListenerStates;

      virtual void onRUDPListenerStateChanged(
                                              IRUDPListenerPtr listener,
                                              RUDPListenerStates state
                                              ) = 0;

      virtual void onRUDPListenerChannelWaiting(IRUDPListenerPtr listener) = 0;
    };

  }
}

ZS_DECLARE_PROXY_BEGIN(hookflash::services::IRUDPListenerDelegate)
ZS_DECLARE_PROXY_METHOD_2(onRUDPListenerStateChanged, hookflash::services::IRUDPListenerPtr, hookflash::services::IRUDPListenerDelegate::RUDPListenerStates)
ZS_DECLARE_PROXY_METHOD_1(onRUDPListenerChannelWaiting, hookflash::services::IRUDPListenerPtr)
ZS_DECLARE_PROXY_END()
