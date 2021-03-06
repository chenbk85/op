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
#include <zsLib/String.h>

namespace hookflash
{
  namespace services
  {
    interaction IRUDPChannel
    {
      typedef zsLib::PUID PUID;
      typedef zsLib::ULONG ULONG;
      typedef zsLib::BYTE BYTE;
      typedef zsLib::String String;
      typedef zsLib::IPAddress IPAddress;

      enum RUDPChannelStates
      {
        RUDPChannelState_Connecting,
        RUDPChannelState_Connected,
        RUDPChannelState_ShuttingDown,
        RUDPChannelState_Shutdown,
      };

      static const char *toString(RUDPChannelStates state);

      enum RUDPChannelShutdownReasons
      {
        RUDPChannelShutdownReason_None,

        RUDPChannelShutdownReason_Closed,

        RUDPChannelShutdownReason_OpenFailure,
        RUDPChannelShutdownReason_DelegateGone,
        RUDPChannelShutdownReason_Timeout,
        RUDPChannelShutdownReason_IllegalStreamState,
      };

      static const char *toString(RUDPChannelShutdownReasons reason);

      enum Shutdown
      {
        Shutdown_None =     (0x00),
        Shutdown_Send =     (0x01),
        Shutdown_Receive =  (0x02),
        Shutdown_Both =     (0x03)
      };

      static const char *toString(Shutdown value);

      enum CongestionAlgorithms
      {
        CongestionAlgorithm_None =                         0,
        CongestionAlgorithm_TCPLikeWindowWithSlowCreepUp = 1,
      };

      static const char *toString(CongestionAlgorithms value);

      virtual PUID getID() const = 0;

      virtual RUDPChannelStates getState() const = 0;
      virtual RUDPChannelShutdownReasons getShutdownReason() const = 0;

      //-----------------------------------------------------------------------
      // PURPOSE: This closes the session gracefully.
      virtual void shutdown() = 0;

      //-----------------------------------------------------------------------
      // PURPOSE: This shutsdown the send/receive state but does not close the
      //          channel session. Use shutdown() to actually shutdown the
      //          channel fully.
      virtual void shutdownDirection(Shutdown state) = 0;

      virtual bool send(
                        const BYTE *buffer,
                        ULONG bufferLengthInBytes
                        ) = 0;

      virtual ULONG getReceiveSizeAvailableInBytes() = 0;
      virtual ULONG receive(
                            BYTE *outBuffer,
                            ULONG bufferLengthInBytes
                            ) = 0;

      virtual IPAddress getConnectedRemoteIP() = 0;

      //-----------------------------------------------------------------------
      // PURPOSE: Return connection information as reported by the remote party
      // NOTE:    Will return an empty string until connected.
      virtual String getRemoteConnectionInfo() = 0;
    };

    interaction IRUDPChannelDelegate
    {
      typedef services::IRUDPChannelPtr IRUDPChannelPtr;
      typedef IRUDPChannel::RUDPChannelStates RUDPChannelStates;

      virtual void onRDUPChannelStateChanged(
                                             IRUDPChannelPtr session,
                                             RUDPChannelStates state
                                             ) = 0;

      virtual void onRUDPChannelReadReady(IRUDPChannelPtr session) = 0;
      virtual void onRUDPChannelWriteReady(IRUDPChannelPtr session) = 0;
    };
  }
}

ZS_DECLARE_PROXY_BEGIN(hookflash::services::IRUDPChannelDelegate)
ZS_DECLARE_PROXY_TYPEDEF(hookflash::services::IRUDPChannelPtr, IRUDPChannelPtr)
ZS_DECLARE_PROXY_TYPEDEF(hookflash::services::IRUDPChannelDelegate::RUDPChannelStates, RUDPChannelStates)
ZS_DECLARE_PROXY_METHOD_2(onRDUPChannelStateChanged, IRUDPChannelPtr, RUDPChannelStates)
ZS_DECLARE_PROXY_METHOD_1(onRUDPChannelReadReady, IRUDPChannelPtr)
ZS_DECLARE_PROXY_METHOD_1(onRUDPChannelWriteReady, IRUDPChannelPtr)
ZS_DECLARE_PROXY_END()
