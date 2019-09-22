#include <tchar.h>
#include <iostream>
#include <curl/curl.h>


using namespace std;


/**
	 * 一旦curl接收到数据，就会调用此回调函数
	 * buffer:数据缓冲区指针
	 * size:调试阶段总是发现为1
	 * nmemb:(memory block)代表此次接受的内存块的长度
	 * userp:用户自定义的一个参数
*/
bool getUrl()
{
	CURL* curl;
	CURLcode res;
	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, "Content-Type:application/x-www-form-urlencoded;charset=UTF-8");
	curl = curl_easy_init();    // 初始化
	if (curl)
	{
		//curl_easy_setopt(curl, CURLOPT_PROXY, "10.99.60.201:8080");// 代理
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);// 改协议头
		curl_easy_setopt(curl, CURLOPT_URL, "http://www.baidu.com");
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp); //将返回的http头输出到fp指向的文件
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, fp); //将返回的html主体数据输出到fp指向的文件
		res = curl_easy_perform(curl);   // 执行
		if (res != 0) {

			curl_slist_free_all(headers);
			curl_easy_cleanup(curl);
		}
		return true;
	}
}
bool postUrl()
{
	CURL* curl;
	CURLcode res;
	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "/tmp/cookie.txt"); // 指定cookie文件
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "&logintype=uid&u=xieyan&psw=xxx86");    // 指定post内容
		//curl_easy_setopt(curl, CURLOPT_PROXY, "10.99.60.201:8080");
		curl_easy_setopt(curl, CURLOPT_URL, " http://mail.sina.com.cn/cgi-bin/login.cgi ");   // 指定url
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
	return true;
}
int main(void)
{
	getUrl();
	postUrl();
}