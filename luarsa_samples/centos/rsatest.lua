local rsautils = require('rsautils')

local src_str = "my name is ecofast小0胡！!"
print(src_str)
local cipherLen, cipher = rsautils.rsa_encrypt(src_str)
if cipherLen ~= -1 then
    local plain = rsautils.rsa_decrypt(cipherLen, cipher)    
    print("src text:", plain)
        
    print("=========================")
    local txt2 = rsautils.rsa_decrypt(128, "aeMIl3wyPP/DIJLudq49k1YeK9o6QhrScyjy2JHcJ7CmFOpQAmbwLxOe/rWigSYeWbAMUw2MB1KTIsool9zEuOSaoiZtgjfpDvf5g/MZUjPAmDofKVutG9xJNonVoK6usHKVcR7wozq/tJ8h/CUWyKGHnLgkxvU3ObbhLPm/wwI=")    
    print("src text2:", txt2)    
end