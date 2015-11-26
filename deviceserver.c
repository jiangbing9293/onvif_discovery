/******************************************************************************

* Copyright 2010-2013

* FileName:     deviceserver.c

* Desc:  监视网络摄像头设备发现服务器文件

*

*

* Author:          Tom-hongtao.gao

* Date:    2013/05/30

* Notes:  数据结构使用链表

*

* -----------------------------------------------------------------

* Histroy: v1.0   2013/05/30, Tom-hongtao.gao create this file

*

******************************************************************************/

  

/*-------------------------------- Includes ----------------------------------*/

 

#include "uuid32.h" 

#include "soapStub.h"

#include "RemoteDiscoveryBinding.nsmap"

#include "wsddapi.h"

  

/*-------------------- Global Definitions and Declarations -------------------*/

 

 

/*----------------------- Constant / Macro Definitions -----------------------*/

 

 

/*------------------------ Type Declarations ---------------------------------*/

 

 

/*------------------------ Variable Declarations -----------------------------*/

 

 

/*------------------------ Function Prototype --------------------------------*/

 

 

 

/*------------------------ Function Implement --------------------------------*/

  

 

int main(int argc, char **argv) 

{ 

    int m, s;

    struct ip_mreq mcast;

   

    //初始化SOAP执行环境

    struct soap probe_soap;

    soap_init2(&probe_soap, SOAP_IO_UDP|SOAP_IO_FLUSH, SOAP_IO_UDP|SOAP_IO_FLUSH);   

    soap_set_namespaces(&probe_soap, namespaces); 

 

    /*下面的函数是将创建的SOAP与指定的监听端口进行绑定，即通过SOAP创建监听 

        套接字，并将该套接字绑定到指定端口，从而在该端口上监听Web服务请求，

        返回创建的套接字标识 */ 

    if(!soap_valid_socket(soap_bind(&probe_soap, NULL, 3702, 10)))

         {

                   soap_print_fault(&probe_soap, stderr);

                   exit(1);

         }

 

    mcast.imr_multiaddr.s_addr = inet_addr("239.255.255.250");

    mcast.imr_interface.s_addr = htonl(INADDR_ANY);

    if(setsockopt(probe_soap.master, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mcast, sizeof(mcast)) < 0)

    {

        printf("setsockopt error!\n");

        return 0;

    }  

 

 

    //成功绑定之后，便开始监听

    for (;;)

    { 

        //监听直到有连接请求

        s = soap_accept(&probe_soap); 

        if (s < 0) { 

            soap_print_fault(&probe_soap, stderr); 

            exit(-1); 

        } 

        fprintf(stderr, "Socket connection successful: slave socket = %d\n", s); 

 

        /*  连接成功后，便处理并响应请求，下面的函数就是用来处理并响应请求的，

            它封装了所有的处理与响应过程，在该函数中调用了本地所实现的相关Web

            服务方法，它所需要的所有请求信息都在SOAP结构体中。*/

        soap_serve(&probe_soap); 

 

        soap_destroy(&probe_soap);   //服务完成后，便撤销SOAP环境

        soap_end(&probe_soap);       //清除所有资源，关闭套接字

    }

   

    soap_done(&probe_soap);

    return 0; 

} 

 

 

SOAP_FMAC5 int SOAP_FMAC6 SOAP_ENV__Fault(struct soap* soap, char *faultcode, char *faultstring, char *faultactor,

                            struct SOAP_ENV__Detail *detail, struct SOAP_ENV__Code *SOAP_ENV__Code,

                            struct SOAP_ENV__Reason *SOAP_ENV__Reason,

                            char *SOAP_ENV__Node, char *SOAP_ENV__Role,

                            struct SOAP_ENV__Detail *SOAP_ENV__Detail )

{

    printf("%s,%d\n",__FUNCTION__, __LINE__);

    return 0;

}

 

void wsdd_event_Hello(struct soap *soap, unsigned int InstanceId, const char *SequenceId, unsigned int MessageNumber, const char *MessageID, const char *RelatesTo, const char *EndpointReference, const char *Types, const char *Scopes, const char *MatchBy, const char *XAddrs, unsigned int MetadataVersion)

{

    printf("%s,%d\n",__FUNCTION__, __LINE__);

    return 0;

}

 

void wsdd_event_Bye(struct soap *soap, unsigned int InstanceId, const char *SequenceId, unsigned int MessageNumber, const char *MessageID, const char *RelatesTo, const char *EndpointReference, const char *Types, const char *Scopes, const char *MatchBy, const char *XAddrs, unsigned int *MetadataVersion)

{

    printf("%s,%d\n",__FUNCTION__, __LINE__);

    return 0;

}

soap_wsdd_mode wsdd_event_Probe(struct soap *soap, const char *MessageID, const char *ReplyTo, const char *Types, const char *Scopes, const char *MatchBy, struct wsdd__ProbeMatchesType *matches)

{

    printf("%s,%d\n",__FUNCTION__, __LINE__);

    printf("MessageID:%s\n", MessageID);

    printf("ReplyTo:%s\n",ReplyTo);

    printf("Types:%s\n",Types);

    printf("Scopes:%s\n",Scopes);

    printf("MatchBy:%s\n",MatchBy); 

   

    //测试代码

    soap_wsdd_init_ProbeMatches(soap,matches);

    soap_wsdd_add_ProbeMatch(soap,matches,

        "urn:uuid:464A4854-4656-5242-4530-313035394100",

        "ns1:NetworkVideoTransmitter",

        "onvif://www.onvif.org/type/video_encoder onvif://www.onvif.org/type/audio_encoder onvif://www.onvif.org/hardware/IPC-model onvif://www.onvif.org/name/IPC-model",

        NULL,

        "http://192.168.0.243/onvif/device_service",10);

    return 0;

}

 

void wsdd_event_ProbeMatches(struct soap *soap, unsigned int InstanceId, const char *SequenceId, unsigned int MessageNumber, const char *MessageID, const char *RelatesTo, struct wsdd__ProbeMatchesType *matches)

{

    printf("%s,%d\n",__FUNCTION__, __LINE__);

    return 0;

}

soap_wsdd_mode wsdd_event_Resolve(struct soap *soap, const char *MessageID, const char *ReplyTo, const char *EndpointReference, struct wsdd__ResolveMatchType *match)

{

    printf("%s,%d\n",__FUNCTION__, __LINE__);

    return 0;

}

 

void wsdd_event_ResolveMatches(struct soap *soap, unsigned int InstanceId, const char *SequenceId, unsigned int MessageNumber, const char *MessageID, const char *RelatesTo, struct wsdd__ResolveMatchType *match)

{

    printf("%s,%d\n",__FUNCTION__, __LINE__);

    return 0;

}
