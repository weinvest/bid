/**
 * Autogenerated by Thrift Compiler (0.9.3)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "ReCaptcha_types.h"

#include <algorithm>
#include <ostream>

#include <thrift/TToString.h>



int _kRecognizeResultConfidenceValues[] = {
  RecognizeResultConfidence::UNKNOWN,
  RecognizeResultConfidence::GUESS,
  RecognizeResultConfidence::RECOGNIZE
};
const char* _kRecognizeResultConfidenceNames[] = {
  "UNKNOWN",
  "GUESS",
  "RECOGNIZE"
};
const std::map<int, const char*> _RecognizeResultConfidence_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(3, _kRecognizeResultConfidenceValues, _kRecognizeResultConfidenceNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));


WindowArea::~WindowArea() throw() {
}


void WindowArea::__set_left(const int32_t val) {
  this->left = val;
}

void WindowArea::__set_right(const int32_t val) {
  this->right = val;
}

void WindowArea::__set_top(const int32_t val) {
  this->top = val;
}

void WindowArea::__set_bottom(const int32_t val) {
  this->bottom = val;
}

uint32_t WindowArea::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_left = false;
  bool isset_right = false;
  bool isset_top = false;
  bool isset_bottom = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->left);
          isset_left = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->right);
          isset_right = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->top);
          isset_top = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->bottom);
          isset_bottom = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_left)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_right)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_top)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_bottom)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t WindowArea::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("WindowArea");

  xfer += oprot->writeFieldBegin("left", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->left);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("right", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32(this->right);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("top", ::apache::thrift::protocol::T_I32, 3);
  xfer += oprot->writeI32(this->top);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("bottom", ::apache::thrift::protocol::T_I32, 4);
  xfer += oprot->writeI32(this->bottom);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(WindowArea &a, WindowArea &b) {
  using ::std::swap;
  swap(a.left, b.left);
  swap(a.right, b.right);
  swap(a.top, b.top);
  swap(a.bottom, b.bottom);
}

WindowArea::WindowArea(const WindowArea& other0) {
  left = other0.left;
  right = other0.right;
  top = other0.top;
  bottom = other0.bottom;
}
WindowArea& WindowArea::operator=(const WindowArea& other1) {
  left = other1.left;
  right = other1.right;
  top = other1.top;
  bottom = other1.bottom;
  return *this;
}
void WindowArea::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "WindowArea(";
  out << "left=" << to_string(left);
  out << ", " << "right=" << to_string(right);
  out << ", " << "top=" << to_string(top);
  out << ", " << "bottom=" << to_string(bottom);
  out << ")";
}


FontCenterScanRequest::~FontCenterScanRequest() throw() {
}


void FontCenterScanRequest::__set_image(const std::string& val) {
  this->image = val;
}

void FontCenterScanRequest::__set_centerWindows(const std::vector<WindowArea> & val) {
  this->centerWindows = val;
}

uint32_t FontCenterScanRequest::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_image = false;
  bool isset_centerWindows = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readBinary(this->image);
          isset_image = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->centerWindows.clear();
            uint32_t _size2;
            ::apache::thrift::protocol::TType _etype5;
            xfer += iprot->readListBegin(_etype5, _size2);
            this->centerWindows.resize(_size2);
            uint32_t _i6;
            for (_i6 = 0; _i6 < _size2; ++_i6)
            {
              xfer += this->centerWindows[_i6].read(iprot);
            }
            xfer += iprot->readListEnd();
          }
          isset_centerWindows = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_image)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_centerWindows)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t FontCenterScanRequest::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("FontCenterScanRequest");

  xfer += oprot->writeFieldBegin("image", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeBinary(this->image);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("centerWindows", ::apache::thrift::protocol::T_LIST, 2);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRUCT, static_cast<uint32_t>(this->centerWindows.size()));
    std::vector<WindowArea> ::const_iterator _iter7;
    for (_iter7 = this->centerWindows.begin(); _iter7 != this->centerWindows.end(); ++_iter7)
    {
      xfer += (*_iter7).write(oprot);
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(FontCenterScanRequest &a, FontCenterScanRequest &b) {
  using ::std::swap;
  swap(a.image, b.image);
  swap(a.centerWindows, b.centerWindows);
}

FontCenterScanRequest::FontCenterScanRequest(const FontCenterScanRequest& other8) {
  image = other8.image;
  centerWindows = other8.centerWindows;
}
FontCenterScanRequest& FontCenterScanRequest::operator=(const FontCenterScanRequest& other9) {
  image = other9.image;
  centerWindows = other9.centerWindows;
  return *this;
}
void FontCenterScanRequest::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "FontCenterScanRequest(";
  out << "image=" << to_string(image);
  out << ", " << "centerWindows=" << to_string(centerWindows);
  out << ")";
}


FontCenterScanResponse::~FontCenterScanResponse() throw() {
}


void FontCenterScanResponse::__set_status(const int32_t val) {
  this->status = val;
}

void FontCenterScanResponse::__set_message(const std::string& val) {
  this->message = val;
__isset.message = true;
}

void FontCenterScanResponse::__set_results(const std::vector<std::string> & val) {
  this->results = val;
}

uint32_t FontCenterScanResponse::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_status = false;
  bool isset_results = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->status);
          isset_status = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->message);
          this->__isset.message = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->results.clear();
            uint32_t _size10;
            ::apache::thrift::protocol::TType _etype13;
            xfer += iprot->readListBegin(_etype13, _size10);
            this->results.resize(_size10);
            uint32_t _i14;
            for (_i14 = 0; _i14 < _size10; ++_i14)
            {
              xfer += iprot->readString(this->results[_i14]);
            }
            xfer += iprot->readListEnd();
          }
          isset_results = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_status)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_results)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t FontCenterScanResponse::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("FontCenterScanResponse");

  xfer += oprot->writeFieldBegin("status", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->status);
  xfer += oprot->writeFieldEnd();

  if (this->__isset.message) {
    xfer += oprot->writeFieldBegin("message", ::apache::thrift::protocol::T_STRING, 2);
    xfer += oprot->writeString(this->message);
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldBegin("results", ::apache::thrift::protocol::T_LIST, 3);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRING, static_cast<uint32_t>(this->results.size()));
    std::vector<std::string> ::const_iterator _iter15;
    for (_iter15 = this->results.begin(); _iter15 != this->results.end(); ++_iter15)
    {
      xfer += oprot->writeString((*_iter15));
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(FontCenterScanResponse &a, FontCenterScanResponse &b) {
  using ::std::swap;
  swap(a.status, b.status);
  swap(a.message, b.message);
  swap(a.results, b.results);
  swap(a.__isset, b.__isset);
}

FontCenterScanResponse::FontCenterScanResponse(const FontCenterScanResponse& other16) {
  status = other16.status;
  message = other16.message;
  results = other16.results;
  __isset = other16.__isset;
}
FontCenterScanResponse& FontCenterScanResponse::operator=(const FontCenterScanResponse& other17) {
  status = other17.status;
  message = other17.message;
  results = other17.results;
  __isset = other17.__isset;
  return *this;
}
void FontCenterScanResponse::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "FontCenterScanResponse(";
  out << "status=" << to_string(status);
  out << ", " << "message="; (__isset.message ? (out << to_string(message)) : (out << "<null>"));
  out << ", " << "results=" << to_string(results);
  out << ")";
}


FontCenterScanIndexRequest::~FontCenterScanIndexRequest() throw() {
}


void FontCenterScanIndexRequest::__set_image(const std::string& val) {
  this->image = val;
}

void FontCenterScanIndexRequest::__set_centerWindows(const std::vector<WindowArea> & val) {
  this->centerWindows = val;
}

void FontCenterScanIndexRequest::__set_indexAreas(const std::vector<WindowArea> & val) {
  this->indexAreas = val;
}

void FontCenterScanIndexRequest::__set_desiredIndexValue(const std::string& val) {
  this->desiredIndexValue = val;
}

uint32_t FontCenterScanIndexRequest::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_image = false;
  bool isset_centerWindows = false;
  bool isset_indexAreas = false;
  bool isset_desiredIndexValue = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readBinary(this->image);
          isset_image = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->centerWindows.clear();
            uint32_t _size18;
            ::apache::thrift::protocol::TType _etype21;
            xfer += iprot->readListBegin(_etype21, _size18);
            this->centerWindows.resize(_size18);
            uint32_t _i22;
            for (_i22 = 0; _i22 < _size18; ++_i22)
            {
              xfer += this->centerWindows[_i22].read(iprot);
            }
            xfer += iprot->readListEnd();
          }
          isset_centerWindows = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->indexAreas.clear();
            uint32_t _size23;
            ::apache::thrift::protocol::TType _etype26;
            xfer += iprot->readListBegin(_etype26, _size23);
            this->indexAreas.resize(_size23);
            uint32_t _i27;
            for (_i27 = 0; _i27 < _size23; ++_i27)
            {
              xfer += this->indexAreas[_i27].read(iprot);
            }
            xfer += iprot->readListEnd();
          }
          isset_indexAreas = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->desiredIndexValue);
          isset_desiredIndexValue = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_image)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_centerWindows)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_indexAreas)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_desiredIndexValue)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t FontCenterScanIndexRequest::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("FontCenterScanIndexRequest");

  xfer += oprot->writeFieldBegin("image", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeBinary(this->image);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("centerWindows", ::apache::thrift::protocol::T_LIST, 2);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRUCT, static_cast<uint32_t>(this->centerWindows.size()));
    std::vector<WindowArea> ::const_iterator _iter28;
    for (_iter28 = this->centerWindows.begin(); _iter28 != this->centerWindows.end(); ++_iter28)
    {
      xfer += (*_iter28).write(oprot);
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("indexAreas", ::apache::thrift::protocol::T_LIST, 3);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRUCT, static_cast<uint32_t>(this->indexAreas.size()));
    std::vector<WindowArea> ::const_iterator _iter29;
    for (_iter29 = this->indexAreas.begin(); _iter29 != this->indexAreas.end(); ++_iter29)
    {
      xfer += (*_iter29).write(oprot);
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("desiredIndexValue", ::apache::thrift::protocol::T_STRING, 4);
  xfer += oprot->writeString(this->desiredIndexValue);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(FontCenterScanIndexRequest &a, FontCenterScanIndexRequest &b) {
  using ::std::swap;
  swap(a.image, b.image);
  swap(a.centerWindows, b.centerWindows);
  swap(a.indexAreas, b.indexAreas);
  swap(a.desiredIndexValue, b.desiredIndexValue);
}

FontCenterScanIndexRequest::FontCenterScanIndexRequest(const FontCenterScanIndexRequest& other30) {
  image = other30.image;
  centerWindows = other30.centerWindows;
  indexAreas = other30.indexAreas;
  desiredIndexValue = other30.desiredIndexValue;
}
FontCenterScanIndexRequest& FontCenterScanIndexRequest::operator=(const FontCenterScanIndexRequest& other31) {
  image = other31.image;
  centerWindows = other31.centerWindows;
  indexAreas = other31.indexAreas;
  desiredIndexValue = other31.desiredIndexValue;
  return *this;
}
void FontCenterScanIndexRequest::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "FontCenterScanIndexRequest(";
  out << "image=" << to_string(image);
  out << ", " << "centerWindows=" << to_string(centerWindows);
  out << ", " << "indexAreas=" << to_string(indexAreas);
  out << ", " << "desiredIndexValue=" << to_string(desiredIndexValue);
  out << ")";
}


FontCenterScanIndexResponse::~FontCenterScanIndexResponse() throw() {
}


void FontCenterScanIndexResponse::__set_status(const int32_t val) {
  this->status = val;
}

void FontCenterScanIndexResponse::__set_message(const std::string& val) {
  this->message = val;
__isset.message = true;
}

void FontCenterScanIndexResponse::__set_results(const std::vector<std::string> & val) {
  this->results = val;
}

void FontCenterScanIndexResponse::__set_guessId(const int32_t val) {
  this->guessId = val;
}

uint32_t FontCenterScanIndexResponse::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_status = false;
  bool isset_results = false;
  bool isset_guessId = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->status);
          isset_status = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->message);
          this->__isset.message = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->results.clear();
            uint32_t _size32;
            ::apache::thrift::protocol::TType _etype35;
            xfer += iprot->readListBegin(_etype35, _size32);
            this->results.resize(_size32);
            uint32_t _i36;
            for (_i36 = 0; _i36 < _size32; ++_i36)
            {
              xfer += iprot->readString(this->results[_i36]);
            }
            xfer += iprot->readListEnd();
          }
          isset_results = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->guessId);
          isset_guessId = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_status)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_results)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_guessId)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t FontCenterScanIndexResponse::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("FontCenterScanIndexResponse");

  xfer += oprot->writeFieldBegin("status", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->status);
  xfer += oprot->writeFieldEnd();

  if (this->__isset.message) {
    xfer += oprot->writeFieldBegin("message", ::apache::thrift::protocol::T_STRING, 2);
    xfer += oprot->writeString(this->message);
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldBegin("results", ::apache::thrift::protocol::T_LIST, 3);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRING, static_cast<uint32_t>(this->results.size()));
    std::vector<std::string> ::const_iterator _iter37;
    for (_iter37 = this->results.begin(); _iter37 != this->results.end(); ++_iter37)
    {
      xfer += oprot->writeString((*_iter37));
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("guessId", ::apache::thrift::protocol::T_I32, 4);
  xfer += oprot->writeI32(this->guessId);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(FontCenterScanIndexResponse &a, FontCenterScanIndexResponse &b) {
  using ::std::swap;
  swap(a.status, b.status);
  swap(a.message, b.message);
  swap(a.results, b.results);
  swap(a.guessId, b.guessId);
  swap(a.__isset, b.__isset);
}

FontCenterScanIndexResponse::FontCenterScanIndexResponse(const FontCenterScanIndexResponse& other38) {
  status = other38.status;
  message = other38.message;
  results = other38.results;
  guessId = other38.guessId;
  __isset = other38.__isset;
}
FontCenterScanIndexResponse& FontCenterScanIndexResponse::operator=(const FontCenterScanIndexResponse& other39) {
  status = other39.status;
  message = other39.message;
  results = other39.results;
  guessId = other39.guessId;
  __isset = other39.__isset;
  return *this;
}
void FontCenterScanIndexResponse::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "FontCenterScanIndexResponse(";
  out << "status=" << to_string(status);
  out << ", " << "message="; (__isset.message ? (out << to_string(message)) : (out << "<null>"));
  out << ", " << "results=" << to_string(results);
  out << ", " << "guessId=" << to_string(guessId);
  out << ")";
}


FontCenterScanIndexFeedbackRequest::~FontCenterScanIndexFeedbackRequest() throw() {
}


void FontCenterScanIndexFeedbackRequest::__set_guessId(const int32_t val) {
  this->guessId = val;
}

void FontCenterScanIndexFeedbackRequest::__set_success(const bool val) {
  this->success = val;
}

uint32_t FontCenterScanIndexFeedbackRequest::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_guessId = false;
  bool isset_success = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->guessId);
          isset_guessId = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_BOOL) {
          xfer += iprot->readBool(this->success);
          isset_success = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_guessId)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_success)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t FontCenterScanIndexFeedbackRequest::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("FontCenterScanIndexFeedbackRequest");

  xfer += oprot->writeFieldBegin("guessId", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->guessId);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("success", ::apache::thrift::protocol::T_BOOL, 2);
  xfer += oprot->writeBool(this->success);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(FontCenterScanIndexFeedbackRequest &a, FontCenterScanIndexFeedbackRequest &b) {
  using ::std::swap;
  swap(a.guessId, b.guessId);
  swap(a.success, b.success);
}

FontCenterScanIndexFeedbackRequest::FontCenterScanIndexFeedbackRequest(const FontCenterScanIndexFeedbackRequest& other40) {
  guessId = other40.guessId;
  success = other40.success;
}
FontCenterScanIndexFeedbackRequest& FontCenterScanIndexFeedbackRequest::operator=(const FontCenterScanIndexFeedbackRequest& other41) {
  guessId = other41.guessId;
  success = other41.success;
  return *this;
}
void FontCenterScanIndexFeedbackRequest::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "FontCenterScanIndexFeedbackRequest(";
  out << "guessId=" << to_string(guessId);
  out << ", " << "success=" << to_string(success);
  out << ")";
}


FontCenterScanIndexFeedbackResponse::~FontCenterScanIndexFeedbackResponse() throw() {
}


void FontCenterScanIndexFeedbackResponse::__set_status(const int32_t val) {
  this->status = val;
}

void FontCenterScanIndexFeedbackResponse::__set_message(const std::string& val) {
  this->message = val;
__isset.message = true;
}

uint32_t FontCenterScanIndexFeedbackResponse::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_status = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->status);
          isset_status = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->message);
          this->__isset.message = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_status)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t FontCenterScanIndexFeedbackResponse::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("FontCenterScanIndexFeedbackResponse");

  xfer += oprot->writeFieldBegin("status", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->status);
  xfer += oprot->writeFieldEnd();

  if (this->__isset.message) {
    xfer += oprot->writeFieldBegin("message", ::apache::thrift::protocol::T_STRING, 2);
    xfer += oprot->writeString(this->message);
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(FontCenterScanIndexFeedbackResponse &a, FontCenterScanIndexFeedbackResponse &b) {
  using ::std::swap;
  swap(a.status, b.status);
  swap(a.message, b.message);
  swap(a.__isset, b.__isset);
}

FontCenterScanIndexFeedbackResponse::FontCenterScanIndexFeedbackResponse(const FontCenterScanIndexFeedbackResponse& other42) {
  status = other42.status;
  message = other42.message;
  __isset = other42.__isset;
}
FontCenterScanIndexFeedbackResponse& FontCenterScanIndexFeedbackResponse::operator=(const FontCenterScanIndexFeedbackResponse& other43) {
  status = other43.status;
  message = other43.message;
  __isset = other43.__isset;
  return *this;
}
void FontCenterScanIndexFeedbackResponse::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "FontCenterScanIndexFeedbackResponse(";
  out << "status=" << to_string(status);
  out << ", " << "message="; (__isset.message ? (out << to_string(message)) : (out << "<null>"));
  out << ")";
}


