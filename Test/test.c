#include <stdio.h>
#include <Windows.h>
#include "libccrypto.h"

int main(int argc, char **argv)
{
    // Handles for RSA
    BCRYPT_ALG_HANDLE hAlgo = NULL;
    BCRYPT_KEY_HANDLE hKey = NULL;

    FILE_CRYPTO_INFO cryptoInfo;
    PBYTE psPlainText = NULL;
    DWORD dwBytesWritten = 0;
    DWORD dwBytesRead = 0;
    CCRYPT_STATUS status;

    // TEST DATA //
    PBYTE pbData1 = "Hello world!\n";
    PBYTE pbData2 = "Hello person!\n";
    PBYTE pbData3 = "This is cool!\n";
    
    if (argc != 2) {
        printf("USAGE: %s <encrypt|decrypt>\n\n", argv[0]);
        return -1;
    }

    // --------------------------------------------------------------------------
    //
    //  Testing encrypting/writing to file
    //
    // --------------------------------------------------------------------------

    if (strcmp(argv[1], "encrypt") == 0)
    {
        if (CCRYPT_STATUS_SUCCESS != (status = CCryptCreateFile(&cryptoInfo, TEXT("test.log"), CCRYPT_FILE_WRITE))) {
            printf("CCryptCreateFile error: %x\n", status);
            return -1;
        }

        // Write data to file
        if (!CCryptWriteFile(&cryptoInfo, pbData1, strlen(pbData1), &dwBytesWritten)) {
            printf("CCryptWriteFile error: %d\n", GetLastError());
            return -1;
        }
        if (!CCryptWriteFile(&cryptoInfo, pbData2, strlen(pbData2), &dwBytesWritten)) {
            printf("CCryptWriteFile error: %d\n", GetLastError());
            return -1;
        }
        if (!CCryptWriteFile(&cryptoInfo, pbData3, strlen(pbData3), &dwBytesWritten)) {
            printf("CCryptWriteFile error: %d\n", GetLastError());
            return -1;
        }

        // Import our RSA public key to encrypt AES key
        if (CCRYPT_STATUS_SUCCESS != (status = RSAImportKey(&hAlgo, &hKey, TEXT("public.key"), BCRYPT_RSAPUBLIC_BLOB))) {
            printf("RSAImportKey error (public): %x\n", status);
            return -1;
        }

        // Finalize the file write
        if (!CCryptFinalizeFile(&cryptoInfo, hKey)) {
            printf("CCryptFinalizeFile error: %d\n", GetLastError());
            return -1;
        }

        CCryptCloseHandle(&cryptoInfo);
        BCryptDestroyKey(hKey);
        BCryptCloseAlgorithmProvider(hAlgo, 0);
    }

    // --------------------------------------------------------------------------
    //
    //  Testing reading/decrypting from file
    //
    // --------------------------------------------------------------------------

    else
    {
        if (CCRYPT_STATUS_SUCCESS != (status = CCryptCreateFile(&cryptoInfo, TEXT("test.log"), CCRYPT_FILE_READ))) {
            printf("CCryptCreateFile error: %x\n", status);
            return -1;
        }

        // Import our RSA private key to decrypt AES key
        if (CCRYPT_STATUS_SUCCESS != (status = RSAImportKey(&hAlgo, &hKey, TEXT("private.key"), BCRYPT_RSAPRIVATE_BLOB))) {
            printf("RSAImportKey error (private): %x\n", status);
            return -1;
        }

        if (!CCryptReadFile(&cryptoInfo, hKey, &psPlainText, &dwBytesRead))
        {
            printf("CCryptReadFile error: %x\n", GetLastError());
            return -1;
        }

        printf("Plain text: %s\n\n", psPlainText);

        CCryptCloseHandle(&cryptoInfo);
        BCryptDestroyKey(hKey);
        BCryptCloseAlgorithmProvider(hAlgo, 0);
    }
    
    return 0;
}