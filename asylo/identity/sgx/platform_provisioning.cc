/*
 *
 * Copyright 2019 Asylo authors
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

#include "asylo/identity/sgx/platform_provisioning.h"

#include <cstdint>
#include <limits>

#include "absl/strings/str_cat.h"
#include "asylo/crypto/util/trivial_object_util.h"

namespace asylo {
namespace sgx {
namespace {

// The maximum value of a PceSvn's |value| field.
constexpr uint32_t kPceSvnMaxValue = std::numeric_limits<uint16_t>::max();

// The maximum value of a PceId's |value| field.
constexpr uint32_t kPceIdMaxValue = std::numeric_limits<uint16_t>::max();

}  // namespace

Status ValidatePpid(const Ppid &ppid) {
  if (!ppid.has_value()) {
    return Status(error::GoogleError::INVALID_ARGUMENT,
                  "Ppid does not have a \"value\" field");
  }

  if (ppid.value().size() != kPpidSize) {
    return Status(
        error::GoogleError::INVALID_ARGUMENT,
        absl::StrCat(
            "Ppid's \"value\" field has an invalid size (must be exactly ",
            kPpidSize, " bytes)"));
  }

  return Status::OkStatus();
}

Status ValidateCpuSvn(const CpuSvn &cpu_svn) {
  if (!cpu_svn.has_value()) {
    return Status(error::GoogleError::INVALID_ARGUMENT,
                  "CpuSvn does not have a \"value\" field");
  }

  if (cpu_svn.value().size() != kCpusvnSize) {
    return Status(
        error::GoogleError::INVALID_ARGUMENT,
        absl::StrCat(
            "CpuSvn's \"value\" field has an invalid size (must be exactly ",
            kCpusvnSize, " bytes)"));
  }

  return Status::OkStatus();
}

Status ValidatePceSvn(const PceSvn &pce_svn) {
  if (!pce_svn.has_value()) {
    return Status(error::GoogleError::INVALID_ARGUMENT,
                  "PceSvn does not have a \"value\" field");
  }

  if (pce_svn.value() > kPceSvnMaxValue) {
    return Status(
        error::GoogleError::INVALID_ARGUMENT,
        absl::StrCat(
            "PceSvn's \"value\" field is too large (must be less than ",
            kPceSvnMaxValue, ")"));
  }

  return Status::OkStatus();
}

Status ValidatePceId(const PceId &pce_id) {
  if (!pce_id.has_value()) {
    return Status(error::GoogleError::INVALID_ARGUMENT,
                  "PceId does not have a \"value\" field");
  }

  if (pce_id.value() > kPceIdMaxValue) {
    return Status(
        error::GoogleError::INVALID_ARGUMENT,
        absl::StrCat("PceId's \"value\" field is too large (must be less than ",
                     kPceIdMaxValue, ")"));
  }

  return Status::OkStatus();
}

Status ValidateReportProto(const ReportProto &report_proto) {
  return ConvertReportProtoToHardwareReport(report_proto).status();
}

Status ValidateTargetInfoProto(const TargetInfoProto &target_info_proto) {
  return ConvertTargetInfoProtoToTargetinfo(target_info_proto).status();
}

StatusOr<Report> ConvertReportProtoToHardwareReport(
    const ReportProto &report_proto) {
  if (!report_proto.has_value()) {
    return Status(error::GoogleError::INVALID_ARGUMENT,
                  "ReportProto does not have a \"value\" field");
  }

  Report report;
  ASYLO_RETURN_IF_ERROR(
      SetTrivialObjectFromBinaryString<Report>(report_proto.value(), &report));
  return report;
}

StatusOr<Targetinfo> ConvertTargetInfoProtoToTargetinfo(
    const TargetInfoProto &target_info_proto) {
  if (!target_info_proto.has_value()) {
    return Status(error::GoogleError::INVALID_ARGUMENT,
                  "TargetInfoProto does not have a \"value\" field");
  }

  Targetinfo targetinfo;
  ASYLO_RETURN_IF_ERROR(SetTrivialObjectFromBinaryString<Targetinfo>(
      target_info_proto.value(), &targetinfo));
  return targetinfo;
}

}  // namespace sgx
}  // namespace asylo
