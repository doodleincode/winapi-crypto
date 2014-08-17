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


Building the project
-------------
The project was made in Visual Studio 2013. The solution is included, if you have VS 2013 it should open out-of-the-box. It may or may not be so straight forward with old versions of Visual Studio. If that's the case, each project depends on the following libraries:

Add these under Linker -> Input -> Additional Dependencies in each project's properties:
* Crypt32.lib
* Bcrypt.lib

Remove pre-compiled headers if your project is setup to use that.

For the KeyGen and Test projects, add the CCrypto project as a dependency. In the project properties, under Common Properties, add a new reference to CCrypto.

The CCrypto project is a static library project. Set Config Type to "Static Library" under Configuration Properties -> General.

Be sure to make these settings on both Release and Debug builds.
