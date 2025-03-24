#include <iostream>
#include <pjsua2.hpp>

using namespace pj;

// SIP通话事件处理类
class MyCall : public Call {
  public:
    MyCall(Account &acc, int call_id = PJSUA_INVALID_ID) : Call(acc, call_id) {}

    // 通话状态回调
    virtual void onCallState(OnCallStateParam &prm) {
        CallInfo ci = getInfo();
        std::cout << "Call state: " << ci.stateText << std::endl;

        if(ci.state == PJSIP_INV_STATE_CONFIRMED) {
            std::cout << "通话已建立，开始媒体传输" << std::endl;
        }
    }

    // 媒体状态回调
    virtual void onAudioMediaState(OnAudioMediaStateParam &prm) {
        AudioMedia *aud_med = nullptr;

        // 获取音频媒体流
        for(unsigned i = 0; i < prm.medias.size(); i++) {
            if(prm.medias[i].type == PJMEDIA_TYPE_AUDIO) {
                aud_med = dynamic_cast<AudioMedia *>(prm.medias[i].media.get());
                break;
            }
        }

        if(aud_med) {
            // 连接音频设备（需配置音频设备）
            AudioMedia &play_dev = Endpoint::instance().audDevManager().getPlaybackDevMedia();
            aud_med->startTransmit(play_dev);
        }
    }
};

// SIP账号管理类
class MyAccount : public Account {
  public:
    virtual void onIncomingCall(OnIncomingCallParam &iprm) {
        MyCall *call = new MyCall(*this, iprm.callId);
        CallOpParam op;
        op.statusCode = PJSIP_SC_OK;
        call->answer(op);
    }
};

int main() {
    Endpoint ep;  // SIP协议栈核心实例

    try {
        // 初始化协议栈
        ep.libCreate();
        EpConfig ep_cfg;
        ep.libInit(ep_cfg);

        // 配置传输层（UDP）
        TransportConfig tcfg;
        tcfg.port = 5060;  // 标准SIP端口
        ep.transportCreate(PJSIP_TRANSPORT_UDP, tcfg);

        // 启动服务
        ep.libStart();
        std::cout << "SIP协议栈已启动" << std::endl;

        // 注册SIP账号
        MyAccount acc;
        AccountConfig acc_cfg;
        acc_cfg.idUri = "sip:1001@example.com";  // SIP URI
        acc_cfg.regConfig.registrarUri = "sip:example.com";
        acc_cfg.sipConfig.authCreds.push_back(AuthCredInfo("digest", "*", "1001", 0, "1234"));
        acc.create(acc_cfg);

        // 发起呼叫
        Call *call = new MyCall(acc);
        CallOpParam prm(true);
        call->makeCall("sip:1002@example.com", prm);

        // 保持主线程运行
        while(true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

    } catch(Error &err) {
        std::cout << "异常: " << err.info() << std::endl;
    }
    return 0;
}