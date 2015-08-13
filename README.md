# curve25519-uwp
An implementation of curve 25519, ported from on curve25519-java [https://github.com/WhisperSystems/curve25519-java].

This is a C++ WinRT component, suitable for use in UWP projects on Windows 10.

Take a look at the unit tests to see how to generate public and private keys.

To use the NuGet package, https://www.nuget.org/packages/curve25519-uwp/

Note that you may need to manually add some XML to your Package.appxmanifest file.
Here is an example: https://gist.github.com/langboost/3d5e1a4c70ca40a7fd12
