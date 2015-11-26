# onvif_discovery
onvif设备搜索
整合网上的gsoap代码，备份供自己后续参考学习
1、局域网设备搜索
2、搜索设备过滤，去掉重复信息
设备发现 gsop步骤：
1、wget http://www.onvif.org/onvif/ver10/network/wsdl/remotediscovery.wsdl 下载wsdl文件
2、wsdl2h -s -c  -t typemap.dat -o remotediscovery.h  remotediscovery.wsdl 生成头文件
3、soapcpp2  -c -L -x -I../gsoap-2.8/gsoap/import  remotediscovery.h 生成相关处理文件
4、从gsoap拷贝相关处理文件至当前文件夹stdsoap2.h、stdsoap2.c、threads.h、threads.c、wsaapi.h、wsaapi.c、wsddapi.c
