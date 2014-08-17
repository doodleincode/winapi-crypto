Win32Crypto
===========

A RSA library for Windows that supports symmetric encryption of data and RSA PKI to secure the symmetric key. The library was designed to have an API similar to the file operations in the Windows API.

Disclaimer
-------------
This library has not been vetted! Use at your own risk!

Requirements
-------------
Should work on Windows XP SP3 and later Windows OS.


Usage
-------------
Check out the sample code in the .\Test\ project.

First, you will need to generate a RSA public/private key pair:

```
KeyGen.exe <key_size> <public_key> <private_key>
```

Ater generating a RSA key pair, you can run the sample code. For integration into your own project, you'll still need to use my `KeyGen.exe` tool to generate your public/private keys. Unfortunately I wasn't able to figure out how to import RSA keys generated from other tools such as `Puttygen`.

Known vulnerabilities
-------------
Currently not utilizing IVs. I will be adding this in the near future.
