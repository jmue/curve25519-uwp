#include "pch.h"
#include "Curve25519Native.h"
#include "Curve25519_Internal.h" //curve25519_donna
#include "ed25519\additions\curve_sigs.h" //curve25519_sign

#include <memory>

using namespace curve25519;

Curve25519Native::Curve25519Native()
{	
}

//May be used in the future to distinguish native and alternative
//Curve25519 implementations (as it's used in TextSecure).
bool Curve25519Native::isNative()
{
  return true;
}

array<uint8_t>^ Curve25519Native::calculateAgreement(array<uint8_t>^ ourPrivate, array<uint8_t>^ theirPublic)
{
  if (ourPrivate->Length != CURVE25519_PRIV_KEY_LEN)
  {
    throw gcnew System::Exception(CURVE25519_PRIV_KEY_LEN_ERR_MSG);
  }

  if (theirPublic->Length != CURVE25519_PUB_KEY_LEN)
  {
    throw gcnew System::Exception(CURVE25519_PUB_KEY_LEN_ERR_MSG);
  }

  pin_ptr<uint8_t> ourPrivatePin = &ourPrivate[0];
  pin_ptr<uint8_t> theirPublicPin = &theirPublic[0];

  uint8_t sharedSecret[CURVE25519_SHARED_KEY_LEN];
  memset(sharedSecret, 0, sizeof(sharedSecret));
  curve25519_donna(sharedSecret, ourPrivatePin, theirPublicPin);

  array<uint8_t>^ sharedSecretBuf = gcnew array<uint8_t>(sizeof(sharedSecret));
  System::Runtime::InteropServices::Marshal::Copy(System::IntPtr(sharedSecret), sharedSecretBuf, 0, sizeof(sharedSecret));
  return sharedSecretBuf;
}

array<uint8_t>^ Curve25519Native::generatePublicKey(array<uint8_t>^ privateKey)
{
  if (privateKey->Length != CURVE25519_PRIV_KEY_LEN)
  {
    throw gcnew System::Exception(CURVE25519_PRIV_KEY_LEN_ERR_MSG);
  }

  pin_ptr<uint8_t> privateKeyPin = &privateKey[0];

  static const uint8_t basepoint[32] = { 9 };

  uint8_t publicKey[CURVE25519_PUB_KEY_LEN];
  memset(publicKey, 0, sizeof(publicKey));
  curve25519_donna(publicKey, privateKeyPin, basepoint);

  array<uint8_t>^ publicKeyBuf = gcnew array<uint8_t>(sizeof(publicKey));
  System::Runtime::InteropServices::Marshal::Copy(System::IntPtr(publicKey), publicKeyBuf, 0, sizeof(publicKey));
  return publicKeyBuf;
}

array<uint8_t>^ Curve25519Native::generatePrivateKey()
{
  array<uint8_t>^ randomBytes = gcnew array<uint8_t>(32);

  System::Security::Cryptography::RNGCryptoServiceProvider^ rng = gcnew System::Security::Cryptography::RNGCryptoServiceProvider();
  rng->GetBytes(randomBytes);

  return generatePrivateKey(randomBytes);
}

array<uint8_t>^ Curve25519Native::generatePrivateKey(array<uint8_t>^ random)
{
  if (random->Length != CURVE25519_PRIV_KEY_LEN)
  {
    throw gcnew System::Exception(CURVE25519_PRIV_KEY_LEN_ERR_MSG);
  }

  pin_ptr<uint8_t> randomPin = &random[0];

  uint8_t privateKey[CURVE25519_PRIV_KEY_LEN];
  memcpy(privateKey, randomPin, sizeof(privateKey));

  //These appear to be performance related adjustments for Curve25519
  //http://crypto.stackexchange.com/questions/11810/when-using-curve25519-why-does-the-private-key-always-have-a-fixed-bit-at-2254
  privateKey[0] &= 248;
  privateKey[31] &= 127;
  privateKey[31] |= 64;

  array<uint8_t>^ privateKeyBuf = gcnew array<uint8_t>(sizeof(privateKey));
  System::Runtime::InteropServices::Marshal::Copy(System::IntPtr(privateKey), privateKeyBuf, 0, sizeof(privateKey));
  return privateKeyBuf;
}

array<uint8_t>^ Curve25519Native::calculateSignature(array<uint8_t>^ random, array<uint8_t>^ privateKey, array<uint8_t>^ message)
{
  if (privateKey->Length != CURVE25519_PRIV_KEY_LEN)
  {
    throw gcnew System::Exception(CURVE25519_PRIV_KEY_LEN_ERR_MSG);
  }

  pin_ptr<uint8_t> randomPin = &random[0];
  pin_ptr<uint8_t> privateKeyPin = &privateKey[0];
  pin_ptr<uint8_t> messagePin = &message[0];

  uint8_t signature[CURVE25519_SIG_LEN];
  memset(signature, 0, sizeof(signature));

  if (0 != curve25519_sign(signature, privateKeyPin, messagePin, message->Length, randomPin))
  {
    throw gcnew System::Exception(CURVE25519_SIG_FAILED_MSG);
  }

  array<uint8_t>^ signatureBuf = gcnew array<uint8_t>(sizeof(signature));
  System::Runtime::InteropServices::Marshal::Copy(System::IntPtr(signature), signatureBuf, 0, sizeof(signature));
  return signatureBuf;
}

bool Curve25519Native::verifySignature(array<uint8_t>^ publicKey, array<uint8_t>^ message, array<uint8_t>^ signature)
{
  pin_ptr<uint8_t> publicKeyPin = &publicKey[0];
  pin_ptr<uint8_t> messagePin = &message[0];
  pin_ptr<uint8_t> signaturePin = &signature[0];

  return (0 == curve25519_verify(signaturePin, publicKeyPin, messagePin, message->Length));
}
