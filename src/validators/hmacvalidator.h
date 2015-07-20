// Copyright (c) 2015 Erwin Jansen
//
// MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#ifndef SRC_VALIDATORS_HMACVALIDATOR_H_
#define SRC_VALIDATORS_HMACVALIDATOR_H_

#include <openssl/hmac.h>
#include <string>
#include "validators/messagevalidator.h"

// Maximum length of a signature in bytes
// Note that SHA512 is 64 bytes.
#define MAX_HMAC_KEYLENGTH 64

/**
 * Can sign & validate a message using an openssl digest function.
 */
class HMACValidator : public MessageSigner {
 public:
  explicit HMACValidator(const char *algorithm);
  explicit HMACValidator(const char *algorithm, const EVP_MD *md, const std::string &key);
  virtual ~HMACValidator();

  bool Verify(json_t *jsonHeader, const uint8_t *header, size_t num_header,
              const uint8_t *signature, size_t num_signature) override;
  bool Sign(const uint8_t *header, size_t num_header,
            uint8_t *signature, size_t *num_signature) override;

  inline unsigned int key_size() const { return key_size_; }
  inline const char *algorithm() const { return algorithm_; }
  std::string toJson() const override;

 private:
  HMACValidator(const HMACValidator &);
  HMACValidator & operator=(const HMACValidator &);
  static int const_time_cmp(const uint8_t* a, const uint8_t* b, const size_t size);

  const EVP_MD* md_;
  const char *algorithm_;
  unsigned int key_size_;
  std::string key_;
};

class HS256Validator : public HMACValidator {
 public:
  explicit HS256Validator(const std::string &key) : HMACValidator("HS256", EVP_sha256(), key) { }
};

class HS384Validator : public HMACValidator {
 public:
  explicit HS384Validator(const std::string &key) : HMACValidator("HS384", EVP_sha384(), key) { }
};

class HS512Validator : public HMACValidator {
 public:
  explicit HS512Validator(const std::string &key) : HMACValidator("HS512", EVP_sha512(), key) { }
};
#endif  // SRC_VALIDATORS_HMACVALIDATOR_H_
