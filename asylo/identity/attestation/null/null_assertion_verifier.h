/*
 *
 * Copyright 2017 Asylo authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef ASYLO_IDENTITY_ATTESTATION_NULL_NULL_ASSERTION_VERIFIER_H_
#define ASYLO_IDENTITY_ATTESTATION_NULL_NULL_ASSERTION_VERIFIER_H_

#include <string>

#include "absl/synchronization/mutex.h"
#include "asylo/identity/attestation/enclave_assertion_verifier.h"

namespace asylo {

/// An implementation of the EnclaveAssertionVerifier interface for null
/// assertions.
///
/// NullAssertionVerifier can verify assertions generated by a
/// NullAssertionGenerator, although there are no cryptographic checks involved
/// in verifying such an assertion.
///
/// The identity extracted from a verified null assertion is the null-terminated
/// string "Null Identity".
class NullAssertionVerifier final : public EnclaveAssertionVerifier {
 public:
  /// Constructs an uninitialized NullAssertionVerifier.
  ///
  /// The verifier can be initialized via a call to Initialize().
  NullAssertionVerifier();

  ///////////////////////////////////////////
  //   From AssertionAuthority interface.  //
  ///////////////////////////////////////////

  Status Initialize(const std::string &config) override;

  bool IsInitialized() const override;

  EnclaveIdentityType IdentityType() const override;

  std::string AuthorityType() const override;

  ///////////////////////////////////////////
  //    From AssertionVerifier interface.  //
  ///////////////////////////////////////////

  Status CreateAssertionRequest(AssertionRequest *request) const override;

  StatusOr<bool> CanVerify(const AssertionOffer &offer) const override;

  Status Verify(const std::string &user_data, const Assertion &assertion,
                EnclaveIdentity *peer_identity) const override;

 private:
  // Indicates whether this verifier has been initialized.
  bool initialized_ ABSL_GUARDED_BY(initialized_mu_);

  // A mutex that guards the initialized_ member.
  mutable absl::Mutex initialized_mu_;

  // The type of this assertion authority.
  static const char *const authority_type_;

  // The type of enclave identity handled by this verifier.
  static constexpr EnclaveIdentityType identity_type_ = NULL_IDENTITY;
};

}  // namespace asylo

#endif  // ASYLO_IDENTITY_ATTESTATION_NULL_NULL_ASSERTION_VERIFIER_H_
