//
// Created by wen on 2016-05-02.
//

#ifndef OMS_OMS_MESSAGE_H
#define OMS_OMS_MESSAGE_H

#include <stdint.h>
#include <string>


namespace oms {

static const uint64_t kOMSApiVersion = 0;
static const uint16_t kOMSAppId = 120;
static const uint16_t kOMSServiceId = 120;
static const size_t kBufferMax = 2048;



template<typename T>
static void SbeDecode(char* message, oms_api_sbe::MessageHeader &hdr, T &qry) {
  hdr.wrap(message, 0, kOMSApiVersion, strlen(message) + 1);
  qry.wrapForDecode((char *) message, hdr.encodedLength(), hdr.blockLength(), hdr.version(),
                    strlen(message) + 1);
}

template<typename T>
static char* SbeEncode(size_t buffer_length, oms_api_sbe::MessageHeader &hdr, T &ans) {
  char* buffer = new char[buffer_length];
  hdr.wrap(buffer, 0, kOMSApiVersion, buffer_length)
  .blockLength(T::sbeBlockLength())
  .templateId(T::sbeTemplateId())
  .schemaId(T::sbeSchemaId())
  .version(T::sbeSchemaVersion());
  ans.wrapForEncode(buffer, hdr.encodedLength(), buffer_length);
  return buffer;
}


} //namespace oms

#endif //OMS_OMS_MESSAGE_H
