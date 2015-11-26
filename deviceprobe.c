/******************************************************************************
* Copyright 2010-2013 A
* FileName:     deviceprobe.c
* Desc:  监视网络摄像头设备发现文件
* Author:          Tom-hongtao.gao
* Date:    2013/05/30
* Notes:  数据结构使用链表
*
* -----------------------------------------------------------------
* Histroy: v1.0   2013/05/30, Tom-hongtao.gao create this file
*          v1.1   2015/11/25, FREELANCER update  
******************************************************************************/
/*-------------------------------- Includes ----------------------------------*/
#include <stdio.h> 
#include <string.h>
#include "soapStub.h"
#include "wsdd.nsmap"

#define MAX_DEVICE 20

/***
 *  device info struct 
 */
typedef struct device {
	char  *dev_uuid;
	char  *dev_server_address;
	char  *username;
	char  *password;
} DeviceInfo;

DeviceInfo g_device_list[MAX_DEVICE];

int main(int argc, char *argv[]) {
    /* 变量声明 */
    struct soap *soap;  //soap环境变量      
    struct wsdd__ProbeType req;   //用于发送消息描述   
    struct wsdd__ProbeType wsdd__Probe;
    struct __wsdd__ProbeMatches resp;
    //struct wsdd__ProbeMatchesType resp; //请求消息的回应
    struct wsdd__ScopesType sScope; //描述查找哪类的Web服务
    struct SOAP_ENV__Header header; //soap消息头描述
    int count = 0;  //获得的设信息备个数
    int result = 0;    //返回值        
	unsigned char macaddr[6];  
	char _HwId[1024];  
	unsigned int Flagrand;

	// Create SessionID randomly  
	srand((int)time(0));  
	Flagrand = rand()%9000 + 8888;   
	macaddr[0] = 0x1;
	macaddr[1] = 0x2; 
	macaddr[2] = 0x3; 
	macaddr[3] = 0x4; 
	macaddr[4] = 0x5; 
	macaddr[5] = 0x6;  
	sprintf(_HwId,"urn:uuid:%ud68a-1dd2-11b2-a105-%02X%02X%02X%02X%02X%02X", Flagrand, macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);
    /************初始化*************/
    soap = soap_new(); //为soap申请变量空间,并初始化
    if ( soap == NULL) {
		return -1;
	}
    soap_set_namespaces(soap, namespaces); //设置soap的namespaces
    soap->recv_timeout = 5; //超过5秒钟没有数据就退出
    soap_default_SOAP_ENV__Header(soap, &header);//将header设置为soap消息    头属性
    header.wsa__MessageID = _HwId;
    header.wsa__To     = "urn:schemas-xmlsoap-org:ws:2005:04:discovery";
    header.wsa__Action = "http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe";
    soap->header = &header; //设置soap头消息的ID
    //设置soap消息的请求服务属性
    soap_default_wsdd__ScopesType(soap, &sScope);
    sScope.__item = "";     
	//sScope.__item = "onvif://www.onvif.org";
    soap_default_wsdd__ProbeType(soap, &req);
    req.Scopes = &sScope;
    //req.Types = "tdn:NetworkVideoTransmitter";
    req.Types = "tds:Device";
    //调用gSoap接口
    //soap_wsdd_Probe
    result  = soap_send___wsdd__Probe(soap, "soap.udp://239.255.255.250:3702", NULL, &req);
    printf("%s: %d, send probe request success!\n",__FUNCTION__, __LINE__);
    if ( result == -1 ) {
        printf ( "soap error: %d, %s, %s\n", soap->error, *soap_faultcode(soap), *soap_faultstring(soap));
        result = soap->error;
    } else {
        do {
            printf("%s: %d, begin receive probematch... \n",__FUNCTION__, __LINE__);
            printf("count=%d \n",count);
            //接收ProbeMatches,成功返回0,否则-1
            result = soap_recv___wsdd__ProbeMatches(soap, &resp);
            printf(" --soap_recv___wsdd__ProbeMatches() result=%d \n",result);
            if (result==-1) {
                printf("Find %d devices!\n", count);
                break;
            } else {                     
                //读取服务端回应的Probematch消息
				if ( resp.wsdd__ProbeMatches){
					/***
					printf("soap_recv___wsdd__Probe:  __sizeProbeMatch=%d\r\n",resp.wsdd__ProbeMatches->__sizeProbeMatch);
					printf("Target EP Address : %s\r\n",      resp.wsdd__ProbeMatches->ProbeMatch->wsa__EndpointReference.Address);
					printf("Target Type : %s\r\n",            resp.wsdd__ProbeMatches->ProbeMatch->Types);
					printf("Target Service Address : %s\r\n", resp.wsdd__ProbeMatches->ProbeMatch->XAddrs);
					printf("Target Metadata Version : %d\r\n",resp.wsdd__ProbeMatches->ProbeMatch->MetadataVersion);
					printf("Target Scopes Address : %s\r\n",  resp.wsdd__ProbeMatches->ProbeMatch->Scopes->__item);
					**/
					DeviceInfo device;
					device.dev_uuid = resp.wsdd__ProbeMatches->ProbeMatch->wsa__EndpointReference.Address;
					device.dev_server_address =  resp.wsdd__ProbeMatches->ProbeMatch->XAddrs;
					char duplicate = 0;
					int index = 0;
					while ( index < MAX_DEVICE) {
						char *uuid = g_device_list[ index].dev_uuid;
						if ( uuid != NULL && strcmp( uuid, device.dev_uuid) == 0 ) {
							duplicate = 1;
							break;
						}
						++index;
					}
					if ( duplicate == 0) {
						printf ( " find device no :%d  \n" , ( count +1));
						printf ( " uuid : %s  \n" , device.dev_uuid);
						printf ( " server address : %s  \n" , device.dev_server_address);
						g_device_list[ count] = device;
						count += 1;
					}
				}
            }
        } while ( 1);
    }
    //清除soap
    soap_end(soap); // clean up and remove deserialized data
    soap_free(soap);//detach and free runtime context
    soap_done(soap); // detach context (last use and no longer in scope)
    return result;
}
