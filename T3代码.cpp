//-----------------------------------------@xcmod6---------------------------------------------
static char BQAQSQEQ[128] = {"YSCNxOJERQnDXGvq9BwsFu7/Pb6+Vr4ZKco0fHI2Lmza18AMpyUT5kijgWl3hedt"};
#include "T3/T3验证.h"
static char s[64];
static bool isLogin = false;
//-----------------------------------------@xcmod6---------------------------------------------
//前端
//下面参数自己填
char *appkey = "ed707cfbf2c5590bc12a640bbf5e7b5d";
char *登录调用码 = "CC1926EB25F68FAE";
char *公告调用码 = "9611EF7A72D381BB";
char *解绑调用码 = "8824C26AFD3D3DD8";
/*✨后台需要配置的东西✨
程序传输配置:
全局数据加密：开启
加解密类型：Base64自定义编码集
请求值加密：开启
返回值加密：开启
时间戳效验：开启
时间戳效验增强：关闭
时间戳通道：系统时间
签名效验：请求签名
返回值格式：文本
JSON返回时间戳：关闭
JSON_code类型：int
接口配置：
单码卡密登录：http 状态开启 安全传输开启
解绑或换绑机器码：http 状态开启 安全传输关闭
获取程序公告：http 状态开启 安全传输关闭
*/
char *提示 = "";
static bool 错误提示 = false;
char *公告 = httppost("w.t3data.net",公告调用码, "");
std::string Login(const char *user_key) {
    if (!g_App)
        return "内部错误";
    auto activity = g_App->activity;
    if (!activity)
        return "内部错误";

    auto vm = activity->vm;
    if (!vm)
        return "内部错误"; 

    auto object = activity->clazz;
    if (!object)
        return "内部错误";

    JNIEnv *env;
    vm->AttachCurrentThread(&env, 0);
    std::string hwid = user_key;
    
    hwid += GetAndroidID(env, object);
    hwid += GetDeviceModel(env);
    hwid += GetDeviceBrand(env);
    std::string UUID = getHEX(加64(GetAndroidID(env, object)));
    vm->DetachCurrentThread();
    std::string errMsg;
    
    //时间戳效验
    time_t ti = time(NULL);
    static char sj[13];
    获取时间戳(sj);
    std::string sjc = getHEX(加64(sj));
    
    //单码签名校验
    char sign2[256];
    sprintf(sign2, "kami=%s&imei=%s&t=%s&%s", getHEX(加64(user_key)), UUID.c_str(), sjc.c_str(), appkey);
    char sign2md5[32] = { 0 };
    获取MD5(sign2, sign2md5);
    std::string sign22 = getHEX(加64(sign2md5));
    
    char 整[4096];
    sprintf(整,"kami=%s&imei=%s&t=%s&s=%s", getHEX(加64(user_key)), UUID.c_str(), sjc.c_str(), sign22.c_str());
    char *内容 = NULL;
    char *数据 = httppost("w.t3data.net",登录调用码, 整);
    内容 = 解64(数据);
    if (strncmp(内容, "登录成功:200;", 12) == 0)
	{
		char *卡密ID = strstrstr(内容, "卡密ID:", ";");
		到期时间 = strstrstr(内容, "到期时间:", ";");
		char *到期时间 = strstrstr(内容, "当前时间戳:", ";");
		char *当前时间 = strstrstr(内容, "当前时间:", ";");
		char *效验密钥 = strstrstr(内容, "效验密钥", ";");
        //获取时间戳(sj);
        long vs1 = (long)atoll(sj);
        long vs2 = (long)atoll(当前时间);
        if ((vs1 - vs2) < 18)
        {
     	    isLogin = true;
        }
        else
        {
            错误提示 = true;
            提示 = "时间戳效验失败";
        }
    }
    else
    {
    错误提示 = true;
    提示 = 内容;
    }
    return "OK";
}
std::string Unbind(const char *user_key) {
    char 解绑整[256];
    sprintf(解绑整,"kami=%s", user_key);
    char *解绑数据 = httppost("w.t3data.net",解绑调用码, 解绑整);
    错误提示 = true;
    提示 = 解绑数据;
    return "OK";
}
//-----------------------------------------@xcmod6---------------------------------------------
//后端
static std::string err;
if (!isLogin) {
   ImGui::PushItemWidth(-1);
   ImGui::InputText("##key", s, sizeof s);
   // auto paste
   auto key = getClipboardText();
   strncpy(s, key.c_str(), sizeof s);
   // auto login
   ImGui::PopItemWidth();
                
   ImGui::PushItemWidth(-1);
   if (ImGui::Button("登录", ImVec2(ImGui::GetContentRegionAvailWidth(), 0))) { Login(s); };
   ImGui::PopItemWidth();
   if (ImGui::Button("解绑", ImVec2(ImGui::GetContentRegionAvailWidth(), 0))) { Unbind(s); }
   // ImGui::PopItemWidth();
   if (错误提示) {
   ImGui::Text("提示:%s", 提示);
   }
   ImGui::Text("公告:%s", 公告);
   ImGui::PopItemWidth();
} else {
//-----------------------------------------@xcmod6---------------------------------------------
ImGui::Text("到期时间:%s", 到期时间);