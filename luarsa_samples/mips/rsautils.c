/*******************************************************************************************
 *
 *  Copyright (C) Ravishanker Kusuma / ecofast.  All Rights Reserved.
 *
 *  File: rsautils.c 
 *  Date: 2017/12/01
 *  Desc: RSA Encryption & Decryption utils with OpenSSL in C
 *
 *  Thks: http://hayageek.com/rsa-encryption-decryption-openssl-c/
 *
 *  Compilation Command: gcc rsautils.c -fPIC -shared -lssl -lcrypto -o librsa.so
 *******************************************************************************************/

#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
 
const int padding = RSA_PKCS1_OAEP_PADDING;
 
int public_encrypt(unsigned char* data, int data_len, unsigned char* key, unsigned char* encrypted)
{
    int ret = -1;
    BIO* keybio = BIO_new_mem_buf(key, -1);
    if (keybio != NULL)
    {
        RSA* rsa = NULL;
        rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
        if (rsa != NULL)
        {
            ret = RSA_public_encrypt(data_len, data, encrypted, rsa, padding);
            RSA_free(rsa);
        }
        BIO_free_all(keybio);
    }
    return ret;
}

int private_decrypt(unsigned char* enc_data, int data_len, unsigned char* key, unsigned char* decrypted)
{
    int ret = -1;
    BIO* keybio = BIO_new_mem_buf(key, -1);
    if (keybio != NULL)
    {
        RSA* rsa = NULL;
        rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
        if (rsa != NULL)
        {
            ret = RSA_private_decrypt(data_len, enc_data, decrypted, rsa, padding);
            RSA_free(rsa);
        }
        BIO_free_all(keybio);
    }
    return ret;
}