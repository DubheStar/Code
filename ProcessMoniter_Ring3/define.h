#pragma once
#include <stdio.h>
#define FILE_DEVICE_EVENT  0x8000

//CTL_CODE�깲�ĸ�������32λ,�ֳ���4���֣��洢�豸���ͣ������룬����Ȩ�ޣ��������ܣ�
//���0��1��λ����ȷ���������������I/O���ļ�ϵͳ���ݻ������������ݴ��ݷ�ʽ������ľ���METHOD_BUFFERED��
//Function ���Զ����IO�����롣�Լ�����ʱȡ0x800��0xFFF����Ϊ0x0��0x7FF��΢�����ġ�
//Method �����ݵĲ���ģʽ��
//METHOD_BUFFERED��������ģʽ
//METHOD_IN_DIRECT��ֱ��дģʽ
//METHOD_OUT_DIRECT��ֱ�Ӷ�ģʽ
//METHOD_NEITHER ��Neitherģʽ
//Access������Ȩ�ޣ���ȡֵ�У�
//FILE_ANY_ACCESS�������û�ӵ�����е�Ȩ��
//FILE_READ_DATA������Ȩ��Ϊֻ��
//FILE_WRITE_DATA������Ȩ��Ϊ��д
//Ҳ���� FILE_WRITE_DATA | FILE_READ_DATA������Ȩ��Ϊ�ɶ���д������û�ﵽFILE_ANY_ACCESS��Ȩ�ޡ�
#define CTL_CODE( DeviceType, Function, Method, Access ) (((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method))


#define EVENT_IOCTL(index) CTL_CODE(FILE_DEVICE_EVENT, index, METHOD_BUFFERED, FILE_READ_DATA)

#define IOCTL_PASSEVENT    CTL_CODE(FILE_DEVICE_EVENT, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PASSBUF	   CTL_CODE(FILE_DEVICE_EVENT, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_UNPASSEVENT  CTL_CODE(FILE_DEVICE_EVENT, 0x803, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PASSEVSTRUCT CTL_CODE(FILE_DEVICE_EVENT, 0x804, METHOD_BUFFERED, FILE_ANY_ACCESS)

typedef struct
{
	BOOL SHOWTHREAD;
	BOOL ONLYSHOWREMOTETHREAD;
	BOOL SHOWEXITPROCESS;
	BOOL SHOWEXITTHREAD;
	BOOL SHOWIMAGE;
}CHECKLIST, * PCHECKLIST;
