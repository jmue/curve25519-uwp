using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections.Generic;
using System.Security.Cryptography;

namespace curve25519_test
{
    /// <summary>
    /// These tests have been ported over from curve25519-java to use the same fixed
    /// keys for unit testing. This should help us make sure our libraries can work
    /// with each other. Function names will match those in the Java test code.
    /// </summary>
    [TestClass]
    public class JavaCompatibilityTests
    {
        #region Test helper code
        private curve25519.Curve25519Native curve25519;
        private const int EXPECTED_LEN = 32;
        private static byte[] GetRandomBuffer(int expectedLen)
        {
            byte[] randomBytes = new byte[EXPECTED_LEN];

            RNGCryptoServiceProvider rng = new RNGCryptoServiceProvider();
            rng.GetBytes(randomBytes);

            return randomBytes;
        }
        #endregion

        [TestInitialize]
        public void Initialize()
        {
            curve25519 = new curve25519.Curve25519Native();
        }

        [TestCleanup]
        public void Cleanup()
        {
            curve25519 = null;
        }

        [TestMethod]
        public void testKeyGen()
        {
            byte[] keyIn = new byte[32];
            byte[] keyOut = null;

            keyIn[0] = 123;

            for (int count = 0; count < 1000; count++)
            {
                keyOut = curve25519.generatePublicKey(keyIn);
                Array.Copy(keyOut, 0, keyIn, 0, EXPECTED_LEN);
            }

            byte[] result2 = new byte[]
            {
                0xa2, 0x3c, 0x84, 0x09, 0xf2,
                0x93, 0xb4, 0x42, 0x6a, 0xf5,
                0xe5, 0xe7, 0xca, 0xee, 0x22,
                0xa0, 0x01, 0xc7, 0x9a, 0xca,
                0x1a, 0xf2, 0xea, 0xcb, 0x4d,
                0xdd, 0xfa, 0x05, 0xf8, 0xbc,
                0x7f, 0x37
            };

            CollectionAssert.AreEqual(result2, keyOut);
        }

        [TestMethod]
        public void testEcDh()
        {
            byte[] p = new byte[32];
            byte[] q = null;
            byte[] n = new byte[32];

            p[0] = 100;
            n[0] = 100;

            n = curve25519.generatePrivateKey(n);

            for (int count = 0; count < 1000; count++)
            {
                q = curve25519.calculateAgreement(n, p);
                Array.Copy(q, 0, p, 0, 32);
                q = curve25519.calculateAgreement(n, p);
                Array.Copy(q, 0, n, 0, 32);
                n = curve25519.generatePrivateKey(n);
            }

            byte[] result = new byte[]
            {
                0xce, 0xb4, 0x4e, 0xd6, 0x4a,
                0xd4, 0xc2, 0xb5, 0x43, 0x9d,
                0x25, 0xde, 0xb1, 0x10, 0xa8,
                0xd7, 0x2e, 0xb3, 0xe3, 0x8e,
                0xf4, 0x8a, 0x42, 0x73, 0xb1,
                0x1b, 0x4b, 0x13, 0x8d, 0x17,
                0xf9, 0x34
            };

            List<byte> qList = new List<byte>(q);
            List<byte> resultList = new List<byte>(result);
            CollectionAssert.AreEqual(resultList, qList);
        }

        // FIXME: There's no actual vector here.  If verifySignature is broken and always returns true,
        // this test will pass.
        [TestMethod]
        public void testSignVerify()
        {
            byte[] msg = new byte[100];
            byte[] sig_out = new byte[64];
            byte[] privkey = new byte[32];
            byte[] pubkey = new byte[32];
            byte[] random = new byte[64];

            privkey[0] = 123;

            for (int count = 0; count < 1000; count++)
            {
                privkey = curve25519.generatePrivateKey(privkey);
                pubkey = curve25519.generatePublicKey(privkey);
                sig_out = curve25519.calculateSignature(random, privkey, msg);

                Assert.IsTrue(curve25519.verifySignature(pubkey, msg, sig_out));

                Array.Copy(sig_out, 0, privkey, 0, 32);
            }
        }

        //TODO: Implement testSignVerify_Fail(), and make sure it throws an exception.
    }
}