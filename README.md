# NERTC Electron SDK API 概览

### 说明

[NERtcEngine](NERtcEngine.html) 包含了 Electron NeRTC 接口。

### 房间管理

|方法|功能描述|起始版本|
|---|---|---|
[initialize](NERtcEngine.html#initialize__anchor)|初始化 NERTC SDK 服务。|V3.9.0
[release](NERtcEngine.html#release__anchor)|销毁 IRtcEngine 对象。|V3.9.0
[getVersion](NERtcEngine.html#getVersion__anchor)|查询 SDK 版本号。|V3.9.0
[setChannelProfile](NERtcEngine.html#setChannelProfile__anchor)|设置房间场景。|V3.9.0
[setClientRole](NERtcEngine.html#setClientRole__anchor)|设置用户角色。|V3.9.0
[joinChannel](NERtcEngine.html#joinChannel__anchor)|加入房间。|V3.9.0
[leaveChannel](NERtcEngine.html#leaveChannel__anchor)|离开房间。|V3.9.0
[getConnectionState](NERtcEngine.html#getConnectionState__anchor)|获取网络连接状态。|V3.9.0

### 房间事件

|方法|功能描述|起始版本|
|---|---|---|
[onClientRoleChanged](NERtcEngine.html#onClientRoleChanged__anchor)|用户角色已切换回调。|V3.9.0
[onJoinChannel](NERtcEngine.html#onJoinChannel__anchor)|加入房间回调。|V3.9.0
[onRejoinChannel](NERtcEngine.html#onRejoinChannel__anchor)|重新加入房间回调。|V3.9.0
[onLeaveChannel](NERtcEngine.html#onLeaveChannel__anchor)|离开房间回调。|V3.9.0
[onUserJoined](NERtcEngine.html#onUserJoined__anchor)|远端用户加入当前房间回调。|V3.9.0
[onUserLeft](NERtcEngine.html#onUserLeft__anchor)|远端用户离开当前房间回调。|V3.9.0
[onDisconnect](NERtcEngine.html#onDisconnect__anchor)|服务器连接断开回调。|V3.9.0
[onReconnectingStart](NERtcEngine.html#onReconnectingStart__anchor)|开始重连回调。|V3.9.0
[onConnectionStateChange](NERtcEngine.html#onConnectionStateChange__anchor)|网络连接状态已改变回调。|V3.9.0
[onReleasedHwResources](NERtcEngine.html#onReleasedHwResources__anchor)|通话结束设备资源释放回调。|V3.9.0

### 音频管理

|方法|功能描述|起始版本|
|---|---|---|
[setAudioProfile](NERtcEngine.html#setAudioProfile__anchor)|设置音频编码配置|V3.9.0
[adjustRecordingSignalVolume](NERtcEngine.html#adjustRecordingSignalVolume__anchor)|调节录音音量|V3.9.0
[adjustPlaybackSignalVolume](NERtcEngine.html#adjustPlaybackSignalVolume__anchor)|设置音频编码配置|V3.9.0
[enableLocalAudio](NERtcEngine.html#enableLocalAudio__anchor)|开关本地音频采集|V3.9.0
[enableLocalAudioStream](NERtcEngine.html#enableLocalAudioStream__anchor)|开关本地音频发送|V4.1.110
[muteLocalAudioStream](NERtcEngine.html#muteLocalAudioStream__anchor)|开关本地音频发送|V3.9.0
[subscribeRemoteAudioStream](NERtcEngine.html#subscribeRemoteAudioStream__anchor)|订阅／取消订阅指定音频流。|V3.9.0
[subscribeRemoteAudioSubStream](NERtcEngine.html#subscribeRemoteAudioSubStream__anchor)|订阅／取消订阅指定音频副流。|V4.1.110
[setRemoteHighPriorityAudioStream](NERtcEngine.html#setRemoteHighPriorityAudioStream__anchor)|设置远端用户音频流高优先级|V4.1.110
[setAudioEffectPreset](NERtcEngine.html#setAudioEffectPreset__anchor)|设置 SDK 预设的人声的变声音效|V4.1.110
[setVoiceBeautifierPreset](NERtcEngine.html#setVoiceBeautifierPreset__anchor)|设置 SDK 预设的美声效果。调用该方法可以为本地发流用户设置 SDK 预设的人声美声效果|V4.1.110
[setLocalVoicePitch](NERtcEngine.html#setLocalVoicePitch__anchor)|设置本地语音音调。该方法改变本地说话人声音的音调|V4.1.110
[setLocalVoiceEqualization](NERtcEngine.html#setLocalVoiceEqualization__anchor)|设置本地语音音效均衡，即自定义设置本地人声均衡波段的中心频率|V4.1.110

### 视频管理

|方法|功能描述|起始版本|
|---|---|---|
[enableLocalVideo](NERtcEngine.html#enableLocalVideo__anchor)|开关本地视频|V3.9.0
[setVideoConfig](NERtcEngine.html#setVideoConfig__anchor)|设置视频发送配置|V3.9.0
[setupLocalVideoCanvas](NERtcEngine.html#setupLocalVideoCanvas__anchor)|设置本地用户视图|V3.9.0
[setupRemoteVideoCanvas](NERtcEngine.html#setupRemoteVideoCanvas__anchor)|设置远端用户视图|V3.9.0
[setRenderMode](NERtcEngine.html#setRenderMode__anchor)|设置本地/远端视图显示模式|V3.9.0
[startVideoPreview](NERtcEngine.html#startVideoPreview__anchor)|开启视频预览|V3.9.0
[stopVideoPreview](NERtcEngine.html#stopVideoPreview__anchor)|停止视频预览|V3.9.0
[muteLocalVideoStream](NERtcEngine.html#muteLocalVideoStream__anchor)|开关本地视频发送|V3.9.0
[subscribeRemoteVideoStream](NERtcEngine.html#subscribeRemoteVideoStream__anchor)|订阅 / 取消订阅指定远端用户的视频流|V3.9.0
[setLocalVideoMirrorMode](NERtcEngine.html#setLocalVideoMirrorMode__anchor)|设置本地视频镜像模式|V3.9.0
[setParameters](NERtcEngine.html#setParameters__anchor)|复杂参数设置|V3.9.0
[sendSEIMsg](NERtcEngine.html#sendSEIMsg__anchor)| 发送媒体补充增强信息（SEI）|V4.1.110
[sendSEIMsgEx](NERtcEngine.html#sendSEIMsgEx__anchor)| 发送媒体补充增强信息（SEI）可选主副流|V4.1.110

### 本地媒体事件

|方法|功能描述|起始版本|
|---|---|---|
[onFirstVideoDataReceived](NERtcEngine.html#onFirstVideoDataReceived__anchor)|已显示首帧远端视频回调|V3.9.0
[onFirstAudioDataReceived](NERtcEngine.html#onFirstAudioDataReceived__anchor)|已接收到远端音频首帧回调|V3.9.0
[onFirstAudioFrameDecoded](NERtcEngine.html#onFirstAudioFrameDecoded__anchor)|已解码远端音频首帧的回调|V3.9.0
[onFirstVideoFrameDecoded](NERtcEngine.html#onFirstVideoFrameDecoded__anchor)|已接收到远端视频并完成解码回调|V3.9.0

### 远端媒体事件

|方法|功能描述|起始版本|
|---|---|---|
[onUserAudioStart](NERtcEngine.html#onUserAudioStart__anchor)|远端用户开启音频回调|V3.9.0
[onUserAudioStop](NERtcEngine.html#onUserAudioStop__anchor)|远端用户停用音频回调|V3.9.0
[onUserVideoStart](NERtcEngine.html#onUserVideoStart__anchor)|远端用户开启视频回调|V3.9.0
[onUserVideoStop](NERtcEngine.html#onUserVideoStop__anchor)|远端用户停用视频回调|V3.9.0
[onUserVideoProfileUpdate](NERtcEngine.html#onUserVideoProfileUpdate__anchor)|远端用户视频配置更新回调|V3.9.0
[onUserAudioMute](NERtcEngine.html#onUserAudioMute__anchor)|远端用户是否静音回调|V3.9.0
[onUserVideoMute](NERtcEngine.html#onUserVideoMute__anchor)|远端用户是否禁视频流回调|V3.9.0
[onUserVideoMute](NERtcEngine.html#onUserVideoMute__anchor)|远端用户是否禁视频流回调|V3.9.0

### 数据统计事件

|方法|功能描述|起始版本|
|---|---|---|
[onRemoteAudioStats](NERtcEngine.html#onRemoteAudioStats__anchor)|通话中远端音频流的统计信息回调|V3.9.0
[onRtcStats](NERtcEngine.html#onRtcStats__anchor)|当前通话统计回调|V3.9.0
[onNetworkQuality](NERtcEngine.html#onNetworkQuality__anchor)|通话中每个用户的网络上下行质量报告回调|V3.9.0
[onLocalAudioStats](NERtcEngine.html#onLocalAudioStats__anchor)|本地音频流统计信息回调|V3.9.0
[onLocalVideoStats](NERtcEngine.html#onLocalVideoStats__anchor)|本地视频流统计信息回调|V3.9.0
[onRemoteVideoStats](NERtcEngine.html#onRemoteVideoStats__anchor)|通话中远端视频流的统计信息回调|V3.9.0

### 屏幕共享

|方法|功能描述|起始版本|
|---|---|---|
[startScreenCaptureByDisplayId](NERtcEngine.html#startScreenCaptureByDisplayId__anchor)|通过屏幕 ID 共享屏幕，该方法仅适用于 macOS|V3.9.0
[startScreenCaptureByWindowId](NERtcEngine.html#startScreenCaptureByWindowId__anchor)|通过窗口 ID 共享窗口|V3.9.0
[updateScreenCaptureRegion](NERtcEngine.html#updateScreenCaptureRegion__anchor)|更新屏幕共享区域|V3.9.0
[stopScreenCapture](NERtcEngine.html#stopScreenCapture__anchor)|停止屏幕共享|V3.9.0
[startScreenCaptureByScreenRect](NERtcEngine.html#startScreenCaptureByScreenRect__anchor)|通过指定区域共享屏幕|V3.9.0
[pauseScreenCapture](NERtcEngine.html#pauseScreenCapture__anchor)|暂停屏幕共享。|V3.9.0
[resumeScreenCapture](NERtcEngine.html#resumeScreenCapture__anchor)|恢复屏幕共享。|V3.9.0
[setupLocalSubStreamVideoCanvas](NERtcEngine.html#setupLocalSubStreamVideoCanvas__anchor)|设置本端的辅流视频回放画布|V3.9.0
[setupRemoteSubStreamVideoCanvas](NERtcEngine.html#setupRemoteSubStreamVideoCanvas__anchor)|设置远端的辅流视频回放画布|V3.9.0
[subscribeRemoteVideoSubStream](NERtcEngine.html#subscribeRemoteVideoSubStream__anchor)|订阅或取消订阅远端的屏幕共享辅流视频，订阅之后才能接收远端的辅流视频数据|V3.9.0
[setSubStreamRenderMode](NERtcEngine.html#setSubStreamRenderMode__anchor)|设置屏幕共享辅流视频渲染缩放模式|V3.9.0

|事件|描述|起始版本|
|---|---|---|
[onUserSubStreamVideoStart](NERtcEngine.html#event:onUserSubStreamVideoStart__anchor)|远端用户开启屏幕共享辅流通道的回调|V3.9.0
[onUserSubStreamVideoStop](NERtcEngine.html#event:onUserSubStreamVideoStop__anchor)|远端用户停止屏幕共享辅流通道的回调|V3.9.0

### 音乐文件播放及混音

|方法|功能描述|起始版本|
|---|---|---|
[startAudioMixing](NERtcEngine.html#startAudioMixing__anchor)|开始播放音乐文件|V3.9.0
[stopAudioMixing](NERtcEngine.html#stopAudioMixing__anchor)|停止播放音乐文件|V3.9.0
[pauseAudioMixing](NERtcEngine.html#pauseAudioMixing__anchor)|暂停播放音乐文件|V3.9.0
[resumeAudioMixing](NERtcEngine.html#resumeAudioMixing__anchor)|恢复播放音乐文件|V3.9.0
[setAudioMixingPlaybackVolume](NERtcEngine.html#setAudioMixingPlaybackVolume__anchor)|设置音乐文件播放音量|V3.9.0
[setAudioMixingSendVolume](NERtcEngine.html#setAudioMixingSendVolume__anchor)|设置音乐文件的发送音量|V3.9.0
[getAudioMixingPlaybackVolume](NERtcEngine.html#getAudioMixingPlaybackVolume__anchor)|获取音乐文件的播放音量|V3.9.0
[getAudioMixingSendVolume](NERtcEngine.html#getAudioMixingSendVolume__anchor)|获取音乐文件的发送音量|V3.9.0
[getAudioMixingDuration](NERtcEngine.html#getAudioMixingDuration__anchor)|获取音乐文件的总长度|V3.9.0
[getAudioMixingCurrentPosition](NERtcEngine.html#getAudioMixingCurrentPosition__anchor)|获取音乐文件的播放进度|V3.9.0
[setAudioMixingPosition](NERtcEngine.html#setAudioMixingPosition__anchor)|设置音乐文件的播放进度|V3.9.0

|事件|描述|起始版本|
|---|---|---|
[onAudioMixingStateChanged](NERtcEngine.html#event:onAudioMixingStateChanged__anchor)|本地用户的音乐文件播放状态改变回调|V3.9.0
[onAudioMixingTimestampUpdate](NERtcEngine.html#event:onAudioMixingTimestampUpdate__anchor)|本地用户的音乐文件播放进度回调|V3.9.0

### 音效文件播放管理

|方法|功能描述|起始版本|
|---|---|---|
[getEffectPlaybackVolume](NERtcEngine.html#getEffectPlaybackVolume__anchor)|获取音效文件播放音量|V3.9.0
[setEffectPlaybackVolume](NERtcEngine.html#setEffectPlaybackVolume__anchor)|设置音效文件播放音量|V3.9.0
[playEffect](NERtcEngine.html#playEffect__anchor)|播放指定音效文件|V3.9.0
[stopEffect](NERtcEngine.html#stopEffect__anchor)|停止播放指定音效文件|V3.9.0
[stopAllEffects](NERtcEngine.html#stopAllEffects__anchor)|停止播放所有音效文件|V3.9.0
[pauseEffect](NERtcEngine.html#pauseEffect__anchor)|暂停音效文件播放|V3.9.0
[pauseAllEffects](NERtcEngine.html#pauseAllEffects__anchor)|暂停所有音效文件播放|V3.9.0
[resumeEffect](NERtcEngine.html#resumeEffect__anchor)|恢复播放指定音效文件|V3.9.0
[resumeAllEffects](NERtcEngine.html#resumeAllEffects__anchor)|恢复播放所有音效文件|V3.9.0
[setEffectSendVolume](NERtcEngine.html#setEffectSendVolume__anchor)|调节音效文件发送音量|V3.9.0
[getEffectSendVolume](NERtcEngine.html#getEffectSendVolume__anchor)|获取音效文件发送音量|V3.9.0

|事件|描述|起始版本|
|---|---|---|
[onAudioEffectFinished](NERtcEngine.html#event:onAudioEffectFinished__anchor)|本地音效文件播放已结束回调|V3.9.0

### 本地声卡采集

|方法|功能描述|起始版本|
|---|---|---|
[enableLoopbackRecording](NERtcEngine.html#enableLoopbackRecording__anchor)|开启声卡采集|V4.1.110
[adjustLoopbackRecordingSignalVolume](NERtcEngine.html#adjustLoopbackRecordingSignalVolume__anchor)|调节声卡采集信号音量|V4.1.110
[adjustUserPlaybackSignalVolume](NERtcEngine.html#adjustUserPlaybackSignalVolume__anchor)|调节本地播放的指定远端用户的指定流类型的信号音量|V4.1.110

### 音量提示

|方法|功能描述|起始版本|
|---|---|---|
[enableAudioVolumeIndication](NERtcEngine.html#enableAudioVolumeIndication__anchor)|启用说话者音量提示|V3.9.0

|事件|描述|起始版本|
|---|---|---|
[onRemoteAudioVolumeIndication](NERtcEngine.html#event:onRemoteAudioVolumeIndication__anchor)|提示频道内谁正在说话及说话者音量的回调|V3.9.0
[onLocalAudioVolumeIndication](NERtcEngine.html#event:onLocalAudioVolumeIndication__anchor)|提示频道内本地用户瞬时音量的回调|V3.9.0

### 耳返

|方法|功能描述|起始版本|
|---|---|---|
[enableEarback](NERtcEngine.html#enableEarback__anchor)|开启耳返功能|V3.9.0
[setEarbackVolume](NERtcEngine.html#setEarbackVolume__anchor)|设置耳返音量|V3.9.0

### 旁路推流（互动直播）

|方法|功能描述|起始版本|
|---|---|---|
[addLiveStreamTask](NERtcEngine.html#addLiveStreamTask__anchor)|添加房间推流任务|V3.9.0
[updateLiveStreamTask](NERtcEngine.html#updateLiveStreamTask__anchor)|更新修改房间推流任务|V3.9.0
[removeLiveStreamTask](NERtcEngine.html#removeLiveStreamTask__anchor)|删除房间推流任务|V3.9.0

|事件|描述|起始版本|
|---|---|---|
[onAddLiveStreamTask](NERtcEngine.html#event:onAddLiveStreamTask__anchor)|通知添加直播任务结果|V3.9.0
[onUpdateLiveStreamTask](NERtcEngine.html#event:onUpdateLiveStreamTask__anchor)|通知更新直播任务结果|V3.9.0
[onRemoveLiveStreamTask](NERtcEngine.html#event:onRemoveLiveStreamTask__anchor)|通知删除直播任务结果|V3.9.0
[onLiveStreamState](NERtcEngine.html#event:onLiveStreamState__anchor)|通知直播推流状态|V3.9.0

### 音频设备管理

|方法|功能描述|起始版本|
|---|---|---|
[setRecordDevice](NERtcEngine.html#setRecordDevice__anchor)|设置音频采集设备|V3.9.0
[getRecordDevice](NERtcEngine.html#getRecordDevice__anchor)|获取当前音频采集设备|V3.9.0
[enumeratePlayoutDevices](NERtcEngine.html#enumeratePlayoutDevices__anchor)|枚举音频播放设备|V3.9.0
[setPlayoutDevice](NERtcEngine.html#setPlayoutDevice__anchor)|设备音频播放设备|V3.9.0
[getPlayoutDevice](NERtcEngine.html#getPlayoutDevice__anchor)|获取当前音频播放设备|V3.9.0
[setRecordDeviceVolume](NERtcEngine.html#setRecordDeviceVolume__anchor)|设置当前音频采集设备音量|V3.9.0
[getRecordDeviceVolume](NERtcEngine.html#getRecordDeviceVolume__anchor)|获取当前音频采集设备音量|V3.9.0
[setPlayoutDeviceVolume](NERtcEngine.html#setPlayoutDeviceVolume__anchor)|设置当前音频播放设备音量|V3.9.0
[getPlayoutDeviceVolume](NERtcEngine.html#getPlayoutDeviceVolume__anchor)|获取当前音频播放设别音量|V3.9.0
[setPlayoutDeviceMute](NERtcEngine.html#setPlayoutDeviceMute__anchor)|设置当前播放设备静音状态|V3.9.0
[getPlayoutDeviceMute](NERtcEngine.html#getPlayoutDeviceMute__anchor)|获取当前播放设备静音状态|V3.9.0
[setRecordDeviceMute](NERtcEngine.html#setRecordDeviceMute__anchor)|设置当前采集设备静音状态|V3.9.0
[getRecordDeviceMute](NERtcEngine.html#getRecordDeviceMute__anchor)|获取当前采集设备静音状态|V3.9.0
[startRecordDeviceTest](NERtcEngine.html#startRecordDeviceTest__anchor)|开始测试音频采集设备|V3.9.0
[stopRecordDeviceTest](NERtcEngine.html#stopRecordDeviceTest__anchor)|停止测试音频采集设备|V3.9.0
[startPlayoutDeviceTest](NERtcEngine.html#startPlayoutDeviceTest__anchor)|开始测试音频播放设备|V3.9.0
[stopPlayoutDeviceTest](NERtcEngine.html#stopPlayoutDeviceTest__anchor)|停止测试音频播放设备|V3.9.0
[startAudioDeviceLoopbackTest](NERtcEngine.html#startAudioDeviceLoopbackTest__anchor)|开始音频采集播放设备回路测试|V3.9.0
[stopAudioDeviceLoopbackTest](NERtcEngine.html#stopAudioDeviceLoopbackTest__anchor)|停止音频采集播放设备回路测试|V3.9.0

|事件|描述|起始版本|
|---|---|---|
[onAudioHowling](NERtcEngine.html#event:onAudioHowling__anchor)|检测到啸叫回调。|V3.9.0

### 视频设备管理

|方法|功能描述|起始版本|
|---|---|---|
[setVideoDevice](NERtcEngine.html#setVideoDevice__anchor)|设置视频采集设备|V3.9.0
[getVideoDevice](NERtcEngine.html#getVideoDevice__anchor)|获取当前视频采集设备|V3.9.0

### 设备管理事件

|方法|功能描述|起始版本|
|---|---|---|
[onAudioDeviceStateChanged](NERtcEngine.html#onAudioDeviceStateChanged__anchor)|音频设备状态更改回调|V3.9.0
[onAudioDefaultDeviceChanged](NERtcEngine.html#onAudioDefaultDeviceChanged__anchor)|音频默认设备更改回调|V3.9.0
[onVideoDeviceStateChanged](NERtcEngine.html#onVideoDeviceStateChanged__anchor)|视频设备状态更改回调|V3.9.0

### 故障排查

|方法|功能描述|起始版本|
|---|---|---|
[startAudioDump](NERtcEngine.html#startAudioDump__anchor)|开始记录音频 dump 音频 dump 可用于分析音频问题|V3.9.0
[stopAudioDump](NERtcEngine.html#stopAudioDump__anchor)|结束记录音频 dump|V3.9.0
[getErrorDescription](NERtcEngine.html#getErrorDescription__anchor)|获取错误描述。|V3.9.0
[uploadSdkInfo](NERtcEngine.html#uploadSdkInfo__anchor)|上传SDK日志信息|V3.9.0
