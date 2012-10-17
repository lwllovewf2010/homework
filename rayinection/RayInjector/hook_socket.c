//
//  hook_socket.c
//  RayInjector
//
//  Created by  on 7/3/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include <dlfcn.h>
#include <util.h>

bool bPayloadPrefix = false;
bool bDumpAES = false;

typedef struct
{
    unsigned int payload_size;
    unsigned short payload_type;
    unsigned short unknown;
    unsigned int ntp_seconds;
    unsigned int ntp_fraction;
} guessed_airplay_header;

static guessed_airplay_header header;

typedef ssize_t (*TYPE_read) (int fd,void* buf,size_t count);
TYPE_read orig_read = NULL;
ssize_t read(int fd,void *buf,size_t count)
{
    static FILE *fout = NULL;
    static unsigned long total = 0;
    static unsigned long payloadCount = 0;
    static bool bStreamStart = false;
    static bool bFirstPayload = true;
    if (orig_read == NULL)
    {
        orig_read = dlsym(RTLD_NEXT, "read");
        printf("orig_read=%x\n",orig_read);
    }
    
    ssize_t ret = orig_read(fd,buf,count);
    total += ret;
    //printf("read %ld bytes\n",ret);
    
    if (bPayloadPrefix && bFirstPayload)
    {
        bPayloadPrefix = false;
        bFirstPayload = false;
        char str[256];
        sprintf(str,"recv payload prefix[%d]",payloadCount++);
        dump16Bytes((unsigned char*)buf,str);
        
    }
    if (ret == 128)
    {
        bStreamStart = true;
        fclose(fout);
        memcpy(&header,buf,16);
        /*
        printf("guessed header:payload_size=%d,type=%d,unknown=%04X,timestamp_sec=%d,fraction=%d\n",
               header.payload_size,
               header.payload_type,
               header.unknown,
               header.ntp_fraction,
               header.ntp_seconds);
         */
        if (header.payload_type == 0x00)
            bPayloadPrefix = true;
    }
    
    return ret;
}

typedef ssize_t (*TYPE_recvfrom) (int socket,void* buffer,size_t buflen,int flags,void* addr,unsigned int *pAddrLen);
TYPE_recvfrom orig_recvfrom = NULL;
ssize_t recvfrom(int socket,void* buffer,size_t buflen,int flags,void* addr,unsigned int *pAddrLen)
{
    if (orig_recvfrom == NULL)
    {
        orig_recvfrom = dlsym(RTLD_NEXT, "recvfrom");
    }
    
    ssize_t ret = orig_recvfrom(socket,buffer,buflen,flags,addr,pAddrLen);
    //printf("recvfrom %ld bytes\n",ret);
    //dump16Bytes((unsigned char*)buffer, "prefix:");
    return ret;
}
typedef ssize_t (*TYPE_recv) (int,void*,size_t,int);
TYPE_recv orig_recv = NULL;

ssize_t recv(int fd,void* buf,size_t len,int flag)
{
    int i = 0;
    if (orig_recv == NULL)
    {
        orig_recv = (TYPE_recv)dlsym(RTLD_NEXT, "recv");
        printf("hooked recv ptr:%x\n",orig_recv);
        
    }
    
    ssize_t ret = 0;
    ret = orig_recv(fd,buf,len,flag);
    //printf("ray:recv %lu bytes\n",ret);
    /*
    if (ret >= 16)
        dump16Bytes(ptr, "recv prefix");
     */
    return ret;
}