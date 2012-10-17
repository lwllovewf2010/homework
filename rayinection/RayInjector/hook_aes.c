//
//  hook_aes.c
//  RayInjector
//
//  Created by  on 7/3/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include <CommonCrypto/CommonCryptor.h>
#include <openssl/aes.h>
#include <dlfcn.h>


extern bool bDumpAES;
unsigned long dumpCount = 0;

static unsigned char aeskey[16];




//========
typedef unsigned int u32;
typedef char s8;


static s8 ConvertDWordToByte( u32 *Data, u32 Offset ) {
    
    u32 tmp, off, bs;
    
    
    off = Offset / 4;
    bs = Offset % 4;
    if( bs ) {
        
        tmp = *(Data + off);
        tmp = ((tmp >> (bs * 8)) & 0xFF);
        return tmp;
    }
    
    tmp = ((*(Data + off)) & 0xFF);
    return tmp;
}


static void DumpData( u32 *Data, u32 Length, u32 BaseAddr ) {
    
    u32 i, j;
    u32 c;
    
    
    printf( "\n\n== Dump Memory Start ==\n\n" );
    printf( " Address | 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F|   ASCII DATA   \n" );
    printf( "---------------------------------------------------------------------------\n" );
    
    
    for( i = 0 ; i <= Length ; i++ ) {
        
        
        if( !(i % 16) ) {
            
            
            if( (i > 15) ) {
                
                for( j = i - 16 ; j < i ; j++ ) {
                    
                    c = ConvertDWordToByte( Data, j );
                    if( ((c >= '!') && (c <= '~')) ) {
                        
                        printf( "%c", c );
                    }
                    else {
                        
                        printf( "." );
                    }
                }
            }
            
            
            if( i ) {
                
                printf( "\n" );
            }
            
            
            if( i == Length ) {
                
                break;
            }
            
            
            printf( "%8.8X : ", i + BaseAddr );
        }
        
        
        printf( "%2.2X ", ConvertDWordToByte( Data, i ) & 0xFF );
    }
    
    
    printf( "---------------------------------------------------------------------------\n" );
    printf( "\n== Dump Memory End ==\n\n" );
}
//=======



typedef void (*TYPE_AES_cbc_encrypt) (const unsigned char*,
                                      unsigned char*,
                                      const unsigned long,
                                      const AES_KEY*,
                                      unsigned char*,
                                      const int);


TYPE_AES_cbc_encrypt original_AES_cbc_encrypt = NULL;


void AES_cbc_encrypt(const unsigned char *in,
                     unsigned char *out,
                     const unsigned long length,
                     const AES_KEY *key,
                     unsigned char *ivec,
                     const int enc)
{
    if (original_AES_cbc_encrypt == NULL)
        original_AES_cbc_encrypt = (TYPE_AES_cbc_encrypt)dlsym(RTLD_NEXT, "AES_cbc_encrypt");
    
    original_AES_cbc_encrypt(in,out,length,key,ivec,enc);
    printf("AES_cbc_encrypt:length=%lu,enc=%d\n",length,enc);
    
}



typedef CCCryptorStatus (*TYPE_CCCryptorCreate) (CCOperation op,
                                                 CCAlgorithm alg,
                                                 CCOptions opt,
                                                 const void *key,
                                                 size_t keyLen,
                                                 const void *iv,
                                                 CCCryptorRef *cryptorRef);


TYPE_CCCryptorCreate original_CCCryptorCreate = NULL;


CCCryptorStatus CCCryptorCreate(CCOperation op,
                                CCAlgorithm alg,
                                CCOptions opt,
                                const void *key,
                                size_t keyLen,
                                const void *iv,
                                CCCryptorRef *cryptorRef)
{
    int i = 0;
    if (original_CCCryptorCreate == NULL)
        original_CCCryptorCreate = (TYPE_CCCryptorCreate)dlsym(RTLD_NEXT, "CCCryptorCreate");
    
    CCCryptorStatus status = original_CCCryptorCreate(op,alg,opt,key,keyLen,iv,cryptorRef);
    
    printf("CCCryptorCreate:op=%d,algo=%d,keyLen=%ld,context=%x\n",op,alg,keyLen,*cryptorRef);
    printf("CCCryptorCreate:key=\n");
    
    DumpData( key, keyLen, 0x0 );
    
#if 0
    for (i=0;i<keyLen;i++)
        printf("%02X ",*(((unsigned char*) key)+i));
    printf("\n");
#endif
    
    memcpy(aeskey,key,16);
    return status;
}


typedef CCCryptorStatus (*TYPE_CCCryptorUpdate) (CCCryptorRef ref,
                                                 const void *in,
                                                 size_t inLen,
                                                 void *out,
                                                 size_t outAvailable,
                                                 size_t *outMoved);
TYPE_CCCryptorUpdate original_CCCryptorUpdate = NULL;


CCCryptorStatus CCCryptorUpdate(CCCryptorRef ref,
                                const void *in,
                                size_t inLen,
                                void *out,
                                size_t outAvailable,
                                size_t *outMoved)
{
    static FILE *fAESOut = NULL;
    static FILE *fAESOut_xor_key = NULL;
    static FILE *fAESOut_xor_input = NULL;
    
    static unsigned long total = 0;
    static unsigned char xor_result[16];
    static bool bFirstCalled = true;
    
    if (original_CCCryptorUpdate == NULL)
        original_CCCryptorUpdate = (TYPE_CCCryptorUpdate)dlsym(RTLD_NEXT,"CCCryptorUpdate");
    
    CCCryptorStatus status = original_CCCryptorUpdate(ref,in,inLen,out,outAvailable,outMoved);
    //printf("CCCryptorUpdate:ref=%x,inLen=%ld,outAvailable=%ld,outMoved=%ld\n",ref,inLen,outAvailable,*outMoved);
    
    
    printf( "CCCryptorUpdateHOOK: IN\n" );
    DumpData( in, inLen, 0x0 );
    
    printf( "CCCryptorUpdateHOOK: OUT\n" );
    DumpData( out, *outMoved, 0x0 );
    
    
    unsigned char* myin = (unsigned char*)in;
    unsigned char* myout = (unsigned char*)out;
    int i = 0;

    if (bFirstCalled)
    {
        bFirstCalled = false;
        total += 16;
        printf("dumpAES[%d],total=%d\n",dumpCount++,total);
        dump16Bytes(aeskey,"aes decrypt key");
        dump16Bytes(myin,"aes decrypt input");
        dump16Bytes(myout,"aes decrypt output");
        
    }
    
    return status;
}


/*
 CCCryptorStatus
 CCCrypt(CCOperation op, CCAlgorithm alg, CCOptions options, const void *key, size_t keyLength,
 const void *iv, const void *dataIn, size_t dataInLength, void *dataOut, size_t dataOutAvailable,
 size_t *dataOutMoved);
*/

typedef CCCryptorStatus (*TYPE_CCCrypt) (CCOperation op, CCAlgorithm alg, CCOptions options, const void *key, size_t keyLength,
                                         const void *iv, const void *dataIn, size_t dataInLength, void *dataOut, size_t dataOutAvailable,
                                         size_t *dataOutMoved);
TYPE_CCCrypt original_CCCrypt = NULL;
CCCryptorStatus CCCrypt(CCOperation op, CCAlgorithm alg, CCOptions options, const void *key, size_t keyLength,
                        const void *iv, const void *dataIn, size_t dataInLength, void *dataOut, size_t dataOutAvailable,
                        size_t *dataOutMoved)
{
    if (original_CCCrypt == NULL)
    {
        original_CCCrypt = dlsym(RTLD_NEXT, "CCCrypt");
        printf("hooked CCCrypt:%x\n",original_CCCrypt);
    }
    
    CCCryptorStatus ret = original_CCCrypt(op,alg,options,key,keyLength,iv,dataIn,dataInLength,dataOut,dataOutAvailable,dataOutMoved);
    
    printf("CCCrypt:op=%d,alg=%d,keyLen=%d,in_len=%d,out_len=%d\n",op,alg,keyLength,dataInLength,*dataOutMoved);
    
#if 0
    int i=0;
    unsigned char* in = (unsigned char*)dataIn;
    unsigned char* out = (unsigned char*)dataOut;
#endif
    
    printf("in:\n");
    DumpData( dataIn, dataInLength, 0x0 );
#if 0
    for (i=0;i<dataInLength;i++)
        printf("%02X ",in[i]);
    printf("\n");
#endif
    
    
    printf("out:\n");
    DumpData( dataOut, *dataOutMoved, 0x0 );
#if 0
    for (i=0;i<*dataOutMoved;i++)
        printf("%02X ",out[i]);
    printf("\n");
#endif
    
    return ret;
}
