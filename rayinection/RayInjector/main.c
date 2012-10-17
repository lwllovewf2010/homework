//
//  main.cpp
//  RayInjector
//
//  Created by  on 6/29/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

//#include <sys/socket.h>
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <util.h>
#include <CommonCrypto/CommonDigest.h>
#include <CoreFoundation/CoreFoundation.h>
//#include <VideoDecodeAcceleration/VDADecoder.h>
unsigned char aeskey[16];
unsigned char xor_key_cipher[16];
unsigned char xor_key_plain[16];

static int testvar = 0;
static FILE *fRecv = NULL;
FILE *fDecoderInput = NULL;
FILE *fAESCipher = NULL;
FILE *fAESCipherXorKey = NULL;

/* 
__attribute__((constructor))
static void dylibInit()
{
    printf("dylib init: var=%d\n",testvar++);
    return;
    if (fRecv == NULL)
    {
        printf("inject dylib init!\n");
        fRecv = fopen("/Users/ransom/temp/recv.log","wb");
        fDecoderInput = fopen("/Users/ransom/temp/decoder_input.log","wb");
        fAESCipher = fopen("/Users/ransom/temp/aes_enc_result.log","wb");
        fAESCipherXorKey = fopen("/Users/ransom/temp/xor_result.log","wb");
    }
    
}

__attribute__((destructor))
static void dylibUnInit()
{
    printf("dylib uninit: var=%d\n",testvar++);
    return;
    if (fRecv)
    {
        printf("inject dylib uninit!\n"); 
        fclose(fRecv);
        fclose(fDecoderInput);
        fclose(fAESCipher);
        fclose(fAESCipherXorKey);
        fRecv = NULL;
        fDecoderInput = NULL;
        fAESCipher = NULL;
        fAESCipherXorKey = NULL;
    }
}
*/



typedef FILE* (*TYPE_fopen) (const char*,const char*);
TYPE_fopen original_fopen = NULL;
FILE * fopenXX(const char * filename, const char * mode)
{
    if (!original_fopen)
        original_fopen = (TYPE_fopen)dlsym(RTLD_NEXT, "fopen");
    
    printf("== fopen: {%s,%s} ==,count=%d\n", filename, mode,testvar++);
    FILE* f = original_fopen(filename, mode);
    return f;
}




typedef unsigned char* (*TYPE_CC_MD5) (const void *data, CC_LONG len,unsigned char *md);
TYPE_CC_MD5 original_CC_MD5 = NULL;
unsigned char* CC_MD5(const void *data,CC_LONG len,unsigned char *md)
{
    if (original_CC_MD5 == NULL)
        original_CC_MD5 = (TYPE_CC_MD5)dlsym(RTLD_NEXT, "CC_MD5");
    
    unsigned char* ret = original_CC_MD5(data,len,md);
    printf("CC_MD5: len=%d",len);
    return ret;
}

typedef struct 
{
    int64_t pts;
    int64_t dts;
    unsigned char *data;
    int size;
    int streamIndex;
    int flag;
    void *side_data;
    int side_data_elems;
    int duration;
    void *ptr1;
    void *ptr2;
    int64_t pos;
    int64_t duration2;
}AVPacket;

typedef int (*TYPE_avcodec_decode_video2) (void* avctx,void* picture,int *got_pic,const void* avpkt);
TYPE_avcodec_decode_video2 original_avcodec_decode2 = NULL;
int avcodec_decode_video2(void* avctx,void* picture,int *got_pic,const void* avpkt)
{
    static FILE *fout = NULL;
    static unsigned total = 0;
    
    if (original_avcodec_decode2 == NULL)
    {
        original_avcodec_decode2 = (TYPE_avcodec_decode_video2)dlsym(RTLD_NEXT, "avcodec_decode_video2");
        if (original_avcodec_decode2 == NULL)
        {
            void *lib = dlopen("/Applications/AirServer.app/Contents/Frameworks/libavcodec_x86_64.dylib",RTLD_NOW);
            printf("load avcodec.dylib:%x,err=%s\n",lib,dlerror());
            original_avcodec_decode2 = dlsym(lib,"avcodec_decode_video2");
        }
        printf("hooked avcodec_decode2:%x\n",original_avcodec_decode2);
        fout = fopen("decoder_input.log","wb");
    }
    
    int ret = original_avcodec_decode2(avctx,picture,got_pic,avpkt);
    AVPacket *pPkt = (AVPacket*)avpkt;
    printf("decode video:size=%d,index=%d\n",pPkt->size,pPkt->streamIndex);
    //dump16Bytes(pPkt->data, "prefix:");
    
    fwrite(pPkt->data,1,pPkt->size,fout);
    total += pPkt->size;
    if (total > 1024)
    {
        fflush(fout);
        total = 0;
    }
    return ret;
}

