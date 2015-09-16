#pragma once

#define CURVE25519_SHARED_KEY_LEN 32
#define CURVE25519_PRIV_KEY_LEN 32
#define CURVE25519_PUB_KEY_LEN 32
#define CURVE25519_SIG_LEN 64
#define CURVE25519_PRIV_KEY_LEN_ERR_MSG "CURVE25519_PRIV_KEY_LEN_ERR_MSG"
#define CURVE25519_PUB_KEY_LEN_ERR_MSG "CURVE25519_PUB_KEY_LEN_ERR_MSG"
#define CURVE25519_SIG_FAILED_MSG "CURVE25519_SIG_FAILED_MSG"

namespace curve25519
{
  //ENTRY POINT: This class is activatable in other C# projects. Access Curve25519 through this.
  public ref class Curve25519Native sealed
  {
  public:
    Curve25519Native();

    //This class implements the interface for Curve25519Provider from curve25519-java's code.
    //Note: In C# code, "Platform::Array<byte>^" should be treated/cast as signed bytes (sbyte) to match with Java's byte type.
    bool isNative();
    array<uint8_t>^ calculateAgreement(array<uint8_t>^ ourPrivate, array<uint8_t>^ theirPublic);

    array<uint8_t>^ generatePublicKey(array<uint8_t>^ privateKey);
    
    array<uint8_t>^ generatePrivateKey();		
    array<uint8_t>^ generatePrivateKey(array<uint8_t>^ random);

    array<uint8_t>^ calculateSignature(array<uint8_t>^ random, array<uint8_t>^ privateKey, array<uint8_t>^ message);
    bool verifySignature(array<uint8_t>^ publicKey, array<uint8_t>^ message, array<uint8_t>^ signature);
  };
}
