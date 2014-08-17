/**
* Helper tool to generate RSA key pairs to use with the LibCrypto library
*
* This tool has not been vetted, use at your own risk!
*
* @author  Daniel Hong
*
* This program is licensed under the GNU GENERAL PUBLIC LICENSE Version 2.
* A LICENSE file should have accompanied this program.
*/

#include <stdio.h>
#include <Windows.h>
#include <bcrypt.h>
#include <Wincrypt.h>
#include "libccrypto.h"

#define NT_SUCCESS(status) (((NTSTATUS)(status)) >= 0)

void ExportKey(BCRYPT_KEY_HANDLE, LPCWSTR, LPSTR);
BOOL Base64Encode(PBYTE, DWORD, LPSTR *, PDWORD);

int main(int argc, char **argv)
{
    BCRYPT_ALG_HANDLE hAlgo = NULL;
    BCRYPT_KEY_HANDLE hKey = NULL;
    NTSTATUS status;

    if (argc != 4) {
        printf("\nUSAGE:\n");
        printf("  %s <key_size> <public_key> <private_key>\n", argv[0]);
        printf("\nARGUMENTS:\n");
        printf("  <key_size>\tThe key size in bits, must be either 2048 or 4096\n");
        printf("  <public_key>\tName of the public key file\n");
        printf("  <private_key>\tName of the private key file\n");
        return -1;
    }

    // Simple sanity check
    if (atoi(argv[1]) != 2048 && atoi(argv[1]) != 4096) {
        printf("The key size must be either 2048 or 4096!\n");
        return -1;
    }

    printf("Generating keys...\n");
    
    if (! NT_SUCCESS(status = BCryptOpenAlgorithmProvider(
            &hAlgo,          // _Out_  BCRYPT_ALG_HANDLE *phAlgorithm,
            BCRYPT_RSA_ALGORITHM,   // _In_   LPCWSTR pszAlgId,
            NULL,                   // _In_   LPCWSTR pszImplementation,
            0                       // _In_   DWORD dwFlags
        )))
    {
        printf("BCryptOpenAlgorithmProvider() error: %x\n", status);
        goto Cleanup;
    }

    if (! NT_SUCCESS(status = BCryptGenerateKeyPair(
            hAlgo, // _Inout_  BCRYPT_ALG_HANDLE hAlgorithm,
            &hKey, // _Out_    BCRYPT_KEY_HANDLE *phKey,
            atoi(argv[1]), // _In_     ULONG dwLength,
            0 // _In_     ULONG dwFlags
        )))
    {
        printf("BCryptGenerateKeyPair() error: %x\n", status);
        goto Cleanup;
    }

    if (! NT_SUCCESS(status = BCryptFinalizeKeyPair(
            hKey, // _Inout_  BCRYPT_KEY_HANDLE hKey,
            0 // _In_     ULONG dwFlags
        )))
    {
        printf("BCryptFinalizeKeyPair() error: %x\n", status);
        goto Cleanup;
    }

    RSAExportKey(hKey, BCRYPT_RSAPUBLIC_BLOB, argv[2]);
    RSAExportKey(hKey, BCRYPT_RSAPRIVATE_BLOB, argv[3]);

    printf("Done!\n");

Cleanup:

    if (hKey != NULL) {
        BCryptDestroyKey(
                hKey // _Inout_  BCRYPT_KEY_HANDLE hKey
            );
    }
    
    if (hAlgo != NULL) {
        BCryptCloseAlgorithmProvider(
                hAlgo, // _Inout_  BCRYPT_ALG_HANDLE hAlgorithm,
                0 // _In_     ULONG dwFlags
            );
    }

    return 0;
}
