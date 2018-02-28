local ffi = require('ffi')
local rsa = ffi.load('./librsa.so')
local basexx = require('basexx')

local _M = {}

ffi.cdef[[
int public_encrypt(unsigned char * data,int data_len,unsigned char * key, unsigned char *encrypted);
int private_decrypt(unsigned char * enc_data,int data_len,unsigned char * key, unsigned char *decrypted);

int private_encrypt(unsigned char * data,int data_len,unsigned char * key, unsigned char *encrypted);
int public_decrypt(unsigned char * enc_data,int data_len,unsigned char * key, unsigned char *decrypted);
]]

local RSA_PUBLIC_KEY = [[-----BEGIN PUBLIC KEY-----
MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQC3bTBJNQJjY6u7Y5b2eOWws0yW
CGuWPm6MGOSVan65wCrJa5p3q3sodQUDVPotjsknjLlje9E1F7Bx94ZuqTwkvAr6
ieLkgbbeqTCzeJ0AryUXiF3auxFSPdpBoD6nxtEeN8bZwfa/IYzdKyKlbhiQbUMN
qWgmxiPVwupwAML7RQIDAQAB
-----END PUBLIC KEY-----]]

local RSA_PRIV_KEY = [[-----BEGIN RSA PRIVATE KEY-----
MIICXAIBAAKBgQC3bTBJNQJjY6u7Y5b2eOWws0yWCGuWPm6MGOSVan65wCrJa5p3
q3sodQUDVPotjsknjLlje9E1F7Bx94ZuqTwkvAr6ieLkgbbeqTCzeJ0AryUXiF3a
uxFSPdpBoD6nxtEeN8bZwfa/IYzdKyKlbhiQbUMNqWgmxiPVwupwAML7RQIDAQAB
AoGAc4NXvUKc1mqWY9Q75cwNGlJQEMwMtPlsNN4YVeBTHjdeuqoBBQwA62GGXqrN
QpOBKl79ASGghob8n0j6aAY70PQqHSU4c06c7UlkeEvxJKlyUTO2UgnjjIHb2flR
uW8y3xmjpXAfwe50eAVMNhCon7DBc+XMF/paSVwiG8V+GAECQQDvosVLImUkIXGf
I2AJ2iSOUF8W1UZ5Ru68E8hJoVkNehk14RUFzTkwhoPHYDseZyEhSunFQbXCotlL
Ar5+O+lBAkEAw/PJXvi3S557ihDjYjKnj/2XtIa1GwBJxOliVM4eVjfRX15OXPR2
6shID4ZNWfkWN3fjVm4CnUS41+bzHNctBQJAGCeiF3a6FzA/0bixH40bjjTPwO9y
kRrzSYX89F8NKOybyfCMO+95ykhk1B4BF4lxr3drpPSAq8Paf1MhfHvxgQJBAJUB
0WNy5o+OWItJBGAr/Ne2E6KnvRhnQ7GFd8zdYJxXndNTt2tgSv2Gh6WmjzOYApjz
heC3jy1gkN89NCn+RrECQBTvoqFHfyAlwOGC9ulcAcQDqj/EgCRVkVe1IsQZenAe
rKCWlUaeIKeVkRz/wzb1zy9AVsPC7Zbnf4nrOxJ23mI=
-----END RSA PRIVATE KEY-----]]

function _M.rsa_encrypt(plainText)
    local c_str = ffi.new("char[?]", 1024 / 8)
    ffi.copy(c_str, plainText)
    local pub = ffi.new("char[?]", #RSA_PUBLIC_KEY)
    ffi.copy(pub, RSA_PUBLIC_KEY)
    local cipherText = ffi.new("char[?]", 2048)
    local cipherLen = rsa.public_encrypt(c_str, #plainText, pub, cipherText)
    if cipherLen == -1 then
        return -1, nil
    end    
    return cipherLen, basexx.to_base64(ffi.string(cipherText, cipherLen))
end

function _M.rsa_decrypt(cipherLen, b64cipherText)
    local c_str = ffi.new("char[?]", cipherLen + 1)
    ffi.copy(c_str, basexx.from_base64(b64cipherText))
    local pri = ffi.new("char[?]", #RSA_PRIV_KEY)
    ffi.copy(pri, RSA_PRIV_KEY)
    local plainText = ffi.new("char[?]", 2048)
    local plainLen = rsa.private_decrypt(c_str, cipherLen, pri, plainText)
    if plainLen == -1 then
        return nil
    end    
    return ffi.string(plainText, plainLen)
end

return _M