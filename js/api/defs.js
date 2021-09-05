"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.NERtcVoiceEqualizationBand = exports.NERtcVoiceBeautifierType = exports.NERtcVoiceChangerType = exports.NERtcMediaStatsEventName = exports.NERtcLiveStreamStateCode = exports.NERtcAudioMixingErrorCode = exports.NERtcAudioMixingState = exports.NERtcAudioDeviceState = exports.NERtcAudioDeviceType = exports.NERtcReasonConnectionChangedType = exports.NERtcConnectionStateType = exports.NERtcNetworkQualityType = exports.NERtcVideoStreamType = exports.NERtcAudioStreamType = exports.NERtcStreamChannelType = exports.NERtcClientRole = exports.NERtcVideoDeviceTransportType = exports.NERtcAudioDeviceTransportType = exports.NERtcLiveStreamAudioCodecProfile = exports.NERtcLiveStreamAudioSampleRate = exports.NERtcLiveStreamVideoScaleMode = exports.NERtcLiveStreamMode = exports.NERtcSubStreamContentPrefer = exports.NERtcScreenProfileType = exports.NERtcDegradationPreference = exports.NERtcVideoFramerateType = exports.NERtcVideoCropMode = exports.NERtcAudioScenarioType = exports.NERtcAudioProfileType = exports.NERtcVideoMirrorMode = exports.NERtcVideoScalingMode = exports.NERtcVideoProfileType = exports.NERtcSessionLeaveReason = exports.NERtcErrorCode = exports.NERtcRemoteVideoStreamType = exports.NERtcChannelProfileType = exports.NERtcLogLevel = void 0;
/** 日志级别。 */
var NERtcLogLevel;
(function (NERtcLogLevel) {
    NERtcLogLevel[NERtcLogLevel["kNERtcLogLevelFatal"] = 0] = "kNERtcLogLevelFatal";
    NERtcLogLevel[NERtcLogLevel["kNERtcLogLevelError"] = 1] = "kNERtcLogLevelError";
    NERtcLogLevel[NERtcLogLevel["kNERtcLogLevelWarning"] = 2] = "kNERtcLogLevelWarning";
    NERtcLogLevel[NERtcLogLevel["kNERtcLogLevelInfo"] = 3] = "kNERtcLogLevelInfo";
    NERtcLogLevel[NERtcLogLevel["kNERtcLogLevelDetailInfo"] = 4] = "kNERtcLogLevelDetailInfo";
    NERtcLogLevel[NERtcLogLevel["kNERtcLogLevelVerbos"] = 5] = "kNERtcLogLevelVerbos";
    NERtcLogLevel[NERtcLogLevel["kNERtcLogLevelDebug"] = 6] = "kNERtcLogLevelDebug";
    NERtcLogLevel[NERtcLogLevel["kNERtcLogLevelOff"] = 7] = "kNERtcLogLevelOff";
})(NERtcLogLevel = exports.NERtcLogLevel || (exports.NERtcLogLevel = {}));
/** 场景模式 */
var NERtcChannelProfileType;
(function (NERtcChannelProfileType) {
    NERtcChannelProfileType[NERtcChannelProfileType["kNERtcChannelProfileCommunication"] = 0] = "kNERtcChannelProfileCommunication";
    NERtcChannelProfileType[NERtcChannelProfileType["kNERtcChannelProfileLiveBroadcasting"] = 1] = "kNERtcChannelProfileLiveBroadcasting";
})(NERtcChannelProfileType = exports.NERtcChannelProfileType || (exports.NERtcChannelProfileType = {}));
/** 视频流类型。*/
var NERtcRemoteVideoStreamType;
(function (NERtcRemoteVideoStreamType) {
    NERtcRemoteVideoStreamType[NERtcRemoteVideoStreamType["kNERtcRemoteVideoStreamTypeHigh"] = 0] = "kNERtcRemoteVideoStreamTypeHigh";
    NERtcRemoteVideoStreamType[NERtcRemoteVideoStreamType["kNERtcRemoteVideoStreamTypeLow"] = 1] = "kNERtcRemoteVideoStreamTypeLow";
    NERtcRemoteVideoStreamType[NERtcRemoteVideoStreamType["kNERtcRemoteVideoStreamTypeNone"] = 2] = "kNERtcRemoteVideoStreamTypeNone";
})(NERtcRemoteVideoStreamType = exports.NERtcRemoteVideoStreamType || (exports.NERtcRemoteVideoStreamType = {}));
/** 错误代码。

错误代码意味着 SDK 遇到不可恢复的错误，需要应用程序干预。
*/
var NERtcErrorCode;
(function (NERtcErrorCode) {
    NERtcErrorCode[NERtcErrorCode["kNERtcNoError"] = 0] = "kNERtcNoError";
    //资源分配错误 （正常请求时不会有此类错误返回）
    NERtcErrorCode[NERtcErrorCode["kNERtcErrChannelReservePermissionDenied"] = 403] = "kNERtcErrChannelReservePermissionDenied";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrChannelReserveTimeOut"] = 408] = "kNERtcErrChannelReserveTimeOut";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrChannelReserveErrorParam"] = 414] = "kNERtcErrChannelReserveErrorParam";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrChannelReserveServerFail"] = 500] = "kNERtcErrChannelReserveServerFail";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrChannelReserveMoreThanTwoUser"] = 600] = "kNERtcErrChannelReserveMoreThanTwoUser";
    //livestream task
    NERtcErrorCode[NERtcErrorCode["kNERtcErrLsTaskRequestInvalid"] = 1301] = "kNERtcErrLsTaskRequestInvalid";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrLsTaskIsInvaild"] = 1400] = "kNERtcErrLsTaskIsInvaild";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrLsTaskRoomExited"] = 1401] = "kNERtcErrLsTaskRoomExited";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrLsTaskNumLimit"] = 1402] = "kNERtcErrLsTaskNumLimit";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrLsTaskDuplicateId"] = 1403] = "kNERtcErrLsTaskDuplicateId";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrLsTaskNotFound"] = 1404] = "kNERtcErrLsTaskNotFound";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrLsTaskRequestErr"] = 1417] = "kNERtcErrLsTaskRequestErr";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrLsTaskInternalServerErr"] = 1500] = "kNERtcErrLsTaskInternalServerErr";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrLsTaskInvalidLayout"] = 1501] = "kNERtcErrLsTaskInvalidLayout";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrLsTaskUserPicErr"] = 1512] = "kNERtcErrLsTaskUserPicErr";
    //其他错误
    NERtcErrorCode[NERtcErrorCode["kNERtcErrChannelStartFail"] = 11000] = "kNERtcErrChannelStartFail";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrChannelDisconnected"] = 11001] = "kNERtcErrChannelDisconnected";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrVersionSelfLow"] = 11002] = "kNERtcErrVersionSelfLow";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrVersionRemoteLow"] = 11003] = "kNERtcErrVersionRemoteLow";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrChannelClosed"] = 11004] = "kNERtcErrChannelClosed";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrChannelKicked"] = 11005] = "kNERtcErrChannelKicked";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrDataError"] = 11400] = "kNERtcErrDataError";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrInvalid"] = 11403] = "kNERtcErrInvalid";
    //连接服务器错误
    NERtcErrorCode[NERtcErrorCode["kNERtcErrChannelJoinTimeOut"] = 20101] = "kNERtcErrChannelJoinTimeOut";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrChannelJoinMeetingModeError"] = 20102] = "kNERtcErrChannelJoinMeetingModeError";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrChannelJoinRtmpModeError"] = 20103] = "kNERtcErrChannelJoinRtmpModeError";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrChannelJoinRtmpNodesError"] = 20104] = "kNERtcErrChannelJoinRtmpNodesError";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrChannelJoinRtmpHostError"] = 20105] = "kNERtcErrChannelJoinRtmpHostError";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrChannelJoinRtmpCreateError"] = 20106] = "kNERtcErrChannelJoinRtmpCreateError";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrChannelJoinLayoutError"] = 20208] = "kNERtcErrChannelJoinLayoutError";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrChannelJoinInvalidParam"] = 20400] = "kNERtcErrChannelJoinInvalidParam";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrChannelJoinDesKey"] = 20401] = "kNERtcErrChannelJoinDesKey";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrChannelJoinInvalidRequst"] = 20417] = "kNERtcErrChannelJoinInvalidRequst";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrChannelServerUnknown"] = 20500] = "kNERtcErrChannelServerUnknown";
    //Engine error code
    NERtcErrorCode[NERtcErrorCode["kNERtcErrFatal"] = 30001] = "kNERtcErrFatal";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrOutOfMemory"] = 30002] = "kNERtcErrOutOfMemory";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrInvalidParam"] = 30003] = "kNERtcErrInvalidParam";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrNotSupported"] = 30004] = "kNERtcErrNotSupported";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrInvalidState"] = 30005] = "kNERtcErrInvalidState";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrLackOfResource"] = 30006] = "kNERtcErrLackOfResource";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrInvalidIndex"] = 30007] = "kNERtcErrInvalidIndex";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrDeviceNotFound"] = 30008] = "kNERtcErrDeviceNotFound";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrInvalidDeviceSourceID"] = 30009] = "kNERtcErrInvalidDeviceSourceID";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrInvalidVideoProfile"] = 30010] = "kNERtcErrInvalidVideoProfile";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrCreateDeviceSourceFail"] = 30011] = "kNERtcErrCreateDeviceSourceFail";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrInvalidRender"] = 30012] = "kNERtcErrInvalidRender";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrDevicePreviewAlreadyStarted"] = 30013] = "kNERtcErrDevicePreviewAlreadyStarted";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrTransmitPendding"] = 30014] = "kNERtcErrTransmitPendding";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrConnectFail"] = 30015] = "kNERtcErrConnectFail";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrCreateDumpFileFail"] = 30016] = "kNERtcErrCreateDumpFileFail";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrStartDumpFail"] = 30017] = "kNERtcErrStartDumpFail";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrDesktopCaptureInvalidState"] = 30020] = "kNERtcErrDesktopCaptureInvalidState";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrDesktopCaptureInvalidParam"] = 30021] = "kNERtcErrDesktopCaptureInvalidParam";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrDesktopCaptureNotReady"] = 30022] = "kNERtcErrDesktopCaptureNotReady";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrChannelAlreadyJoined"] = 30100] = "kNERtcErrChannelAlreadyJoined";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrChannelNotJoined"] = 30101] = "kNERtcErrChannelNotJoined";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrChannelRepleatedlyLeave"] = 30102] = "kNERtcErrChannelRepleatedlyLeave";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrRequestJoinChannelFail"] = 30103] = "kNERtcErrRequestJoinChannelFail";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrSessionNotFound"] = 30104] = "kNERtcErrSessionNotFound";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrUserNotFound"] = 30105] = "kNERtcErrUserNotFound";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrInvalidUserID"] = 30106] = "kNERtcErrInvalidUserID";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrMediaNotStarted"] = 30107] = "kNERtcErrMediaNotStarted";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrSourceNotFound"] = 30108] = "kNERtcErrSourceNotFound";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrConnectionNotFound"] = 30200] = "kNERtcErrConnectionNotFound";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrStreamNotFound"] = 30201] = "kNERtcErrStreamNotFound";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrAddTrackFail"] = 30202] = "kNERtcErrAddTrackFail";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrTrackNotFound"] = 30203] = "kNERtcErrTrackNotFound";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrMediaConnectionDisconnected"] = 30204] = "kNERtcErrMediaConnectionDisconnected";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrSignalDisconnected"] = 30205] = "kNERtcErrSignalDisconnected";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrServerKicked"] = 30206] = "kNERtcErrServerKicked";
    NERtcErrorCode[NERtcErrorCode["kNERtcErrKickedForRoomClosed"] = 30207] = "kNERtcErrKickedForRoomClosed";
    NERtcErrorCode[NERtcErrorCode["kNERtcRuntimeErrADMNoAuthorize"] = 40000] = "kNERtcRuntimeErrADMNoAuthorize";
    NERtcErrorCode[NERtcErrorCode["kNERtcRuntimeErrVDMNoAuthorize"] = 50000] = "kNERtcRuntimeErrVDMNoAuthorize";
    NERtcErrorCode[NERtcErrorCode["kNERtcRuntimeErrScreenCaptureNoAuthorize"] = 60000] = "kNERtcRuntimeErrScreenCaptureNoAuthorize";
})(NERtcErrorCode = exports.NERtcErrorCode || (exports.NERtcErrorCode = {}));
/** 用户离开原因。*/
var NERtcSessionLeaveReason;
(function (NERtcSessionLeaveReason) {
    NERtcSessionLeaveReason[NERtcSessionLeaveReason["kNERtcSessionLeaveNormal"] = 0] = "kNERtcSessionLeaveNormal";
    NERtcSessionLeaveReason[NERtcSessionLeaveReason["kNERtcSessionLeaveForFailOver"] = 1] = "kNERtcSessionLeaveForFailOver";
    NERtcSessionLeaveReason[NERtcSessionLeaveReason["kNERtcSessionLeaveUpdate"] = 2] = "kNERtcSessionLeaveUpdate";
    NERtcSessionLeaveReason[NERtcSessionLeaveReason["kNERtcSessionLeaveForKick"] = 3] = "kNERtcSessionLeaveForKick";
    NERtcSessionLeaveReason[NERtcSessionLeaveReason["kNERtcSessionLeaveTimeOut"] = 4] = "kNERtcSessionLeaveTimeOut";
})(NERtcSessionLeaveReason = exports.NERtcSessionLeaveReason || (exports.NERtcSessionLeaveReason = {}));
/** 视频编码配置。用于衡量编码质量。*/
var NERtcVideoProfileType;
(function (NERtcVideoProfileType) {
    NERtcVideoProfileType[NERtcVideoProfileType["kNERtcVideoProfileLowest"] = 0] = "kNERtcVideoProfileLowest";
    NERtcVideoProfileType[NERtcVideoProfileType["kNERtcVideoProfileLow"] = 1] = "kNERtcVideoProfileLow";
    NERtcVideoProfileType[NERtcVideoProfileType["kNERtcVideoProfileStandard"] = 2] = "kNERtcVideoProfileStandard";
    NERtcVideoProfileType[NERtcVideoProfileType["kNERtcVideoProfileHD720P"] = 3] = "kNERtcVideoProfileHD720P";
    NERtcVideoProfileType[NERtcVideoProfileType["kNERtcVideoProfileHD1080P"] = 4] = "kNERtcVideoProfileHD1080P";
    NERtcVideoProfileType[NERtcVideoProfileType["kNERtcVideoProfileNone"] = 5] = "kNERtcVideoProfileNone";
    NERtcVideoProfileType[NERtcVideoProfileType["kNERtcVideoProfileMAX"] = 4] = "kNERtcVideoProfileMAX";
    NERtcVideoProfileType[NERtcVideoProfileType["kNERtcVideoProfileFake"] = 6] = "kNERtcVideoProfileFake";
})(NERtcVideoProfileType = exports.NERtcVideoProfileType || (exports.NERtcVideoProfileType = {}));
var NERtcVideoScalingMode;
(function (NERtcVideoScalingMode) {
    NERtcVideoScalingMode[NERtcVideoScalingMode["kNERtcVideoScaleFit"] = 0] = "kNERtcVideoScaleFit";
    NERtcVideoScalingMode[NERtcVideoScalingMode["kNERtcVideoScaleFullFill"] = 1] = "kNERtcVideoScaleFullFill";
    NERtcVideoScalingMode[NERtcVideoScalingMode["kNERtcVideoScaleCropFill"] = 2] = "kNERtcVideoScaleCropFill";
})(NERtcVideoScalingMode = exports.NERtcVideoScalingMode || (exports.NERtcVideoScalingMode = {}));
/** @enum NERtcVideoMirrorMode 设置镜像模式。*/
var NERtcVideoMirrorMode;
(function (NERtcVideoMirrorMode) {
    NERtcVideoMirrorMode[NERtcVideoMirrorMode["kNERtcVideoMirrorModeAuto"] = 0] = "kNERtcVideoMirrorModeAuto";
    NERtcVideoMirrorMode[NERtcVideoMirrorMode["kNERtcVideoMirrorModeEnabled"] = 1] = "kNERtcVideoMirrorModeEnabled";
    NERtcVideoMirrorMode[NERtcVideoMirrorMode["kNERtcVideoMirrorModeDisabled"] = 2] = "kNERtcVideoMirrorModeDisabled";
})(NERtcVideoMirrorMode = exports.NERtcVideoMirrorMode || (exports.NERtcVideoMirrorMode = {}));
var NERtcAudioProfileType;
(function (NERtcAudioProfileType) {
    NERtcAudioProfileType[NERtcAudioProfileType["kNERtcAudioProfileDefault"] = 0] = "kNERtcAudioProfileDefault";
    NERtcAudioProfileType[NERtcAudioProfileType["kNERtcAudioProfileStandard"] = 1] = "kNERtcAudioProfileStandard";
    NERtcAudioProfileType[NERtcAudioProfileType["kNERtcAudioProfileStandardExtend"] = 2] = "kNERtcAudioProfileStandardExtend";
    NERtcAudioProfileType[NERtcAudioProfileType["kNERtcAudioProfileMiddleQuality"] = 3] = "kNERtcAudioProfileMiddleQuality";
    NERtcAudioProfileType[NERtcAudioProfileType["kNERtcAudioProfileMiddleQualityStereo"] = 4] = "kNERtcAudioProfileMiddleQualityStereo";
    NERtcAudioProfileType[NERtcAudioProfileType["kNERtcAudioProfileHighQuality"] = 5] = "kNERtcAudioProfileHighQuality";
    NERtcAudioProfileType[NERtcAudioProfileType["kNERtcAudioProfileHighQualityStereo"] = 6] = "kNERtcAudioProfileHighQualityStereo";
})(NERtcAudioProfileType = exports.NERtcAudioProfileType || (exports.NERtcAudioProfileType = {}));
var NERtcAudioScenarioType;
(function (NERtcAudioScenarioType) {
    NERtcAudioScenarioType[NERtcAudioScenarioType["kNERtcAudioScenarioDefault"] = 0] = "kNERtcAudioScenarioDefault";
    NERtcAudioScenarioType[NERtcAudioScenarioType["kNERtcAudioScenarioSpeech"] = 1] = "kNERtcAudioScenarioSpeech";
    NERtcAudioScenarioType[NERtcAudioScenarioType["kNERtcAudioScenarioMusic"] = 2] = "kNERtcAudioScenarioMusic";
})(NERtcAudioScenarioType = exports.NERtcAudioScenarioType || (exports.NERtcAudioScenarioType = {}));
/** @enum NERtcVideoCropMode 视频画面裁剪模式。*/
var NERtcVideoCropMode;
(function (NERtcVideoCropMode) {
    NERtcVideoCropMode[NERtcVideoCropMode["kNERtcVideoCropModeDefault"] = 0] = "kNERtcVideoCropModeDefault";
    NERtcVideoCropMode[NERtcVideoCropMode["kNERtcVideoCropMode16x9"] = 1] = "kNERtcVideoCropMode16x9";
    NERtcVideoCropMode[NERtcVideoCropMode["kNERtcVideoCropMode4x3"] = 2] = "kNERtcVideoCropMode4x3";
    NERtcVideoCropMode[NERtcVideoCropMode["kNERtcVideoCropMode1x1"] = 3] = "kNERtcVideoCropMode1x1";
})(NERtcVideoCropMode = exports.NERtcVideoCropMode || (exports.NERtcVideoCropMode = {}));
/** @enum NERtcVideoFramerateType 视频帧率。*/
var NERtcVideoFramerateType;
(function (NERtcVideoFramerateType) {
    NERtcVideoFramerateType[NERtcVideoFramerateType["kNERtcVideoFramerateFpsDefault"] = 0] = "kNERtcVideoFramerateFpsDefault";
    NERtcVideoFramerateType[NERtcVideoFramerateType["kNERtcVideoFramerateFps_7"] = 7] = "kNERtcVideoFramerateFps_7";
    NERtcVideoFramerateType[NERtcVideoFramerateType["kNERtcVideoFramerateFps_10"] = 10] = "kNERtcVideoFramerateFps_10";
    NERtcVideoFramerateType[NERtcVideoFramerateType["kNERtcVideoFramerateFps_15"] = 15] = "kNERtcVideoFramerateFps_15";
    NERtcVideoFramerateType[NERtcVideoFramerateType["kNERtcVideoFramerateFps_24"] = 24] = "kNERtcVideoFramerateFps_24";
    NERtcVideoFramerateType[NERtcVideoFramerateType["kNERtcVideoFramerateFps_30"] = 30] = "kNERtcVideoFramerateFps_30";
    NERtcVideoFramerateType[NERtcVideoFramerateType["kNERtcVideoFramerateFps_60"] = 60] = "kNERtcVideoFramerateFps_60";
})(NERtcVideoFramerateType = exports.NERtcVideoFramerateType || (exports.NERtcVideoFramerateType = {}));
/** @enum NERtcDegradationPreference 视频编码策略。*/
var NERtcDegradationPreference;
(function (NERtcDegradationPreference) {
    NERtcDegradationPreference[NERtcDegradationPreference["kNERtcDegradationDefault"] = 0] = "kNERtcDegradationDefault";
    NERtcDegradationPreference[NERtcDegradationPreference["kNERtcDegradationMaintainFramerate"] = 1] = "kNERtcDegradationMaintainFramerate";
    NERtcDegradationPreference[NERtcDegradationPreference["kNERtcDegradationMaintainQuality"] = 2] = "kNERtcDegradationMaintainQuality";
    NERtcDegradationPreference[NERtcDegradationPreference["kNERtcDegradationBalanced"] = 3] = "kNERtcDegradationBalanced";
})(NERtcDegradationPreference = exports.NERtcDegradationPreference || (exports.NERtcDegradationPreference = {}));
/** 屏幕共享编码参数配置。*/
var NERtcScreenProfileType;
(function (NERtcScreenProfileType) {
    NERtcScreenProfileType[NERtcScreenProfileType["kNERtcScreenProfile480P"] = 0] = "kNERtcScreenProfile480P";
    NERtcScreenProfileType[NERtcScreenProfileType["kNERtcScreenProfileHD720P"] = 1] = "kNERtcScreenProfileHD720P";
    NERtcScreenProfileType[NERtcScreenProfileType["kNERtcScreenProfileHD1080P"] = 2] = "kNERtcScreenProfileHD1080P";
    NERtcScreenProfileType[NERtcScreenProfileType["kNERtcScreenProfileCustom"] = 3] = "kNERtcScreenProfileCustom";
    NERtcScreenProfileType[NERtcScreenProfileType["kNERtcScreenProfileNone"] = 4] = "kNERtcScreenProfileNone";
    NERtcScreenProfileType[NERtcScreenProfileType["kNERtcScreenProfileMAX"] = 2] = "kNERtcScreenProfileMAX";
})(NERtcScreenProfileType = exports.NERtcScreenProfileType || (exports.NERtcScreenProfileType = {}));
/** 屏幕共享功能的编码策略倾向
- kNERtcSubStreamContentPreferMotion: 内容类型为动画;当共享的内容是视频、电影或游戏时，推荐选择该内容类型
当用户设置内容类型为动画时，按用户设置的帧率处理
- kNERtcSubStreamContentPreferDetails: 内容类型为细节;当共享的内容是图片或文字时，推荐选择该内容类型
当用户设置内容类型为细节时，最高允许用户设置到10帧，设置超过10帧时，不生效，按10帧处理
 */
var NERtcSubStreamContentPrefer;
(function (NERtcSubStreamContentPrefer) {
    NERtcSubStreamContentPrefer[NERtcSubStreamContentPrefer["kNERtcSubStreamContentPreferMotion"] = 0] = "kNERtcSubStreamContentPreferMotion";
    NERtcSubStreamContentPrefer[NERtcSubStreamContentPrefer["kNERtcSubStreamContentPreferDetails"] = 1] = "kNERtcSubStreamContentPreferDetails";
})(NERtcSubStreamContentPrefer = exports.NERtcSubStreamContentPrefer || (exports.NERtcSubStreamContentPrefer = {}));
/** 直播推流模式 */
var NERtcLiveStreamMode;
(function (NERtcLiveStreamMode) {
    NERtcLiveStreamMode[NERtcLiveStreamMode["kNERtcLsModeVideo"] = 0] = "kNERtcLsModeVideo";
    NERtcLiveStreamMode[NERtcLiveStreamMode["kNERtcLsModeAudio"] = 1] = "kNERtcLsModeAudio";
})(NERtcLiveStreamMode = exports.NERtcLiveStreamMode || (exports.NERtcLiveStreamMode = {}));
/** 直播推流视频裁剪模式 */
var NERtcLiveStreamVideoScaleMode;
(function (NERtcLiveStreamVideoScaleMode) {
    NERtcLiveStreamVideoScaleMode[NERtcLiveStreamVideoScaleMode["kNERtcLsModeVideoScaleFit"] = 0] = "kNERtcLsModeVideoScaleFit";
    NERtcLiveStreamVideoScaleMode[NERtcLiveStreamVideoScaleMode["kNERtcLsModeVideoScaleCropFill"] = 1] = "kNERtcLsModeVideoScaleCropFill";
})(NERtcLiveStreamVideoScaleMode = exports.NERtcLiveStreamVideoScaleMode || (exports.NERtcLiveStreamVideoScaleMode = {}));
/** 直播推流音频采样率 */
var NERtcLiveStreamAudioSampleRate;
(function (NERtcLiveStreamAudioSampleRate) {
    NERtcLiveStreamAudioSampleRate[NERtcLiveStreamAudioSampleRate["kNERtcLiveStreamAudioSampleRate32000"] = 32000] = "kNERtcLiveStreamAudioSampleRate32000";
    NERtcLiveStreamAudioSampleRate[NERtcLiveStreamAudioSampleRate["kNERtcLiveStreamAudioSampleRate44100"] = 44100] = "kNERtcLiveStreamAudioSampleRate44100";
    NERtcLiveStreamAudioSampleRate[NERtcLiveStreamAudioSampleRate["kNERtcLiveStreamAudioSampleRate48000"] = 48000] = "kNERtcLiveStreamAudioSampleRate48000"; /**< （默认）采样率为 48 kHz。*/
})(NERtcLiveStreamAudioSampleRate = exports.NERtcLiveStreamAudioSampleRate || (exports.NERtcLiveStreamAudioSampleRate = {}));
/** 直播推流音频编码规格 */
var NERtcLiveStreamAudioCodecProfile;
(function (NERtcLiveStreamAudioCodecProfile) {
    NERtcLiveStreamAudioCodecProfile[NERtcLiveStreamAudioCodecProfile["kNERtcLiveStreamAudioCodecProfileLCAAC"] = 0] = "kNERtcLiveStreamAudioCodecProfileLCAAC";
    NERtcLiveStreamAudioCodecProfile[NERtcLiveStreamAudioCodecProfile["kNERtcLiveStreamAudioCodecProfileHEAAC"] = 1] = "kNERtcLiveStreamAudioCodecProfileHEAAC"; /**< HE-AAC 规格，表示高效音频编码规格。*/
})(NERtcLiveStreamAudioCodecProfile = exports.NERtcLiveStreamAudioCodecProfile || (exports.NERtcLiveStreamAudioCodecProfile = {}));
/** 音频设备链接类型。*/
var NERtcAudioDeviceTransportType;
(function (NERtcAudioDeviceTransportType) {
    NERtcAudioDeviceTransportType[NERtcAudioDeviceTransportType["kNERtcAudioDeviceTransportTypeUnknown"] = 0] = "kNERtcAudioDeviceTransportTypeUnknown";
    NERtcAudioDeviceTransportType[NERtcAudioDeviceTransportType["kNERtcAudioDeviceTransportTypeBluetooth"] = 1] = "kNERtcAudioDeviceTransportTypeBluetooth";
    NERtcAudioDeviceTransportType[NERtcAudioDeviceTransportType["kNERtcAudioDeviceTransportTypeBluetoothA2DP"] = 2] = "kNERtcAudioDeviceTransportTypeBluetoothA2DP";
    NERtcAudioDeviceTransportType[NERtcAudioDeviceTransportType["kNERtcAudioDeviceTransportTypeBluetoothLE"] = 3] = "kNERtcAudioDeviceTransportTypeBluetoothLE";
    NERtcAudioDeviceTransportType[NERtcAudioDeviceTransportType["kNERtcAudioDeviceTransportTypeUSB"] = 4] = "kNERtcAudioDeviceTransportTypeUSB";
    NERtcAudioDeviceTransportType[NERtcAudioDeviceTransportType["kNERtcAudioDeviceTransportTypeHDMI"] = 5] = "kNERtcAudioDeviceTransportTypeHDMI";
    NERtcAudioDeviceTransportType[NERtcAudioDeviceTransportType["kNERtcAudioDeviceTransportTypeBuiltIn"] = 6] = "kNERtcAudioDeviceTransportTypeBuiltIn";
    NERtcAudioDeviceTransportType[NERtcAudioDeviceTransportType["kNERtcAudioDeviceTransportTypeThunderbolt"] = 7] = "kNERtcAudioDeviceTransportTypeThunderbolt";
    NERtcAudioDeviceTransportType[NERtcAudioDeviceTransportType["kNERtcAudioDeviceTransportTypeAirPlay"] = 8] = "kNERtcAudioDeviceTransportTypeAirPlay";
    NERtcAudioDeviceTransportType[NERtcAudioDeviceTransportType["kNERtcAudioDeviceTransportTypeVirtual"] = 9] = "kNERtcAudioDeviceTransportTypeVirtual";
    NERtcAudioDeviceTransportType[NERtcAudioDeviceTransportType["kNERtcAudioDeviceTransportTypeOther"] = 10] = "kNERtcAudioDeviceTransportTypeOther";
})(NERtcAudioDeviceTransportType = exports.NERtcAudioDeviceTransportType || (exports.NERtcAudioDeviceTransportType = {}));
/** 摄像头设备链接类型。*/
var NERtcVideoDeviceTransportType;
(function (NERtcVideoDeviceTransportType) {
    NERtcVideoDeviceTransportType[NERtcVideoDeviceTransportType["kNERtcVideoDeviceTransportTypeUnknown"] = 0] = "kNERtcVideoDeviceTransportTypeUnknown";
    NERtcVideoDeviceTransportType[NERtcVideoDeviceTransportType["kNERtcVideoDeviceTransportTypeUSB"] = 1] = "kNERtcVideoDeviceTransportTypeUSB";
    NERtcVideoDeviceTransportType[NERtcVideoDeviceTransportType["kNERtcVideoDeviceTransportTypeVirtual"] = 2] = "kNERtcVideoDeviceTransportTypeVirtual";
    NERtcVideoDeviceTransportType[NERtcVideoDeviceTransportType["kNERtcVideoDeviceTransportTypeOther"] = 3] = "kNERtcVideoDeviceTransportTypeOther";
})(NERtcVideoDeviceTransportType = exports.NERtcVideoDeviceTransportType || (exports.NERtcVideoDeviceTransportType = {}));
/** 参会者角色类型 */
var NERtcClientRole;
(function (NERtcClientRole) {
    NERtcClientRole[NERtcClientRole["kNERtcClientRoleBroadcaster"] = 0] = "kNERtcClientRoleBroadcaster";
    NERtcClientRole[NERtcClientRole["kNERtcClientRoleAudience"] = 1] = "kNERtcClientRoleAudience";
})(NERtcClientRole = exports.NERtcClientRole || (exports.NERtcClientRole = {}));
/** SEI发送的流通道类型 */
var NERtcStreamChannelType;
(function (NERtcStreamChannelType) {
    NERtcStreamChannelType[NERtcStreamChannelType["kNERtcStreamChannelTypeMainStream"] = 0] = "kNERtcStreamChannelTypeMainStream";
    NERtcStreamChannelType[NERtcStreamChannelType["kNERtcStreamChannelTypeSubStream"] = 1] = "kNERtcStreamChannelTypeSubStream";
})(NERtcStreamChannelType = exports.NERtcStreamChannelType || (exports.NERtcStreamChannelType = {}));
/** 音频流类型，目前同时支持音频两路流：主流和辅流 */
var NERtcAudioStreamType;
(function (NERtcAudioStreamType) {
    NERtcAudioStreamType[NERtcAudioStreamType["kNERtcAudioStreamMain"] = 0] = "kNERtcAudioStreamMain";
    NERtcAudioStreamType[NERtcAudioStreamType["kNERtcAudioStreamSub"] = 1] = "kNERtcAudioStreamSub";
})(NERtcAudioStreamType = exports.NERtcAudioStreamType || (exports.NERtcAudioStreamType = {}));
var NERtcVideoStreamType;
(function (NERtcVideoStreamType) {
    NERtcVideoStreamType[NERtcVideoStreamType["kNERtcVideoStreamMain"] = 0] = "kNERtcVideoStreamMain";
    NERtcVideoStreamType[NERtcVideoStreamType["kNERtcVideoStreamSub"] = 1] = "kNERtcVideoStreamSub"; /** 视频流副流 */
})(NERtcVideoStreamType = exports.NERtcVideoStreamType || (exports.NERtcVideoStreamType = {}));
/** @enum NERtcNetworkQualityType 网络质量类型。*/
var NERtcNetworkQualityType;
(function (NERtcNetworkQualityType) {
    NERtcNetworkQualityType[NERtcNetworkQualityType["kNERtcNetworkQualityUnknown"] = 0] = "kNERtcNetworkQualityUnknown";
    NERtcNetworkQualityType[NERtcNetworkQualityType["kNERtcNetworkQualityExcellent"] = 1] = "kNERtcNetworkQualityExcellent";
    NERtcNetworkQualityType[NERtcNetworkQualityType["kNERtcNetworkQualityGood"] = 2] = "kNERtcNetworkQualityGood";
    NERtcNetworkQualityType[NERtcNetworkQualityType["kNERtcNetworkQualityPoor"] = 3] = "kNERtcNetworkQualityPoor";
    NERtcNetworkQualityType[NERtcNetworkQualityType["kNERtcNetworkQualityBad"] = 4] = "kNERtcNetworkQualityBad";
    NERtcNetworkQualityType[NERtcNetworkQualityType["kNERtcNetworkQualityVeryBad"] = 5] = "kNERtcNetworkQualityVeryBad";
    NERtcNetworkQualityType[NERtcNetworkQualityType["kNERtcNetworkQualityDown"] = 6] = "kNERtcNetworkQualityDown";
})(NERtcNetworkQualityType = exports.NERtcNetworkQualityType || (exports.NERtcNetworkQualityType = {}));
/** 连接状态 */
var NERtcConnectionStateType;
(function (NERtcConnectionStateType) {
    NERtcConnectionStateType[NERtcConnectionStateType["kNERtcConnectionStateDisconnected"] = 1] = "kNERtcConnectionStateDisconnected";
    NERtcConnectionStateType[NERtcConnectionStateType["kNERtcConnectionStateConnecting"] = 2] = "kNERtcConnectionStateConnecting";
    NERtcConnectionStateType[NERtcConnectionStateType["kNERtcConnectionStateConnected"] = 3] = "kNERtcConnectionStateConnected";
    NERtcConnectionStateType[NERtcConnectionStateType["kNERtcConnectionStateReconnecting"] = 4] = "kNERtcConnectionStateReconnecting";
    NERtcConnectionStateType[NERtcConnectionStateType["kNERtcConnectionStateFailed"] = 5] = "kNERtcConnectionStateFailed";
})(NERtcConnectionStateType = exports.NERtcConnectionStateType || (exports.NERtcConnectionStateType = {}));
/** 连接状态变更原因 */
var NERtcReasonConnectionChangedType;
(function (NERtcReasonConnectionChangedType) {
    NERtcReasonConnectionChangedType[NERtcReasonConnectionChangedType["kNERtcReasonConnectionChangedLeaveChannel"] = 1] = "kNERtcReasonConnectionChangedLeaveChannel";
    NERtcReasonConnectionChangedType[NERtcReasonConnectionChangedType["kNERtcReasonConnectionChangedChannelClosed"] = 2] = "kNERtcReasonConnectionChangedChannelClosed";
    NERtcReasonConnectionChangedType[NERtcReasonConnectionChangedType["kNERtcReasonConnectionChangedBeKicked"] = 3] = "kNERtcReasonConnectionChangedBeKicked";
    NERtcReasonConnectionChangedType[NERtcReasonConnectionChangedType["kNERtcReasonConnectionChangedTimeOut"] = 4] = "kNERtcReasonConnectionChangedTimeOut";
    NERtcReasonConnectionChangedType[NERtcReasonConnectionChangedType["kNERtcReasonConnectionChangedJoinChannel"] = 5] = "kNERtcReasonConnectionChangedJoinChannel";
    NERtcReasonConnectionChangedType[NERtcReasonConnectionChangedType["kNERtcReasonConnectionChangedJoinSucceed"] = 6] = "kNERtcReasonConnectionChangedJoinSucceed";
    NERtcReasonConnectionChangedType[NERtcReasonConnectionChangedType["kNERtcReasonConnectionChangedReJoinSucceed"] = 7] = "kNERtcReasonConnectionChangedReJoinSucceed";
    NERtcReasonConnectionChangedType[NERtcReasonConnectionChangedType["kNERtcReasonConnectionChangedMediaConnectionDisconnected"] = 8] = "kNERtcReasonConnectionChangedMediaConnectionDisconnected";
    NERtcReasonConnectionChangedType[NERtcReasonConnectionChangedType["kNERtcReasonConnectionChangedSignalDisconnected"] = 9] = "kNERtcReasonConnectionChangedSignalDisconnected";
    NERtcReasonConnectionChangedType[NERtcReasonConnectionChangedType["kNERtcReasonConnectionChangedRequestChannelFailed"] = 10] = "kNERtcReasonConnectionChangedRequestChannelFailed";
    NERtcReasonConnectionChangedType[NERtcReasonConnectionChangedType["kNERtcReasonConnectionChangedJoinChannelFailed"] = 11] = "kNERtcReasonConnectionChangedJoinChannelFailed";
})(NERtcReasonConnectionChangedType = exports.NERtcReasonConnectionChangedType || (exports.NERtcReasonConnectionChangedType = {}));
/** 音频设备类型。*/
var NERtcAudioDeviceType;
(function (NERtcAudioDeviceType) {
    NERtcAudioDeviceType[NERtcAudioDeviceType["kNERtcAudioDeviceUnknown"] = 0] = "kNERtcAudioDeviceUnknown";
    NERtcAudioDeviceType[NERtcAudioDeviceType["kNERtcAudioDeviceRecord"] = 1] = "kNERtcAudioDeviceRecord";
    NERtcAudioDeviceType[NERtcAudioDeviceType["kNERtcAudioDevicePlayout"] = 2] = "kNERtcAudioDevicePlayout";
})(NERtcAudioDeviceType = exports.NERtcAudioDeviceType || (exports.NERtcAudioDeviceType = {}));
/** 音频设备类型状态。*/
var NERtcAudioDeviceState;
(function (NERtcAudioDeviceState) {
    NERtcAudioDeviceState[NERtcAudioDeviceState["kNERtcAudioDeviceActive"] = 0] = "kNERtcAudioDeviceActive";
    NERtcAudioDeviceState[NERtcAudioDeviceState["kNERtcAudioDeviceUnactive"] = 1] = "kNERtcAudioDeviceUnactive";
})(NERtcAudioDeviceState = exports.NERtcAudioDeviceState || (exports.NERtcAudioDeviceState = {}));
/** 音乐文件播放状态。
*/
var NERtcAudioMixingState;
(function (NERtcAudioMixingState) {
    NERtcAudioMixingState[NERtcAudioMixingState["kNERtcAudioMixingStateFinished"] = 0] = "kNERtcAudioMixingStateFinished";
    NERtcAudioMixingState[NERtcAudioMixingState["kNERtcAudioMixingStateFailed"] = 1] = "kNERtcAudioMixingStateFailed";
})(NERtcAudioMixingState = exports.NERtcAudioMixingState || (exports.NERtcAudioMixingState = {}));
/** @enum NERtcAudioMixingErrorCode 混音音乐文件错误码。
*/
var NERtcAudioMixingErrorCode;
(function (NERtcAudioMixingErrorCode) {
    NERtcAudioMixingErrorCode[NERtcAudioMixingErrorCode["kNERtcAudioMixingErrorOK"] = 0] = "kNERtcAudioMixingErrorOK";
    NERtcAudioMixingErrorCode[NERtcAudioMixingErrorCode["kNERtcAudioMixingErrorFatal"] = 1] = "kNERtcAudioMixingErrorFatal";
    NERtcAudioMixingErrorCode[NERtcAudioMixingErrorCode["kNERtcAudioMixingErrorCanNotOpen"] = 2] = "kNERtcAudioMixingErrorCanNotOpen";
    NERtcAudioMixingErrorCode[NERtcAudioMixingErrorCode["kNERtcAudioMixingErrorDecode"] = 3] = "kNERtcAudioMixingErrorDecode";
    NERtcAudioMixingErrorCode[NERtcAudioMixingErrorCode["kNERtcAudioMixingErrorInterrupt"] = 4] = "kNERtcAudioMixingErrorInterrupt";
    NERtcAudioMixingErrorCode[NERtcAudioMixingErrorCode["kNERtcAudioMixingErrorHttpNotFound"] = 5] = "kNERtcAudioMixingErrorHttpNotFound";
    NERtcAudioMixingErrorCode[NERtcAudioMixingErrorCode["kNERtcAudioMixingErrorOpen"] = 6] = "kNERtcAudioMixingErrorOpen";
    NERtcAudioMixingErrorCode[NERtcAudioMixingErrorCode["kNERtcAudioMixingErrorNInfo"] = 7] = "kNERtcAudioMixingErrorNInfo";
    NERtcAudioMixingErrorCode[NERtcAudioMixingErrorCode["kNERtcAudioMixingErrorNStream"] = 8] = "kNERtcAudioMixingErrorNStream";
    NERtcAudioMixingErrorCode[NERtcAudioMixingErrorCode["kNERtcAudioMixingErrorNCodec"] = 9] = "kNERtcAudioMixingErrorNCodec";
    NERtcAudioMixingErrorCode[NERtcAudioMixingErrorCode["kNERtcAudioMixingErrorNMem"] = 10] = "kNERtcAudioMixingErrorNMem";
    NERtcAudioMixingErrorCode[NERtcAudioMixingErrorCode["kNERtcAudioMixingErrorCodecOpen"] = 11] = "kNERtcAudioMixingErrorCodecOpen";
    NERtcAudioMixingErrorCode[NERtcAudioMixingErrorCode["kNERtcAudioMixingErrorInvalidInfo"] = 12] = "kNERtcAudioMixingErrorInvalidInfo";
    NERtcAudioMixingErrorCode[NERtcAudioMixingErrorCode["kNERtcAudioMixingErrorOpenTimeout"] = 13] = "kNERtcAudioMixingErrorOpenTimeout";
    NERtcAudioMixingErrorCode[NERtcAudioMixingErrorCode["kNERtcAudioMixingErrorIoTimeout"] = 14] = "kNERtcAudioMixingErrorIoTimeout";
    NERtcAudioMixingErrorCode[NERtcAudioMixingErrorCode["kNERtcAudioMixingErrorIo"] = 15] = "kNERtcAudioMixingErrorIo";
    //kNERtcAudioMixingErrorTooFrequentCall = 101,  /**< 音乐文件打开太频繁。*/
    //kNERtcAudioMixingErrorInterruptedEOF= 102,    /**< 音乐文件播放中断。*/
})(NERtcAudioMixingErrorCode = exports.NERtcAudioMixingErrorCode || (exports.NERtcAudioMixingErrorCode = {}));
/** 直播推流状态。*/
var NERtcLiveStreamStateCode;
(function (NERtcLiveStreamStateCode) {
    NERtcLiveStreamStateCode[NERtcLiveStreamStateCode["kNERtcLsStatePushing"] = 505] = "kNERtcLsStatePushing";
    NERtcLiveStreamStateCode[NERtcLiveStreamStateCode["kNERtcLsStatePushFail"] = 506] = "kNERtcLsStatePushFail";
    NERtcLiveStreamStateCode[NERtcLiveStreamStateCode["kNERtcLsStatePushStopped"] = 511] = "kNERtcLsStatePushStopped";
    NERtcLiveStreamStateCode[NERtcLiveStreamStateCode["kNERtcLsStateImageError"] = 512] = "kNERtcLsStateImageError";
})(NERtcLiveStreamStateCode = exports.NERtcLiveStreamStateCode || (exports.NERtcLiveStreamStateCode = {}));
/** @enum NERtcMediaStatsEventName  SDK 向 App 上报统计信息 */
var NERtcMediaStatsEventName;
(function (NERtcMediaStatsEventName) {
    NERtcMediaStatsEventName["RtcStats"] = "onRtcStats";
    NERtcMediaStatsEventName["LocalAudioStats"] = "onLocalAudioStats";
    NERtcMediaStatsEventName["RemoteAudioStats"] = "onRemoteAudioStats";
    NERtcMediaStatsEventName["LocalVideoStats"] = "onLocalVideoStats";
    NERtcMediaStatsEventName["RemoteVideoStats"] = "onRemoteVideoStats";
    NERtcMediaStatsEventName["NetworkQuality"] = "onNetworkQuality";
})(NERtcMediaStatsEventName = exports.NERtcMediaStatsEventName || (exports.NERtcMediaStatsEventName = {}));
/** 4.1.1 */
/** 变声 预设值 */
var NERtcVoiceChangerType;
(function (NERtcVoiceChangerType) {
    NERtcVoiceChangerType[NERtcVoiceChangerType["kNERtcVoiceChangerOff"] = 0] = "kNERtcVoiceChangerOff";
    NERtcVoiceChangerType[NERtcVoiceChangerType["kNERtcVoiceChangerRobot"] = 1] = "kNERtcVoiceChangerRobot";
    NERtcVoiceChangerType[NERtcVoiceChangerType["kNERtcVoiceChangerGaint"] = 2] = "kNERtcVoiceChangerGaint";
    NERtcVoiceChangerType[NERtcVoiceChangerType["kNERtcVoiceChangerHorror"] = 3] = "kNERtcVoiceChangerHorror";
    NERtcVoiceChangerType[NERtcVoiceChangerType["kNERtcVoiceChangerMature"] = 4] = "kNERtcVoiceChangerMature";
    NERtcVoiceChangerType[NERtcVoiceChangerType["kNERtcVoiceChangerManToWoman"] = 5] = "kNERtcVoiceChangerManToWoman";
    NERtcVoiceChangerType[NERtcVoiceChangerType["kNERtcVoiceChangerWomanToMan"] = 6] = "kNERtcVoiceChangerWomanToMan";
    NERtcVoiceChangerType[NERtcVoiceChangerType["kNERtcVoiceChangerManToLoli"] = 7] = "kNERtcVoiceChangerManToLoli";
    NERtcVoiceChangerType[NERtcVoiceChangerType["kNERtcVoiceChangerWomanToLoli"] = 8] = "kNERtcVoiceChangerWomanToLoli"; /**< 女变萝莉 */
})(NERtcVoiceChangerType = exports.NERtcVoiceChangerType || (exports.NERtcVoiceChangerType = {}));
/** 预设的美声效果 */
var NERtcVoiceBeautifierType;
(function (NERtcVoiceBeautifierType) {
    NERtcVoiceBeautifierType[NERtcVoiceBeautifierType["kNERtcVoiceBeautifierOff"] = 0] = "kNERtcVoiceBeautifierOff";
    NERtcVoiceBeautifierType[NERtcVoiceBeautifierType["kNERtcVoiceBeautifierMuffled"] = 1] = "kNERtcVoiceBeautifierMuffled";
    NERtcVoiceBeautifierType[NERtcVoiceBeautifierType["kNERtcVoiceBeautifierMellow"] = 2] = "kNERtcVoiceBeautifierMellow";
    NERtcVoiceBeautifierType[NERtcVoiceBeautifierType["kNERtcVoiceBeautifierClear"] = 3] = "kNERtcVoiceBeautifierClear";
    NERtcVoiceBeautifierType[NERtcVoiceBeautifierType["kNERtcVoiceBeautifierMagnetic"] = 4] = "kNERtcVoiceBeautifierMagnetic";
    NERtcVoiceBeautifierType[NERtcVoiceBeautifierType["kNERtcVoiceBeautifierRecordingstudio"] = 5] = "kNERtcVoiceBeautifierRecordingstudio";
    NERtcVoiceBeautifierType[NERtcVoiceBeautifierType["kNERtcVoiceBeautifierNature"] = 6] = "kNERtcVoiceBeautifierNature";
    NERtcVoiceBeautifierType[NERtcVoiceBeautifierType["kNERtcVoiceBeautifierKTV"] = 7] = "kNERtcVoiceBeautifierKTV";
    NERtcVoiceBeautifierType[NERtcVoiceBeautifierType["kNERtcVoiceBeautifierRemote"] = 8] = "kNERtcVoiceBeautifierRemote";
    NERtcVoiceBeautifierType[NERtcVoiceBeautifierType["kNERtcVoiceBeautifierChurch"] = 9] = "kNERtcVoiceBeautifierChurch";
    NERtcVoiceBeautifierType[NERtcVoiceBeautifierType["kNERtcVoiceBeautifierBedroom"] = 10] = "kNERtcVoiceBeautifierBedroom";
    NERtcVoiceBeautifierType[NERtcVoiceBeautifierType["kNERtcVoiceBeautifierLive"] = 11] = "kNERtcVoiceBeautifierLive";
})(NERtcVoiceBeautifierType = exports.NERtcVoiceBeautifierType || (exports.NERtcVoiceBeautifierType = {}));
/** 音效均衡波段的中心频率 */
var NERtcVoiceEqualizationBand;
(function (NERtcVoiceEqualizationBand) {
    NERtcVoiceEqualizationBand[NERtcVoiceEqualizationBand["kNERtcVoiceEqualizationBand_31"] = 0] = "kNERtcVoiceEqualizationBand_31";
    NERtcVoiceEqualizationBand[NERtcVoiceEqualizationBand["kNERtcVoiceEqualizationBand_62"] = 1] = "kNERtcVoiceEqualizationBand_62";
    NERtcVoiceEqualizationBand[NERtcVoiceEqualizationBand["kNERtcVoiceEqualizationBand_125"] = 2] = "kNERtcVoiceEqualizationBand_125";
    NERtcVoiceEqualizationBand[NERtcVoiceEqualizationBand["kNERtcVoiceEqualizationBand_250"] = 3] = "kNERtcVoiceEqualizationBand_250";
    NERtcVoiceEqualizationBand[NERtcVoiceEqualizationBand["kNERtcVoiceEqualizationBand_500"] = 4] = "kNERtcVoiceEqualizationBand_500";
    NERtcVoiceEqualizationBand[NERtcVoiceEqualizationBand["kNERtcVoiceEqualizationBand_1K"] = 5] = "kNERtcVoiceEqualizationBand_1K";
    NERtcVoiceEqualizationBand[NERtcVoiceEqualizationBand["kNERtcVoiceEqualizationBand_2K"] = 6] = "kNERtcVoiceEqualizationBand_2K";
    NERtcVoiceEqualizationBand[NERtcVoiceEqualizationBand["kNERtcVoiceEqualizationBand_4K"] = 7] = "kNERtcVoiceEqualizationBand_4K";
    NERtcVoiceEqualizationBand[NERtcVoiceEqualizationBand["kNERtcVoiceEqualizationBand_8K"] = 8] = "kNERtcVoiceEqualizationBand_8K";
    NERtcVoiceEqualizationBand[NERtcVoiceEqualizationBand["kNERtcVoiceEqualizationBand_16K"] = 9] = "kNERtcVoiceEqualizationBand_16K";
})(NERtcVoiceEqualizationBand = exports.NERtcVoiceEqualizationBand || (exports.NERtcVoiceEqualizationBand = {}));
