/**
 * Autogenerated by Thrift Compiler (0.9.3)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef ReCaptcha_TYPES_H
#define ReCaptcha_TYPES_H

#include <iosfwd>

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <thrift/cxxfunctional.h>




struct RecognizeResultConfidence {
  enum type {
    UNKNOWN = 0,
    GUESS = 1,
    RECOGNIZE = 2
  };
};

extern const std::map<int, const char*> _RecognizeResultConfidence_VALUES_TO_NAMES;

typedef class FontCenterScanResponse FontCenterScanIndexResponse;

class WindowArea;

class FontCenterScanRequest;

class FontCenterScanResponse;

class FontCenterScanIndexRequest;

class FontCenterScanIndexFeedbackRequest;

class FontCenterScanIndexFeedbackResponse;


class WindowArea {
 public:

  WindowArea(const WindowArea&);
  WindowArea& operator=(const WindowArea&);
  WindowArea() : left(0), right(0), top(0), bottom(0) {
  }

  virtual ~WindowArea() throw();
  int32_t left;
  int32_t right;
  int32_t top;
  int32_t bottom;

  void __set_left(const int32_t val);

  void __set_right(const int32_t val);

  void __set_top(const int32_t val);

  void __set_bottom(const int32_t val);

  bool operator == (const WindowArea & rhs) const
  {
    if (!(left == rhs.left))
      return false;
    if (!(right == rhs.right))
      return false;
    if (!(top == rhs.top))
      return false;
    if (!(bottom == rhs.bottom))
      return false;
    return true;
  }
  bool operator != (const WindowArea &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const WindowArea & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(WindowArea &a, WindowArea &b);

inline std::ostream& operator<<(std::ostream& out, const WindowArea& obj)
{
  obj.printTo(out);
  return out;
}


class FontCenterScanRequest {
 public:

  FontCenterScanRequest(const FontCenterScanRequest&);
  FontCenterScanRequest& operator=(const FontCenterScanRequest&);
  FontCenterScanRequest() : image() {
  }

  virtual ~FontCenterScanRequest() throw();
  std::string image;
  std::vector<WindowArea>  centerWindows;

  void __set_image(const std::string& val);

  void __set_centerWindows(const std::vector<WindowArea> & val);

  bool operator == (const FontCenterScanRequest & rhs) const
  {
    if (!(image == rhs.image))
      return false;
    if (!(centerWindows == rhs.centerWindows))
      return false;
    return true;
  }
  bool operator != (const FontCenterScanRequest &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FontCenterScanRequest & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(FontCenterScanRequest &a, FontCenterScanRequest &b);

inline std::ostream& operator<<(std::ostream& out, const FontCenterScanRequest& obj)
{
  obj.printTo(out);
  return out;
}

typedef struct _FontCenterScanResponse__isset {
  _FontCenterScanResponse__isset() : message(false) {}
  bool message :1;
} _FontCenterScanResponse__isset;

class FontCenterScanResponse {
 public:

  FontCenterScanResponse(const FontCenterScanResponse&);
  FontCenterScanResponse& operator=(const FontCenterScanResponse&);
  FontCenterScanResponse() : status(0), message(), guessId(0) {
  }

  virtual ~FontCenterScanResponse() throw();
  int32_t status;
  std::string message;
  std::vector<std::string>  results;
  int32_t guessId;

  _FontCenterScanResponse__isset __isset;

  void __set_status(const int32_t val);

  void __set_message(const std::string& val);

  void __set_results(const std::vector<std::string> & val);

  void __set_guessId(const int32_t val);

  bool operator == (const FontCenterScanResponse & rhs) const
  {
    if (!(status == rhs.status))
      return false;
    if (__isset.message != rhs.__isset.message)
      return false;
    else if (__isset.message && !(message == rhs.message))
      return false;
    if (!(results == rhs.results))
      return false;
    if (!(guessId == rhs.guessId))
      return false;
    return true;
  }
  bool operator != (const FontCenterScanResponse &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FontCenterScanResponse & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(FontCenterScanResponse &a, FontCenterScanResponse &b);

inline std::ostream& operator<<(std::ostream& out, const FontCenterScanResponse& obj)
{
  obj.printTo(out);
  return out;
}


class FontCenterScanIndexRequest {
 public:

  FontCenterScanIndexRequest(const FontCenterScanIndexRequest&);
  FontCenterScanIndexRequest& operator=(const FontCenterScanIndexRequest&);
  FontCenterScanIndexRequest() : image(), desiredIndexValue() {
  }

  virtual ~FontCenterScanIndexRequest() throw();
  std::string image;
  std::vector<WindowArea>  centerWindows1;
  std::vector<WindowArea>  centerWindows2;
  std::string desiredIndexValue;

  void __set_image(const std::string& val);

  void __set_centerWindows1(const std::vector<WindowArea> & val);

  void __set_centerWindows2(const std::vector<WindowArea> & val);

  void __set_desiredIndexValue(const std::string& val);

  bool operator == (const FontCenterScanIndexRequest & rhs) const
  {
    if (!(image == rhs.image))
      return false;
    if (!(centerWindows1 == rhs.centerWindows1))
      return false;
    if (!(centerWindows2 == rhs.centerWindows2))
      return false;
    if (!(desiredIndexValue == rhs.desiredIndexValue))
      return false;
    return true;
  }
  bool operator != (const FontCenterScanIndexRequest &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FontCenterScanIndexRequest & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(FontCenterScanIndexRequest &a, FontCenterScanIndexRequest &b);

inline std::ostream& operator<<(std::ostream& out, const FontCenterScanIndexRequest& obj)
{
  obj.printTo(out);
  return out;
}


class FontCenterScanIndexFeedbackRequest {
 public:

  FontCenterScanIndexFeedbackRequest(const FontCenterScanIndexFeedbackRequest&);
  FontCenterScanIndexFeedbackRequest& operator=(const FontCenterScanIndexFeedbackRequest&);
  FontCenterScanIndexFeedbackRequest() : guessId(0), success(0) {
  }

  virtual ~FontCenterScanIndexFeedbackRequest() throw();
  int32_t guessId;
  bool success;

  void __set_guessId(const int32_t val);

  void __set_success(const bool val);

  bool operator == (const FontCenterScanIndexFeedbackRequest & rhs) const
  {
    if (!(guessId == rhs.guessId))
      return false;
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const FontCenterScanIndexFeedbackRequest &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FontCenterScanIndexFeedbackRequest & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(FontCenterScanIndexFeedbackRequest &a, FontCenterScanIndexFeedbackRequest &b);

inline std::ostream& operator<<(std::ostream& out, const FontCenterScanIndexFeedbackRequest& obj)
{
  obj.printTo(out);
  return out;
}

typedef struct _FontCenterScanIndexFeedbackResponse__isset {
  _FontCenterScanIndexFeedbackResponse__isset() : message(false) {}
  bool message :1;
} _FontCenterScanIndexFeedbackResponse__isset;

class FontCenterScanIndexFeedbackResponse {
 public:

  FontCenterScanIndexFeedbackResponse(const FontCenterScanIndexFeedbackResponse&);
  FontCenterScanIndexFeedbackResponse& operator=(const FontCenterScanIndexFeedbackResponse&);
  FontCenterScanIndexFeedbackResponse() : status(0), message() {
  }

  virtual ~FontCenterScanIndexFeedbackResponse() throw();
  int32_t status;
  std::string message;

  _FontCenterScanIndexFeedbackResponse__isset __isset;

  void __set_status(const int32_t val);

  void __set_message(const std::string& val);

  bool operator == (const FontCenterScanIndexFeedbackResponse & rhs) const
  {
    if (!(status == rhs.status))
      return false;
    if (__isset.message != rhs.__isset.message)
      return false;
    else if (__isset.message && !(message == rhs.message))
      return false;
    return true;
  }
  bool operator != (const FontCenterScanIndexFeedbackResponse &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FontCenterScanIndexFeedbackResponse & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(FontCenterScanIndexFeedbackResponse &a, FontCenterScanIndexFeedbackResponse &b);

inline std::ostream& operator<<(std::ostream& out, const FontCenterScanIndexFeedbackResponse& obj)
{
  obj.printTo(out);
  return out;
}



#endif
