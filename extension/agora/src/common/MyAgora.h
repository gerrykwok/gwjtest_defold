#pragma once

#if defined(DM_PLATFORM_ANDROID)
#include "../android/AMG_Voice_Native_Android_HD_v2_2_3_20/IAgoraRtcEngine.h"
#elif defined(DM_PLATFORM_WINDOWS)
#include "../win32/Agora_Native_SDK_for_Windows_v2.0.8.3_FULL/IAgoraRtcEngine.h"
#elif defined(DM_PLATFORM_OSX)
#import <AgoraRtcEngineKit/IAgoraRtcEngine.h>
#elif defined(DM_PLATFORM_IOS)
#import <AgoraAudioKit/IAgoraRtcEngine.h>
#endif
using namespace agora::rtc;

class MyAgoraEventHandler : public IRtcEngineEventHandler
{
public:
	void onJoinChannelSuccess(const char* channel, uid_t userId, int elapsed);
	void onRejoinChannelSuccess(const char* channel, uid_t userId, int elapsed);
	void onLeaveChannel(const RtcStats& stats);
	void onUserJoined(uid_t userId, int elapsed);
	void onUserOffline(uid_t userId, USER_OFFLINE_REASON_TYPE reason);
	void onUserMuteAudio(uid_t userId, bool muted);
	void onMicrophoneEnabled(bool enabled);
	void onAudioVolumeIndication(const AudioVolumeInfo* speakers, unsigned int speakerNumber, int totalVolume);
};

void agora_initAgora();
