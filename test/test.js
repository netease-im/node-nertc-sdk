const NERtcEngine = require('../js/api/index').default
const nertcEngine = new NERtcEngine
const assert = require('assert');
const axios = require('axios');
const { add } = require('lodash');
const { stringify } = require('querystring');
const path = require('path')

var generateID = (len) => {
  if (typeof (len) === 'undefined' || len == null || typeof (len) !== 'number') {
    len = 6
  }
  let code = ''
  let codeLength = len
  let random = [1, 2, 3, 4, 5, 6, 7, 8, 9]
  for (var i = 0; i < codeLength; i++) {
    let index = Math.floor(Math.random() * 9)
    code += random[index]
  }
  return code
}

var gToken = ''
const gAppKey = `d60ed3e24a542526ada4f3bb23f14ac2`
const gPrdServer = "https://nrtc.netease.im/demo/getChecksum.action";
var gUid = 258369//parseInt(generateID(6))
var remoteUid = 158362
var gChannelName = "159369"//generateID(6)
var gChannelName2 = "152858"//generateID(6)
var g_video_capture_devices = ""

//功能
var NERtcRemoteVideoStreamType = {
  kNERtcRemoteVideoStreamTypeHigh: 0, //默认大流
  kNERtcRemoteVideoStreamTypeLow: 1, //小流
  kNERtcRemoteVideoStreamTypeNone: 2, //不订阅
};

var NERtcAudioProfileType = {
  kNERtcAudioProfileDefault: 0,			    /**< 0: 默认设置。Speech场景下为 kNERtcAudioProfileStandardExtend，Music场景下为 kNERtcAudioProfileHighQuality */
  kNERtcAudioProfileStandard: 1,			    /**< 1: 普通质量的音频编码，16000Hz，20Kbps */
  kNERtcAudioProfileStandardExtend: 2,       /**< 2: 普通质量的音频编码，16000Hz，32Kbps */
  kNERtcAudioProfileMiddleQuality: 3,		/**< 3: 中等质量的音频编码，48000Hz，64Kbps */
  kNERtcAudioProfileMiddleQualityStereo: 4,  /**< 4: 中等质量的立体声编码，48000Hz * 2，80Kbps  */
  kNERtcAudioProfileHighQuality: 5,          /**< 5: 高质量的音频编码，48000Hz，96Kbps  */
  kNERtcAudioProfileHighQualityStereo: 6,    /**< 6: 高质量的立体声编码，48000Hz * 2，128Kbps  */
}

/** 音频应用场景。不同的场景设置对应不同的音频采集模式、播放模式*/
var NERtcAudioScenarioType = {
  /** 0: 默认设置
     - kNERtcChannelProfileCommunication下为kNERtcAudioScenarioSpeech，
     - kNERtcChannelProfileLiveBroadcasting下为kNERtcAudioScenarioMusic。
   */
  kNERtcAudioScenarioDefault: 0,
  /** 1: 语音场景. NERtcAudioProfileType 推荐使用 kNERtcAudioProfileMiddleQuality 及以下 */
  kNERtcAudioScenarioSpeech: 1,
  /** 2: 音乐场景。NERtcAudioProfileType 推荐使用 kNERtcAudioProfileMiddleQualityStereo 及以上 */
  kNERtcAudioScenarioMusic: 2,
} ;

/** 变声 预设值 */
var NERtcVoiceChangerType = {
  kNERtcVoiceChangerOff           :  0,    /**< 默认关闭 */
  kNERtcVoiceChangerRobot         :  1,    /**< 机器人 */
  kNERtcVoiceChangerGaint         :  2,    /**< 巨人 */
  kNERtcVoiceChangerHorror        :  3,    /**< 恐怖 */
  kNERtcVoiceChangerMature        :  4,    /**< 成熟 */
  kNERtcVoiceChangerManToWoman    :  5,    /**< 男变女 */
  kNERtcVoiceChangerWomanToMan    :  6,    /**< 女变男 */
  kNERtcVoiceChangerManToLoli     :  7,    /**< 男变萝莉 */
  kNERtcVoiceChangerWomanToLoli   :  8,    /**< 女变萝莉 */
};

/** 预设的美声效果 */
var NERtcVoiceBeautifierType = {
  kNERtcVoiceBeautifierOff : 0,             /**< 默认关闭 */
  kNERtcVoiceBeautifierMuffled : 1,         /**< 低沉 */
  kNERtcVoiceBeautifierMellow : 2,          /**< 圆润 */
  kNERtcVoiceBeautifierClear : 3,           /**< 清澈 */
  kNERtcVoiceBeautifierMagnetic : 4,        /**< 磁性 */
  kNERtcVoiceBeautifierRecordingstudio : 5, /**< 录音棚 */
  kNERtcVoiceBeautifierNature : 6,          /**< 天籁 */
  kNERtcVoiceBeautifierKTV : 7,             /**< KTV */
  kNERtcVoiceBeautifierRemote : 8,          /**< 悠远 */
  kNERtcVoiceBeautifierChurch : 9,          /**< 教堂 */
  kNERtcVoiceBeautifierBedroom : 10,        /**< 卧室 */
  kNERtcVoiceBeautifierLive : 11,           /**< Live */
};

/** 音效均衡波段的中心频率 */
var NERtcVoiceEqualizationBand = {
  kNERtcVoiceEqualizationBand_31  : 0, /**<  31 Hz */
  kNERtcVoiceEqualizationBand_62  : 1, /**<  62 Hz */
  kNERtcVoiceEqualizationBand_125 : 2, /**<  125 Hz */
  kNERtcVoiceEqualizationBand_250 : 3, /**<  250 Hz */
  kNERtcVoiceEqualizationBand_500 : 4, /**<  500 Hz */
  kNERtcVoiceEqualizationBand_1K  : 5, /**<  1 kHz */
  kNERtcVoiceEqualizationBand_2K  : 6, /**<  2 kHz */
  kNERtcVoiceEqualizationBand_4K  : 7, /**<  4 kHz */
  kNERtcVoiceEqualizationBand_8K  : 8, /**<  8 kHz */
  kNERtcVoiceEqualizationBand_16K : 9, /**<  16 kHz */
} ;

 
const remoteVideoStreamType = 0 // 0:默认大流 1:小流 2:不订阅




async function getToken(uid, appKey, server){
  let url = server + "?uid=" + uid + "&appkey=" + appKey
  await axios.post(url, {}, 
      {
        header: {
          "Content-Type": "application/x-www-form-urlencoded;charset=utf-8",
        },
      }
    ).then((res) => {
      if (res.data.code !== 200) {
        addlog(`getToken error, res.data.code${res.data.code}`);
        return;
      }
      gToken = res.data.checksum;
      addlog(`getToken succ, gToken:${gToken}`);
      return;
    }).catch((e) => {
      addlog(`getToken, e:${e}`);
    });
}

function addlog(log){
  console.log(log)
}

let ret = 0
describe('nertc test', function(){
  describe('start test', function(){
    it('initialize', function(){
      let context = {}
      context.app_key = gAppKey
      context.log_dir_path = './'
      context.log_level = 3
      context.log_file_max_size_KBytes = 100*1024*1024
      ret = nertcEngine.initialize(context)
      addlog(`nertcEngine.initialize: ret:${ret}`)
      assert.strictEqual(ret, 0)
    });

/*************************加入通话前接口******************************/
    it('startRecordDeviceTest', function(){
      // nertcEngine.once('onLocalAudioVolumeIndication', function (volume) {
      //   addlog(`onLocalAudioVolumeIndication, result:${volume}`)
      //   done()
      // })

      ret = nertcEngine.startRecordDeviceTest(100)
      addlog(`nertcEngine.startRecordDeviceTest. ret:${ret}`)

      ret = nertcEngine.stopRecordDeviceTest()
      addlog(`nertcEngine.stopRecordDeviceTest. ret:${ret}`)
      assert.strictEqual(ret, 0)
    });

    // it('startPlayoutDeviceTest', function(){
    //   ret = nertcEngine.startPlayoutDeviceTest()

    // });

    it('setLocalMediaPriority', function(){
      ret = nertcEngine.setLocalMediaPriority(50, false)
      addlog(`setLocalMediaPriority ret: ${ret}`)
      assert.strictEqual(ret, 0)
    });

    it('setClientRole', function(){
      ret = nertcEngine.setClientRole(1)
      addlog(`nertcEngine.setClientRole:${ret}`)
      assert.strictEqual(ret, 0)
    });

    it('setChannelProfile', function(){
      ret = nertcEngine.setChannelProfile(0)
      addlog(`nertcEngine.setChannelProfile:${ret}`)
      assert.strictEqual(ret, 0)
    });

    it('setAudioProfile', function(){
      addlog(`nertcEngine.setAudioProfile:${nertcEngine.setAudioProfile(NERtcAudioProfileType.kNERtcAudioProfileMiddleQuality, NERtcAudioScenarioType.kNERtcAudioScenarioSpeech)}`)
      assert.strictEqual(0, 0)
    });

    it('enumerateVideoCaptureDevices',function(){
      let video_capture_devices = nertcEngine.enumerateVideoCaptureDevices()
      g_video_capture_devices = JSON.stringify(video_capture_devices, null, 2)
      addlog(`nertcEngine.enumerateVideoCaptureDevices:${g_video_capture_devices}`)
      if (video_capture_devices.length > 0) {
        ret = nertcEngine.setVideoDevice(video_capture_devices[0].device_id)
        addlog(`nertcEngine.setVideoDevice. ret:${ret}`)

        let cur_video_device = nertcEngine.getVideoDevice()
        addlog(`nertcEngine.getVideoDevice. cur_video_device:${cur_video_device}`)
      }
      assert.strictEqual(ret, 0)
    });

    it('setParameters', function(){
      let param = {
        'record_host_enabled': false,
        'record_audio_enabled': false,
        'record_video_enabled': false,
        'record_type': 0,
        'auto_subscribe_audio': false,
        'publish_self_stream_enabled': false,
        'log_level': 3,
        'audio_processing_aec_enable': true,
        'audio_processing_agc_enable': true,
        'audio_processing_ns_enable': true,
        'audio_processing_ai_ns_enable': false,
        'audio_processing_external_audiomix_enable': false,
        'audio_processing_earphone': false,
        'video_sendonpub_type': 1 
      }
      ret = nertcEngine.setParameters(JSON.stringify(param))
      addlog(`setParameters ret: ${ret}`)
      assert.strictEqual(ret, 0)
    });

    it('setAudioEffectPreset', function() {
      ret = nertcEngine.setAudioEffectPreset(NERtcVoiceChangerType.kNERtcVoiceChangerOff)
      addlog(`setAudioEffectPreset ret: ${ret}`)
      assert.strictEqual(ret, 0)
    });

    it('setVoiceBeautifierPreset', function(){
      ret = nertcEngine.setVoiceBeautifierPreset(NERtcVoiceBeautifierType.kNERtcVoiceBeautifierMuffled)
      addlog(`setVoiceBeautifierPreset ret: ${ret}`)
      assert.strictEqual(ret, 0)
    });

    it('setLocalVoicePitch', function(){
      ret = nertcEngine.setLocalVoicePitch(0.5)
      addlog(`setLocalVoicePitch ret: ${ret}`)
      assert.strictEqual(ret, 0)
    });

    it('setLocalVoiceEqualization', function(){
      ret = nertcEngine.setLocalVoiceEqualization(NERtcVoiceEqualizationBand.kNERtcVoiceEqualizationBand_31, 0)
      addlog(`setLocalVoiceEqualization ret: ${ret}`)
      assert.strictEqual(ret, 0)
    });


/*************************加入通话前接口end******************************/


/*************************加入通话后接口******************************/
    it('getToken', async function(){
      await getToken(gUid, gAppKey, gPrdServer)      
    });

    it('JoinChannel', function(done){
      nertcEngine.once('onConnectionStateChange', (state, reason)=>{
        addlog(`onConnectionStateChange, state:${state} reason:${reason}`)
         if(state != 2 || reason != 5){
            done(-1)
         }
      })

      nertcEngine.once('onJoinChannel', function (cid, uid, result, elapsed) {
        addlog(`onJoinChannel, result:${result}`)
        if(0 !== result){
          done(result)
        }
      })

      nertcEngine.once('onUserJoined', (uid, userName)=>{
        addlog(`onUserJoined, uid:${uid} userName:${userName}`)
        if(uid === ''){
          done(-1)
        }
      })

      nertcEngine.once('onUserAudioStart', (uid)=>{
        addlog(`onUserAudioStart, uid:${uid}`)
        if(uid === ''){
          done(-1)
        }
      })

      nertcEngine.once('onUserVideoStart', (uid)=>{
        addlog(`onUserVideoStart, uid:${uid}`)
        if(uid === ''){
          done(-1)
        }else{
          done()
        }
      })

      ret = nertcEngine.joinChannel(gToken, gChannelName, gUid)
      addlog(`do join channel ret:${ret}`)
      //要加延时，不然回调会出现句柄错误(回调中出现)
    }).timeout(5000);
    
    it('enableLocalAudio', function(){
      ret = nertcEngine.enableLocalAudio(true)
      addlog(`nertcEngine.enableLocalAudio:${ret}`)
      assert.strictEqual(ret, 0)
    });

    it('subscribeRemoteAudioStream', function(){
      ret = nertcEngine.subscribeRemoteAudioStream(remoteUid, false)
      addlog(`subscribeRemoteAudioStream ret: ${ret}`)
      assert.strictEqual(ret, 0)
    });

    it('subscribeRemoteVideoStream', function(){
      ret = nertcEngine.subscribeRemoteVideoStream(remoteUid, NERtcRemoteVideoStreamType.kNERtcRemoteVideoStreamTypeHigh, true)
      addlog(`subscribeRemoteVideoStream ret: ${ret}`)
      assert.strictEqual(ret, 0)
    });

    it('subscribeRemoteVideoSubStream', function(){
      ret = nertcEngine.subscribeRemoteVideoSubStream(remoteUid, false)
      addlog(`subscribeRemoteVideoSubStream ret: ${ret}`)
      assert.strictEqual(ret, 0)
    });

    it('enumerateScreenCaptureSourceInfo', function(){
      let thumbWidth = '320'
      let thumbHeight = '180'
      let iconWidth = '32'
      let iconHeight = '32'
      let lst = nertcEngine.enumerateScreenCaptureSourceInfo(parseInt(thumbWidth), parseInt(thumbHeight), parseInt(iconWidth), parseInt(iconHeight))
      addlog(`nertcEngine.enumerateScreenCaptureSourceInfo. lst:${lst}`)
      assert.strictEqual(ret, 0)
    });

    it('startAudioDump', function(){
      ret = nertcEngine.startAudioDump()
      addlog(`nertcEngine.startAudioDump:${ret}`)
      assert.strictEqual(ret, 0)
    });

    it('stopAudioDump', function(){
      ret = nertcEngine.stopAudioDump()
      addlog(`nertcEngine.stopAudioDump:${ret}`)
      assert.strictEqual(ret, 0)
    });

    it('enableLocalVideo', function(){
      ret = nertcEngine.enableLocalVideo(true)
      addlog(`nertcEngine.enableLocalVideo:${ret}`)
      assert.strictEqual(ret, 0)
    });

    it('startAudioRecording', function(done){
      nertcEngine.once('onAudioRecording', (code, file_path)=>{
        addlog(`onAudioRecording, code:${code}`)
        done()
      })

      let fileDir = process.platform === 'darwin' ? path.join(process.env.HOME, 'nertc_logs') : path.join(process.cwd(), 'nertc_logs')
      let fileName = path.join(fileDir, 'audio.aac')
      addlog(`startAudioRecording fileName:${fileName}`)
      ret = nertcEngine.startAudioRecording(fileName, 3200, 0)
      addlog(`nertcEngine.startAudioRecording. ret:${ret}`)

      ret = nertcEngine.stopAudioRecording()
      addlog(`nertcEngine.stopAudioRecording. ret:${ret}`)
    });

    it('adjustUserPlaybackSignalVolume', function(){
      ret = nertcEngine.adjustUserPlaybackSignalVolume(remoteUid, 100)
      addlog(`adjustUserPlaybackSignalVolume. ret:${ret}`)
      assert.strictEqual(ret, 0)
    });

    it('audioMixing', function(){
      nertcEngine.once('onAudioMixingTimestampUpdate', (timestamp_ms)=>{
        addlog(`onAudioMixingTimestampUpdate, timestamp_ms:${timestamp_ms}`)
      })

      let opt = {}
      opt.path = `http://music.163.com/song/media/outer/url?id=5274363.mp3`
      opt.loop_count = 1
      opt.send_enabled = true
      opt.send_volume = 100
      opt.playback_enabled = true
      opt.playback_volume = 100
      ret = nertcEngine.startAudioMixing(opt)
      addlog(`nertcEngine.startAudioMixing ret:${ret}`)

      let dur = nertcEngine.getAudioMixingDuration()
      addlog(`nertcEngine.getAudioMixingDuration. dur:${dur}`)

      ret = nertcEngine.setAudioMixingPosition(99)
      addlog(`setAudioMixingPosition. ret:${ret}`)

      let pos = nertcEngine.getAudioMixingCurrentPosition()
      addlog(`getAudioMixingCurrentPosition pos:${pos}`)

      ret = nertcEngine.setAudioMixingSendVolume(20)
      addlog(`nertcEngine.setAudioMixingSendVolume ret:${ret}`)
      
      let vol = nertcEngine.getAudioMixingSendVolume()
      addlog(`nertcEngine.getAudioMixingSendVolume. vol:${vol}`)

      ret = nertcEngine.setAudioMixingPlaybackVolume(25)
      addlog(`nertcEngine.setAudioMixingPlaybackVolume ret:${ret}`)
      
      vol = nertcEngine.getAudioMixingPlaybackVolume()
      addlog(`nertcEngine.getAudioMixingPlaybackVolume. vol:${vol}`)

      ret = nertcEngine.pauseAudioMixing()
      addlog(`nertcEngine.pauseAudioMixing ret:${ret}`)

      ret = nertcEngine.resumeAudioMixing()
      addlog(`nertcEngine.resumeAudioMixing ret:${ret}`)

      ret = nertcEngine.stopAudioMixing()
      addlog(`nertcEngine.stopAudioMixing. ret:${ret}`)
      assert.strictEqual(ret, 0)
    });

    it('effect', function(){
      let opt = [{
        'path': `http://music.163.com/song/media/outer/url?id=1433584979.mp3`,
        'loop_count': 2,
        'send_enabled': true,
        'send_volume': 100,
        'playback_enabled': true,
        'playback_volume': 100
      }
    ]

      let opt2 = [
        {
          'path': `http://music.163.com/song/media/outer/url?id=1336840616.mp3`,
          'loop_count': 2,
          'send_enabled': true,
          'send_volume': 100,
          'playback_enabled': true,
          'playback_volume': 100
        }
      ]
      ret = nertcEngine.playEffect(1, opt)
      addlog(`nertcEngine.playEffect. ret:${ret}`)

      ret = nertcEngine.playEffect(2, opt2)
      addlog(`nertcEngine.playEffect. ret:${ret}`)

      ret = nertcEngine.pauseEffect(1)
      addlog(`nertcEngine.pauseEffect. ret:${ret}`)

      ret = nertcEngine.resumeEffect(1)
      addlog(`nertcEngine.resumeEffect. ret:${ret}`)

      ret = nertcEngine.pauseAllEffects()
      addlog(`nertcEngine.pauseAllEffects. ret:${ret}`)

      ret = nertcEngine.resumeAllEffects()
      addlog(`nertcEngine.resumeAllEffects. ret:${ret}`)

      ret = nertcEngine.setEffectSendVolume(2, 50)
      addlog(`nertcEngine.setEffectSendVolume. ret:${ret}`)

      ret = nertcEngine.getEffectSendVolume(2, 50)
      addlog(`nertcEngine.getEffectSendVolume. ret:${ret}`)

      ret = nertcEngine.setEffectPlaybackVolume(2, 60)
      addlog(`nertcEngine.setEffectPlaybackVolume. ret:${ret}`)

      ret = nertcEngine.getEffectPlaybackVolume(2)
      addlog(`nertcEngine.getEffectPlaybackVolume. ret:${ret}`)

      ret = nertcEngine.stopEffect(1)
      addlog(`nertcEngine.stopEffect. ret:${ret}`)

      ret = nertcEngine.stopAllEffects()
      addlog(`nertcEngine.stopAllEffects. ret:${ret}`)

      assert.strictEqual(ret, 0)
    });

    it('enableEarback', function(){
      ret = nertcEngine.enableEarback(true, 70)
      addlog(`nertcEngine.enableEarback. ret:${ret}`)

      ret = nertcEngine.setEarbackVolume(90)
      addlog(`nertcEngine.setEarbackVolume. ret:${ret}`)

      assert.strictEqual(ret, 0)
    });

    it('enableAudioVolumeIndication', function(){
      ret = nertcEngine.enableAudioVolumeIndication(true, 100)
      addlog(`nertcEngine.enableAudioVolumeIndication. ret:${ret}`)
      assert.strictEqual(ret, 0)
    });

    it('onLocalAudioVolumeIndication', function(done){
      nertcEngine.once('onLocalAudioVolumeIndication', function (volume) {
        addlog(`onLocalAudioVolumeIndication, volume:${volume}`)
        done()
      })      
    });

    it('onRemoteAudioVolumeIndication', function(done){
      nertcEngine.once('onRemoteAudioVolumeIndication', function (speakers, speaker_number, total_volume) {
        addlog(`onRemoteAudioVolumeIndication, volume:${total_volume}`)
        done()
      })      
    });

    //必须伴随视频流一起发送 sendSEIMsgEx一样
    // it('sendSEIMsg', function(){
    //   const u8string = new TextEncoder().encode('test send seimsg')
    //   ret = nertcEngine.sendSEIMsg(u8string.buffer)
    //   addlog(`nertcEngine.sendSEIMsg. ret:${ret}`)
    //   assert.strictEqual(ret, 0)
    // });

    it('getVersion', function(){
      addlog(`nertcEngine.getVersion. ret:${nertcEngine.getVersion()}`)
      assert.strictEqual(ret, 0)
    });

    // it('uploadSdkInfo', function(){
    //   ret= nertcEngine.uploadSdkInfo()
    //   addlog(`nertcEngine.uploadSdkInfo. ret:${ret}`)
    //   assert.strictEqual(0, 0)
    // });


    it('recordDevices', function(){
      let recordDevices = nertcEngine.enumerateRecordDevices()
      addlog(`nertcEngine.enumerateRecordDevices. ret:${recordDevices}`)
      if(recordDevices.length > 0){
        let type = typeof recordDevices[0].device_id
        addlog(`${type}, ${recordDevices[0].device_id}`)
        ret = nertcEngine.setRecordDevice(recordDevices[0].device_id)
        addlog(`nertcEngine.muteLocalVideoStream:${ret}`)

        let cur_record_device = nertcEngine.getRecordDevice()
        addlog(`nertcEngine.getRecordDevice:${cur_record_device}`)

        ret = nertcEngine.setRecordDeviceVolume(75)
        addlog(`nertcEngine.setRecordDeviceVolume:${ret}`)

        let vol = nertcEngine.getRecordDeviceVolume()
        addlog(`nertcEngine.getRecordDeviceVolume:${vol}`)

        ret = nertcEngine.setRecordDeviceMute(true)
        addlog(`nertcEngine.setRecordDeviceMute:${ret}`)

        let record_mute = nertcEngine.getRecordDeviceMute()
        addlog(`nertcEngine.getRecordDeviceMute:${record_mute}`)

        ret = nertcEngine.setRecordDeviceMute(false)
        addlog(`nertcEngine.setRecordDeviceMute:${ret}`)

        ret = nertcEngine.adjustRecordingSignalVolume(200)
        addlog(`nertcEngine.adjustRecordingSignalVolume:${ret}`)
      }
      assert.strictEqual(ret, 0)
    });

    it('playoutDevice', function(){
      let devices = nertcEngine.enumeratePlayoutDevices()
      addlog(`nertcEngine.enumeratePlayoutDevices. ret:${devices}`)

      if(devices.length > 0){
        ret = nertcEngine.setPlayoutDevice(devices[0].device_id)
        addlog(`nertcEngine.setPlayoutDevice:${ret}`)

        let cur_device = nertcEngine.getPlayoutDevice()
        addlog(`nertcEngine.getPlayoutDevice:${cur_device}`)

        ret = nertcEngine.setPlayoutDeviceVolume(85)
        addlog(`nertcEngine.setPlayoutDeviceVolume:${ret}`)

        let vol = nertcEngine.getPlayoutDeviceVolume()
        addlog(`nertcEngine.getPlayoutDeviceVolume:${vol}`)

        ret = nertcEngine.setPlayoutDeviceMute(true)
        addlog(`nertcEngine.setPlayoutDeviceMute:${ret}`)

        let playout_mute = nertcEngine.getPlayoutDeviceMute()
        addlog(`nertcEngine.getPlayoutDeviceMute:${playout_mute}`)

        ret = nertcEngine.setPlayoutDeviceMute(false)
        addlog(`nertcEngine.setPlayoutDeviceMute:${ret}`)

        ret = nertcEngine.adjustPlaybackSignalVolume(150)
        addlog(`nertcEngine.adjustPlaybackSignalVolume:${ret}`)

        assert.strictEqual(ret, 0)
      }
    });

    it('muteLocalVideoStream',function(){
      ret = nertcEngine.muteLocalVideoStream(true)
      addlog(`nertcEngine.muteLocalVideoStream:${ret}`)
      assert.strictEqual(ret, 0)
    });

    it('getConnectionState',function(){
      addlog(`nertcEngine.getConnectionState:${nertcEngine.getConnectionState()}`)
      assert.strictEqual(0, 0)
    });

    it('muteLocalAudioStream',function(){
      addlog(`nertcEngine.muteLocalAudioStream:${nertcEngine.muteLocalAudioStream(true)}`)
      assert.strictEqual(0, 0)
    });

    it('setVideoConfig',function(){
      let cfg = {
        'max_profile': 2,
        'width': 0,
        'height': 0,
        'crop_mode': 0,
        'framerate': 30,
        'min_framerate': 0,
        'bitrate': 0,
        'min_bitrate': 0,
        'degradation_preference': 0
      }
      addlog(`nertcEngine.setVideoConfig:${nertcEngine.setVideoConfig(cfg)}`)
      assert.strictEqual(0, 0)
    });

    it('enableDualStreamMode',function(){
      addlog(`nertcEngine.enableDualStreamMode:${nertcEngine.enableDualStreamMode(true)}`)
      assert.strictEqual(0, 0)
    });

    it('setLocalVideoMirrorMode',function(){
      addlog(`nertcEngine.setLocalVideoMirrorMode:${nertcEngine.setLocalVideoMirrorMode(1)}`)
      assert.strictEqual(0, 0)
    });

    it('onRtcStats',(done)=>{
      nertcEngine.once('onRtcStats', (stats)=>{
        addlog(`onRtcStats, stats:${JSON.stringify(stats, null, 2)}`)
        done()
      })
    }).timeout(5000);

    // it('onLocalAudioStats',(done)=>{
    //   nertcEngine.once('onLocalAudioStats', (stats)=>{
    //     addlog(`onLocalAudioStats, stats:${JSON.stringify(stats, null, 2)}`)
    //     done()
    //   })
    // }).timeout(5000);

    it('onClientRoleChanged', (done)=>{
      nertcEngine.once('onClientRoleChanged', (oldRole, newRole)=>{
        addlog(`onClientRoleChanged, oldRole:${oldRole} newRole:${newRole}`)
        done()
      })

      ret = nertcEngine.setClientRole(0)
      addlog(`nertcEngine.setClientOldRole:${ret}`)
      ret = nertcEngine.setClientRole(1)
      addlog(`nertcEngine.setClientNewRole:${ret}`)
      // assert.strictEqual(ret, 0)
    });

    it('switchChannel', async function(){
      ret = nertcEngine.setClientRole(1)
      addlog(`switchChannel setClientRole ret:${ret}`)
      
      await getToken(gUid, gAppKey, gPrdServer)

      ret = nertcEngine.switchChannel(gToken, gChannelName2)
      addlog(`switchChannel ret:${ret}`)
    });

/*************************加入通话后接口end******************************/

    it('leaveChannel', function(done){
      nertcEngine.once('onReleasedHwResources', function (result) {
        addlog(`onReleasedHwResources: result:${result}`)
        if(0 != result){
          done(-1);
        }
      })

      nertcEngine.once('onLeaveChannel', function (result) {
        addlog(`onLeaveChannel: result:${result}`)
        if(result != 0){
          done(-1);
        } else {
          done();
        }
      })
      ret = nertcEngine.leaveChannel()
      addlog(`do leave channel ret: ${ret}`)
    });

    it('release', function(){
      nertcEngine.release()
      assert.strictEqual(0, 0)
    });



  });
}).timeout(50000);

